// thanks mvaneerde for the WASAPI code https://github.com/mvaneerde/blog/tree/develop/meters/meters (I trimmed down a lot - there might be more to trim but I'm no good at WASAPI)
// the rest developed by Shadowdevo
#include <atlbase.h>
#include <audiopolicy.h>
#include <chrono>
#include <ctime>
#include <endpointvolume.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mmdeviceapi.h>
#include <string>
#include <windows.h>

using namespace std;
using namespace filesystem;

int i = 0;
int timesRepeated = 0;
float peak_session = 0.0f;
float lastPeakValue = 0.0f;
bool hootFound = false;
bool checkForHoot = true;
bool done = false;
time_t now;
char key;
char confirmation;
BYTE hotkeyCode;
char* dateTime;
string settings = "hoot shuffle.settings";

char* getTime()
{
    now = time(0);
    dateTime = ctime(&now);
    *std::remove(dateTime, dateTime + strlen(dateTime), '\n') = '\0'; // formatting for time stamp to be displayed correctly (removes a strange newline character that's added from the previous line)
    return dateTime;
}

// checks to see if the character given is not a letter or number (e.g. ( ` ! @ # $ % etc) 
// so we can convert ones that require shift to the non shift version (e.g. if the input is ~ then it's converted to ` or ! to 1 etc)
// if the character is A-Z, converts to lowercase
void convertLower(char c)
{
    switch (c)
    {
    case '~':
        key = '`';
        break;
    case '!':
        key = '1';
        break;
    case '@':
        key = '2';
        break;
    case '#':
        key = '3';
        break;
    case '$':
        key = '4';
        break;
    case '%':
        key = '5';
        break;
    case '^':
        key = '6';
        break;
    case '&':
        key = '7';
        break;
    case '*':
        key = '8';
        break;
    case '(':
        key = '9';
        break;
    case ')':
        key = '0';
        break;
    case '_':
        key = '-';
        break;
    case '+':
        key = '=';
        break;
    case '{':
        key = '[';
        break;
    case '}':
        key = ']';
        break;
    case '|':
        key = '\\';
        break;
    case ':':
        key = ';';
        break;
    case '"':
        key = '\'';
        break;
    case '<':
        key = ',';
        break;
    case '>':
        key = '.';
        break;
    case '?':
        key = '/';
        break;
    default:
        key = static_cast<char>(tolower(c));
        break;
    }
}

void createSettings()
{
    ofstream CreateSettings(settings);
    while (!done)
    {
        cout << "\nPlease enter your desired hotkey for shuffling: ";
        cin >> key;
        while (!cin.eof() && cin.get() != '\n'); // clears the cin buffer in case user inputs multiple characters - this prevents confirmation from getting pounded by the buffer
        convertLower(key);
        cout << "Your chose: " << key << " is this correct? y/n: ";
        cin >> confirmation;
        switch (confirmation)
        {
        case 'Y':
        case 'y':
            hotkeyCode = (BYTE)VkKeyScanExA(key, GetKeyboardLayout(0)); // sets the hotkey value based on what the user inputted
            CreateSettings << key; // puts the user defined hotkey into the settings file
            done = true;
            break;
        case 'N':
        case 'n':
            cout << "\n[Hotkey not saved. Please choose a different hotkey!]\n";
            break;
        default:
            cout << "\n[Hotkey not saved. Please choose a different hotkey!]\n";
            break;
        }
    }
    CreateSettings.close();
}

void getSettings()
{
    ifstream ReadSettings(settings);
    char character;
    int count = 0;
    while (ReadSettings >> character) ++count; // count the amount of characters inside the settings file
    ReadSettings.clear();
    ReadSettings.seekg(0, ios::beg); // reset the buffer for later use

    if (count != 1)
    {
        // if there's more than 1 character prompt the user for a new hotkey
        cout << "\n[Error reading settings file!] Must be 1 character - Number of characters found: " << count << endl;
        createSettings();
    }
    else
    {   
        // get key from file
        while (ReadSettings >> key); // gets the character in the settings file - written this way to catch whitespaces in the settings file if they exist for some reason
        hotkeyCode = (BYTE)VkKeyScanExA(key, GetKeyboardLayout(0));
    }
    ReadSettings.close();
}

// creates the hoot shuffle.ahk script using the key pulled from hoot shuffle.settings
void createScript()
{
    ofstream HootAHK("hoot shuffle.ahk");
    string str = "process, exist, hoot shuffle.exe ; prevents script from running unless hoot shuffle is running\n\
if !errorlevel\n\
    ExitApp\n\
process, exist, hoot.exe ; closes hoot shuffle if hoot was closed\n\
if !errorlevel\n\
    Process, Close, hoot shuffle.exe\n\
ControlSend, , {Esc 2}, ahk_exe hoot.exe ; this stuff is for resetting back to the beginning when we use the manual shuffle button\n\
Sleep 100\n\
ControlSend, , {Up}, ahk_exe hoot.exe\n\
Sleep 100\n\
ControlSend, , {Enter}, ahk_exe hoot.exe\n\
Sleep 100\n\
PlayRandomSong()\n\
\n\
PlayRandomSong()\n\
{\n\
    Loop\n\
    {\n\
        process, exist, hoot shuffle.exe ; closes the script if hoot shuffle gets closed\n\
        if !errorlevel\n\
            ExitApp\n\
        Random, timesToPD, 0, 30 ; how many times we page down on the games directory\n\
        Random, timesToArrowKey, 0, 12 ; how many times we press the arrow key after paging down\n\
        ControlSend,, {PgDn %timesToPD%}, ahk_exe hoot.exe\n\
        ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe\n\
        ControlSend,, {Enter}, ahk_exe hoot.exe ; enter this randomly chosen game directory\n\
        \n\
        Random, downOrUp, 0, 1 ; down = 0 up = 1\n\
        if (downOrUp = 0)\n\
        {\n\
            Random, timesToArrowKey, 0, 5\n\
            ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe\n\
            ControlSend,, {Enter}, ahk_exe hoot.exe ; play the selected track\n\
            Random, continueArrowKey, 0, 1 ; reason for continueArrowKey is to get better variety when shuffling tracks, previously it was choosing the firstand last tracks too often\n\
            if (continueArrowKey = 1)\n\
            {\n\
                Random, timesToArrowKey, 0, 10\n\
                ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe\n\
                ControlSend,, {Enter}, ahk_exe hoot.exe\n\
                Random, continueArrowKey, 0, 1\n\
                if (continueArrowKey = 1)\n\
                {\n\
                    Random, timesToArrowKey, 0, 20\n\
                    ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe\n\
                    ControlSend,, {Enter}, ahk_exe hoot.exe\n\
                    Random, continueArrowKey, 0, 1\n\
                    if (continueArrowKey = 1)\n\
                    {\n\
                        Random, timesToArrowKey, 0, 30\n\
                        ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe\n\
                        ControlSend,, {Enter}, ahk_exe hoot.exe\n\
                        Random, continueArrowKey, 0, 1\n\
                        if (continueArrowKey = 1)\n\
                        {\n\
                            Random, timesToArrowKey, 0, 40\n\
                            ControlSend,, {Down %timesToArrowKey%}, ahk_exe hoot.exe\n\
                            ControlSend,, {Enter}, ahk_exe hoot.exe\n\
                        }\n\
                    }\n\
                }\n\
            }\n\
        }\n\
        else\n\
        {\n\
            Random, timesToArrowKey, 0, 5\n\
            ControlSend,, {End}, ahk_exe hoot.exe ; go to the bottom before going up\n\
            ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe\n\
            ControlSend,, {Enter}, ahk_exe hoot.exe ; play the selected track\n\
            Random, continueArrowKey, 0, 1\n\
            if (continueArrowKey = 1)\n\
            {\n\
                Random, timesToArrowKey, 0, 10\n\
                ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe\n\
                ControlSend,, {Enter}, ahk_exe hoot.exe\n\
                Random, continueArrowKey, 0, 1\n\
                if (continueArrowKey = 1)\n\
                {\n\
                    Random, timesToArrowKey, 0, 20\n\
                    ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe\n\
                    ControlSend,, {Enter}, ahk_exe hoot.exe\n\
                    Random, continueArrowKey, 0, 1\n\
                    if (continueArrowKey = 1)\n\
                    {\n\
                        Random, timesToArrowKey, 0, 30\n\
                        ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe\n\
                        ControlSend,, {Enter}, ahk_exe hoot.exe\n\
                        Random, continueArrowKey, 0, 1\n\
                        if (continueArrowKey = 1)\n\
                        {\n\
                            Random, timesToArrowKey, 0, 40\n\
                            ControlSend,, {Up %timesToArrowKey%}, ahk_exe hoot.exe\n\
                            ControlSend,, {Enter}, ahk_exe hoot.exe\n\
                        }\n\
                    }\n\
                }\n\
            }\n\
        }\n\
        \n\
        Sleep 1000 ; need to wait a second after playing otherwise it will stop the track\n\
        ControlSend,, {Esc}, ahk_exe hoot.exe ; get back to the game directory\n\
        ControlSend,, {Home}, ahk_exe hoot.exe ; go to the top of the game directory\n\
        Sleep 180000 ; let the track play for 3 minutes(180 seconds)\n\
    }\n\
}\n\
\n\
ForceRandomSong()\n\
{\n\
    Reload\n\
}\n\n";
    str += key;
    str.append("::ForceRandomSong() ; rebinds key defined in hoot shuffle.settings to call the ForceRandomSong function when pressed (functions as a manual shuffle button");
    HootAHK << str;
    HootAHK.close();
}

int main()
{
    cout << "Program started @ [" << getTime() << "]\n\n";
    if (!exists(settings))
    {
        cout << "[Settings not detected!]\n";
        createSettings();
        cout << "\nHotkey settings created and saved successfully. Your hotkey is: " << key << "\nIf you wish to change your hotkey, please delete the file \"hoot shuffle.settings\" and restart the program.\n";
    }
    else
    {
        cout << "Loading settings from \"hoot shuffle.settings\"\n";
        getSettings();
        convertLower(key);
        cout << "\nHotkey settings loaded successfully. Your hotkey is: " << key << "\nIf you wish to change your hotkey, please delete the file \"hoot shuffle.settings\" and restart the program.\n";
    }
    createScript(); // make sure that the script exists/is updated with the most up to date hotkey definition

    cout << "\nOpening hoot and AHK script...\n";
    ShellExecute(NULL, "Open", "hoot.exe", NULL, NULL, SW_SHOWNORMAL);
    Sleep(2000);
    ShellExecute(NULL, "Open", "hoot shuffle.ahk", NULL, NULL, SW_SHOWNORMAL);
    Sleep(1000);
    cout << "Done!\n\n";
    cout << "Searching for hoot volume mixer process...\n\n";

    HRESULT hr = S_OK;
    hr = CoInitialize(NULL);
    // get default device
    CComPtr<IMMDeviceEnumerator> pMMDeviceEnumerator;
    hr = pMMDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));
    EDataFlow flows[] = { eRender, eCapture };

    for (;;)
    {
        for (UINT f = 0; f < ARRAYSIZE(flows); f++) 
        {
            CComPtr<IMMDeviceCollection> pMMDeviceCollection;
            hr = pMMDeviceEnumerator->EnumAudioEndpoints(flows[f], DEVICE_STATE_ACTIVE, &pMMDeviceCollection);
            if (FAILED(hr)) 
            {
                continue;
            }

            UINT32 nDevices;
            hr = pMMDeviceCollection->GetCount(&nDevices);
            if (FAILED(hr)) 
            {
                continue;
            }

            for (UINT32 d = 0; d < nDevices; d++) 
            {
                CComPtr<IMMDevice> pMMDevice;
                hr = pMMDeviceCollection->Item(d, &pMMDevice);
                if (FAILED(hr)) 
                {
                    continue;
                }

                // get a session enumerator
                CComPtr<IAudioSessionManager2> pAudioSessionManager2;
                CComPtr<IAudioSessionEnumerator> pAudioSessionEnumerator;
                hr = pMMDevice->Activate(
                    __uuidof(IAudioSessionManager2),
                    CLSCTX_ALL,
                    nullptr,
                    reinterpret_cast<void**>(&pAudioSessionManager2)
                );
                hr = pAudioSessionManager2->GetSessionEnumerator(&pAudioSessionEnumerator);

                // get the number of sessions that exist
                int count = 0;
                hr = pAudioSessionEnumerator->GetCount(&count);

                int active = 0;
                for (int session = 0; session < count; session++) 
                {
                    // get the session identifier
                    CComPtr<IAudioSessionControl> pAudioSessionControl;
                    hr = pAudioSessionEnumerator->GetSession(session, &pAudioSessionControl);

                    // get the state
                    AudioSessionState state;
                    hr = pAudioSessionControl->GetState(&state);
                    if (AudioSessionStateActive != state) 
                    {
                        // skip this session if it's in an active state
                        continue;
                    }
                    active++;

                    // get the interface
                    CComPtr<IAudioSessionControl2> pAudioSessionControl2;
                    hr = pAudioSessionControl->QueryInterface(IID_PPV_ARGS(&pAudioSessionControl2));

                    // get the session identifier which contains information about what is making sound
                    CComHeapPtr<WCHAR> szSessionIdentifier;
                    hr = pAudioSessionControl2->GetSessionIdentifier(&szSessionIdentifier);
                    
                    // get the current audio peak meter level for this session
                    CComPtr<IAudioMeterInformation> pAudioMeterInformation_Session;
                    hr = pAudioSessionControl->QueryInterface(IID_PPV_ARGS(&pAudioMeterInformation_Session));
                    hr = pAudioMeterInformation_Session->GetPeakValue(&peak_session);

                    // create a pointer to a wchar, and set it equal to the value of trying to find "hoot.exe" in the wchar session identifier variable
                    // if the pointer is not null, then it found a match
                    wchar_t* pwc;
                    pwc = wcsstr(szSessionIdentifier, L"hoot.exe");
                    if (pwc != NULL)
                    {
                        hootFound = true;
                        if (hootFound && checkForHoot)
                        {
                            cout << "-- [Hoot found!] Now listening... --\n\n";
                            checkForHoot = false;
                        }
                        if (peak_session > 0 && timesRepeated < 4)
                        {
                            float currentPeakValue = peak_session;
                            if (currentPeakValue == lastPeakValue)
                            {
                                timesRepeated++; // need this to catch some buggy songs that hang on a note but aren't playing anything
                            }
                            lastPeakValue = peak_session;
                            i = 0;
                        }
                        else
                        {
                            i++;
                        }
                        if (i >= 8) // if i has been incremented enough times to make this true, that means no audio has been detected for about 4 seconds (every time i is incremented 2 times, a little over a second has passed)
                        {
                            // ouputs a timestamp for the operation
                            cout << "-- Timestamp: [" << getTime() << "] --\nNo audio from hoot in the last 5 seconds!" << "\nHitting the shuffle button...\n\n";

                            // press the shuffle button
                            keybd_event(hotkeyCode, 0, KEYEVENTF_EXTENDEDKEY, 0); // presses ` (shuffles hoot)
                            Sleep(100);
                            keybd_event(hotkeyCode, 0, KEYEVENTF_KEYUP, 0); // releases the key press
                            Sleep(100);
                            // shuffle button has been pressed so reset these variables to begin listening for no audio again
                            i = 0;
                            timesRepeated = 0;
                        }
                    }
                }
            }
        }
        Sleep(500); // half second wait between updates
    }
    exit(EXIT_SUCCESS);
}