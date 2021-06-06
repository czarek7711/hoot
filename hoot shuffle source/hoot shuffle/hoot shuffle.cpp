// thanks mvaneerde for the WASAPI code https://github.com/mvaneerde/blog/tree/develop/meters/meters (I'm sure more of this could be trimmed out but I'm lazy)
// the rest developed by Shadowdevo
#include <initguid.h>
#include <windows.h>
#include <math.h>
#include <appmodel.h>
#include <cguid.h>
#include <atlbase.h>
#include <stdio.h>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <iostream>
#include <chrono>
#include <ctime>

#define AUDCLNT_S_NO_SINGLE_PROCESS AUDCLNT_SUCCESS (0x00d)

#define LOG(format, ...) wprintf(format L"\n", __VA_ARGS__)

#define GUID_FORMAT L"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}"
#define GUID_VALUES(g) \
    g.Data1, g.Data2, g.Data3, \
    g.Data4[0], g.Data4[1], g.Data4[2], g.Data4[3],  \
    g.Data4[4], g.Data4[5], g.Data4[6], g.Data4[7]

int i = 0;
int timesRepeated = 0;
float lastPeakValue = 0;
bool hootFound = false;
bool checkForHoot = true;

class CoUninitializeOnExit {
public:
    CoUninitializeOnExit() {}
    ~CoUninitializeOnExit() {
        CoUninitialize();
    }
};

class CloseHandleOnExit {
public:
    CloseHandleOnExit(HANDLE h) : m_h(h) {}
    ~CloseHandleOnExit() {
        CloseHandle(m_h);
    }
private:
    HANDLE m_h;
};

class PropVariantClearOnExit {
public:
    PropVariantClearOnExit(PROPVARIANT* p) : m_p(p) {}
    ~PropVariantClearOnExit() {
        PropVariantClear(m_p);
    }
private:
    PROPVARIANT* m_p;
};

//float DbFromAmp(float a);

int _cdecl wmain() {
    ShellExecute(NULL, "Open", "hoot.exe", NULL, NULL, SW_SHOWNORMAL);
    Sleep(2000);
    ShellExecute(NULL, "Open", "hoot.ahk", NULL, NULL, SW_SHOWNORMAL);
    Sleep(1000);

    time_t now = time(0);
    char* dt = ctime(&now);
    *std::remove(dt, dt + strlen(dt), '\n') = '\0'; // removes newline characters from dt
    std::cout << "Program started @ [" << dt << "]\n";
    std::cout << "Searching for hoot process...\n\n";
    for (; ; ) {
        HRESULT hr = S_OK;

        hr = CoInitialize(NULL);
        if (FAILED(hr)) {
            LOG(L"CoInitialize failed: hr = 0x%08x", hr);
            return -__LINE__;
        }
        CoUninitializeOnExit cuoe;

        // get default device
        CComPtr<IMMDeviceEnumerator> pMMDeviceEnumerator;
        hr = pMMDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));
        if (FAILED(hr)) {
            LOG(L"CoCreateInstance(IMMDeviceEnumerator) failed: hr = 0x%08x", hr);
            return -__LINE__;
        }

        EDataFlow flows[] = { eRender, eCapture };
        for (UINT f = 0; f < ARRAYSIZE(flows); f++) {
            if (flows[f] == eRender){}

            CComPtr<IMMDeviceCollection> pMMDeviceCollection;
            hr = pMMDeviceEnumerator->EnumAudioEndpoints(flows[f], DEVICE_STATE_ACTIVE, &pMMDeviceCollection);
            if (FAILED(hr)) {
                LOG(L"IMMDeviceEnumerator::EnumAudioEndpoints failed: hr = 0x%08x", hr);
                continue;
            }

            UINT32 nDevices;
            hr = pMMDeviceCollection->GetCount(&nDevices);
            if (FAILED(hr)) {
                LOG(L"IMMDeviceCollection::GetCount failed: hr = 0x%08x", hr);
                continue;
            }

            for (UINT32 d = 0; d < nDevices; d++) {
                CComPtr<IMMDevice> pMMDevice;
                hr = pMMDeviceCollection->Item(d, &pMMDevice);
                if (FAILED(hr)) {
                    LOG(L"IMMDeviceCollection::Item failed: hr = 0x%08x", hr);
                    continue;
                }

                // get the name of the endpoint
                CComPtr<IPropertyStore> pPropertyStore;
                hr = pMMDevice->OpenPropertyStore(STGM_READ, &pPropertyStore);
                if (FAILED(hr)) {
                    LOG(L"IMMDevice::OpenPropertyStore failed: hr = 0x%08x", hr);
                    continue;
                }

                PROPVARIANT v; PropVariantInit(&v);
                PropVariantClearOnExit pvcoe(&v);
                hr = pPropertyStore->GetValue(PKEY_Device_FriendlyName, &v);
                if (FAILED(hr)) {
                    LOG(L"IPropertyStore::GetValue(PKEY_Device_FriendlyName) failed: hr = 0x%08x", hr);
                    continue;
                }

                if (VT_LPWSTR != v.vt) {
                    LOG(L"PKEY_Device_FriendlyName has unexpected vartype %u", v.vt);
                    continue;
                }

                // get the current audio peak meter level for this endpoint
                CComPtr<IAudioMeterInformation> pAudioMeterInformation_Endpoint;
                hr = pMMDevice->Activate(
                    __uuidof(IAudioMeterInformation),
                    CLSCTX_ALL,
                    NULL,
                    reinterpret_cast<void**>(&pAudioMeterInformation_Endpoint)
                );
                if (FAILED(hr)) {
                    LOG(L"IMMDevice::Activate(IAudioMeterInformation) failed: hr = 0x%08x", hr);
                    continue;
                }

                float peak_endpoint = 0.0f;
                hr = pAudioMeterInformation_Endpoint->GetPeakValue(&peak_endpoint);
                if (FAILED(hr)) {
                    LOG(L"IAudioMeterInformation::GetPeakValue() failed: hr = 0x%08x", hr);
                    continue;
                }

                // get a session enumerator
                CComPtr<IAudioSessionManager2> pAudioSessionManager2;
                hr = pMMDevice->Activate(
                    __uuidof(IAudioSessionManager2),
                    CLSCTX_ALL,
                    nullptr,
                    reinterpret_cast<void**>(&pAudioSessionManager2)
                );
                if (FAILED(hr)) {
                    LOG(L"IMMDevice::Activate(IAudioSessionManager2) failed: hr = 0x%08x", hr);
                    return -__LINE__;
                }

                CComPtr<IAudioSessionEnumerator> pAudioSessionEnumerator;
                hr = pAudioSessionManager2->GetSessionEnumerator(&pAudioSessionEnumerator);
                if (FAILED(hr)) {
                    LOG(L"IAudioSessionManager2::GetSessionEnumerator() failed: hr = 0x%08x", hr);
                    return -__LINE__;
                }

                // iterate over all the sessions
                int count = 0;
                hr = pAudioSessionEnumerator->GetCount(&count);
                if (FAILED(hr)) {
                    LOG(L"IAudioSessionEnumerator::GetCount() failed: hr = 0x%08x", hr);
                    return -__LINE__;
                }

                int active = 0;
                for (int session = 0; session < count; session++) {
                    // get the session identifier
                    CComPtr<IAudioSessionControl> pAudioSessionControl;
                    hr = pAudioSessionEnumerator->GetSession(session, &pAudioSessionControl);
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionEnumerator::GetSession() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    AudioSessionState state;
                    hr = pAudioSessionControl->GetState(&state);
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl::GetState() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }
                    if (AudioSessionStateActive != state) {
                        // skip this session
                        continue;
                    }
                    active++;

                    CComHeapPtr<WCHAR> szIconPath;
                    hr = pAudioSessionControl->GetIconPath(&szIconPath);
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl::GetIconPath() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    CComHeapPtr<WCHAR> szDisplayName;
                    hr = pAudioSessionControl->GetDisplayName(&szDisplayName);
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl::GetDisplayName() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    GUID guidGroupingParam;
                    hr = pAudioSessionControl->GetGroupingParam(&guidGroupingParam);
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl::GetGroupingParam() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    CComPtr<IAudioSessionControl2> pAudioSessionControl2;
                    hr = pAudioSessionControl->QueryInterface(IID_PPV_ARGS(&pAudioSessionControl2));
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl::QueryInterface(IAudioSessionControl2) failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    CComHeapPtr<WCHAR> szSessionIdentifier;
                    hr = pAudioSessionControl2->GetSessionIdentifier(&szSessionIdentifier);
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl2::GetSessionIdentifier() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    CComHeapPtr<WCHAR> szSessionInstanceIdentifier;
                    hr = pAudioSessionControl2->GetSessionInstanceIdentifier(&szSessionInstanceIdentifier);
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl2::GetSessionInstanceIdentifier() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    DWORD pid = 0;
                    hr = pAudioSessionControl2->GetProcessId(&pid);
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl2::GetProcessId() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    hr = pAudioSessionControl2->IsSystemSoundsSession();
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl2::IsSystemSoundsSession() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    // get the current audio peak meter level for this session
                    CComPtr<IAudioMeterInformation> pAudioMeterInformation_Session;
                    hr = pAudioSessionControl->QueryInterface(IID_PPV_ARGS(&pAudioMeterInformation_Session));
                    if (FAILED(hr)) {
                        LOG(L"IAudioSessionControl::QueryInterface(IAudioMeterInformation) failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    float peak_session = 0.0f;
                    hr = pAudioMeterInformation_Session->GetPeakValue(&peak_session);
                    if (FAILED(hr)) {
                        LOG(L"IAudioMeterInformation::GetPeakValue() failed: hr = 0x%08x", hr);
                        return -__LINE__;
                    }

                    // create a pointer to a wchar, and set it equal to the value of trying to find "hoot.exe" in the wchar session identifier variable
                    // if the pointer is null, then it found a match
                    wchar_t* pwc;
                    pwc = wcsstr(szSessionIdentifier, L"hoot.exe");
                    if (pwc != NULL)
                    {
                        hootFound = true;
                        if (hootFound && checkForHoot)
                        {
                            std::cout << "Hoot found! Now listening...\n\n";
                            checkForHoot = false;
                        }
                        //wprintf(L"hoot found!\n");
                        if (peak_session > 0 && timesRepeated < 4)
                        {
                            float currentPeakValue = peak_session;
                            if (currentPeakValue == lastPeakValue)
                            {
                                timesRepeated++; // need this to catch some buggy songs that hang on a note but aren't playing anything
                            }
                            lastPeakValue = peak_session;
                            i = 0;
                            //wprintf(L"hoot has been playing audio recently!\n");
                        }
                        else
                        {
                            i++;
                        }
                        if (i >= 10) // if i has been incremented enough times to make this true, that means no audio has been detected for at least 5 seconds
                        {
                            now = time(0);
                            dt = ctime(&now);
                            *std::remove(dt, dt + strlen(dt), '\n') = '\0';
                            std::cout << "-- Timestamp: [" << dt << "] --\nNo audio from hoot in the last 5 seconds!" << "\nHitting the shuffle button...\n\n";
                            keybd_event(0xC0, 0, KEYEVENTF_EXTENDEDKEY, 0); // presses ` (shuffles hoot)
                            Sleep(100);
                            keybd_event(0xC0, 0, KEYEVENTF_KEYUP, 0); // releases the key press
                            Sleep(100);
                            i = 0;
                            timesRepeated = 0;
                        }
                    }
                }
            }
        }
        Sleep(500); // waits half a second before posting the peak data again
    }
}