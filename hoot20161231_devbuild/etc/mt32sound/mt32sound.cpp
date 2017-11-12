//#define DEBUG_WITH_MIDI
//#define DEBUG_OUTPUT_LOG
//#define DEBUG_OUTPUT_MT32EMU_LOG

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef DEBUG_WITH_MIDI
#include <mmsystem.h>
#endif /* DEBUG_WITH_MIDI */

#include "mt32sound.h"
#include "munt/mt32emu/include/mt32emu/mt32emu.h"


#define MIDI_BUFF_SIZE (4096)

#define MIDI_STATE_READY		0
#define MIDI_STATE_DATA2		1
#define MIDI_STATE_DATA3		2
#define MIDI_STATE_EXCLUSIVE	3
#define MIDI_STATE_TIMECODE		4
#define MIDI_STATE_SYSTEMDATA1	5
#define MIDI_STATE_SYSTEMDATA2	6
#define MIDI_STATE_SYSTEMDATA3	7


static const MT32Emu::ROMImage *controlROMImage;
static const MT32Emu::ROMImage *pcmROMImage;
static MT32Emu::Synth* MT32 = NULL;

static TCHAR moduledir[MAX_PATH];
static int midi_state;
static unsigned char midi_buff[MIDI_BUFF_SIZE];
static int midi_buff_ptr;
static unsigned char midi_last_data;

#ifdef DEBUG_WITH_MIDI
static HMIDIOUT hMidi;
#endif /* DEBUG_WITH_MIDI */


#define INTERP_SHIFT	(14)
#define INTERP_SIZE		(1 << INTERP_SHIFT)

static short interp_prev_sample[2];
static short interp_cur_sample[2];
static unsigned int interp_pos;
static unsigned int interp_ratio;

static inline
unsigned long
midi_data(unsigned char stat, unsigned char second, unsigned char third)
{
	unsigned long ret = 0;

	ret = third;	ret <<= 8;
	ret |= second;	ret <<= 8;
	ret |= stat;

	return ret;
}


static void
CutFileName(TCHAR *path)
{
	TCHAR	*tail = NULL;

	while(*path)
	{
		TCHAR *next = ::CharNextExA(CP_OEMCP, path, 0);
		if (*path == '\\')
		{
			tail = next;
		}
		path = next;
	}

	if (tail != NULL)
	{
		*tail = '\0';
	}
}


#if 0
static int
report(void *userData, MT32Emu::ReportType type, const void *reportData)
{
	switch (type)
	{
	case MT32Emu::ReportType_lcdMessage:
		::OutputDebugString("[");
		::OutputDebugString((const char*)reportData);
		::OutputDebugString("]\r\n");
		break;
	default:
		break;
	}

	return 0;
}


static void
printDebug(void *userData, const char *fmt, va_list list)
{
#ifdef DEBUG_OUTPUT_MT32EMU_LOG
	static char buff[1024];
	::vsnprintf(buff, sizeof(buff), fmt, list);
	::OutputDebugString(buff);
	::OutputDebugString("\r\n");
#endif /* DEBUG_OUTPUT_MT32EMU_LOG */
}
#endif

int WINAPI
MT32Sound_Open(int sample_rate,
			   int use_reverb, int use_default,
			   int rev_type, int rev_time, int rev_level)
{
	int ret = -1;
	MT32Emu::FileStream controlROMFile;
	MT32Emu::FileStream pcmROMFile;

	if (MT32 != NULL) return ret;

	::ZeroMemory(midi_buff, sizeof(midi_buff));
	midi_buff_ptr = 0;
	midi_last_data = 0;

	::ZeroMemory(interp_prev_sample, sizeof(interp_prev_sample));
	::ZeroMemory(interp_cur_sample, sizeof(interp_cur_sample));
	interp_pos = INTERP_SIZE;
	interp_ratio = MT32Emu::SAMPLE_RATE * INTERP_SIZE / sample_rate;

	MT32 = new MT32Emu::Synth();
	if (MT32 == NULL) return ret;


	if (!controlROMFile.open("CM32L_CONTROL.ROM"))
	{
		if (!controlROMFile.open("MT32_CONTROL.ROM"))
		{
#ifdef DEBUG_OUTPUT_LOG
			::OutputDebugString("cannot load CONTROL ROM\r\n");
#endif
			return ret;
		}
	}
	if (!pcmROMFile.open("CM32L_PCM.ROM")) {
		if (!pcmROMFile.open("MT32_PCM.ROM")) {
#ifdef DEBUG_OUTPUT_LOG
			::OutputDebugString("cannot load PCM ROM\r\n");
#endif
			return ret;
		}
	}
	controlROMImage = MT32Emu::ROMImage::makeROMImage(&controlROMFile);
	pcmROMImage = MT32Emu::ROMImage::makeROMImage(&pcmROMFile);

#if 0
	MT32Emu::SynthProperties prop;
	prop.sampleRate = sample_rate;
	prop.useReverb = (use_reverb == 1);
	prop.useDefaultReverb = (use_default == 1);

	prop.useDefaultReverb = true;
	prop.reverbType = rev_type;
	prop.reverbTime = rev_time;
	prop.reverbLevel = rev_level;

	prop.baseDir = moduledir;
	prop.userData = NULL;
	prop.report = ::report;
	prop.printDebug = ::printDebug;
	prop.openFile = NULL;
	prop.closeFile = NULL;
#endif

	if (MT32->open(*controlROMImage, *pcmROMImage))
	{
		midi_state = MIDI_STATE_READY;
		midi_buff_ptr = 0;
#ifdef DEBUG_OUTPUT_LOG
		::OutputDebugString("ready MT-32\r\n");
#endif
		ret = 0;
	}
	else
	{
#ifdef DEBUG_OUTPUT_LOG
		::OutputDebugString("cannot open MT-32\r\n");
#endif
	}

#ifdef DEBUG_WITH_MIDI
	if (::midiOutOpen(&hMidi, MIDI_MAPPER, 0, 0, CALLBACK_NULL) == MMSYSERR_NOERROR)
	{
		::midiOutReset(hMidi);
	}
#endif /* DEBUG_WITH_MIDI */

	return ret;
}


int WINAPI
MT32Sound_Close(void)
{
	int ret = -1;

	if (MT32 == NULL) return ret;

	MT32->close();

	delete MT32;
	MT32 = NULL;

	MT32Emu::ROMImage::freeROMImage(controlROMImage);
	MT32Emu::ROMImage::freeROMImage(pcmROMImage);
	controlROMImage = NULL;
	pcmROMImage = NULL;

	ret = 0;

#ifdef DEBUG_WITH_MIDI
	::midiOutReset(hMidi);
	::midiOutClose(hMidi);
#endif /* DEBUG_WITH_MIDI */

	return ret;
}


int WINAPI
MT32Sound_Write(unsigned char data)
{
	int ret = -1;

	if (MT32 == NULL) return ret;

#ifdef DEBUG_OUTPUT_LOG
	{
		static unsigned char buff[16];
		static int ct = 0;
		buff[ct++] = data;
		if (ct == 16)
		{
			static const char *hex = "0123456789abcdef";
			static char s[3 * 16 + 1];
			int i;
			for (i = 0; i < 16; i++)
			{
				unsigned char d = buff[i];
				s[i*3+0] = hex[(d >> 4) & 0x0f];
				s[i*3+1] = hex[(d >> 0) & 0x0f];
				s[i*3+2] = ' ';
			}
			s[15*3+2] = '\n';
			s[15*3+3] = '\0';
			::OutputDebugString(s);
			ct = 0;
		}
	}
#endif /* DEBUG_OUTPUT_LOG */

	switch (data)
	{
	case 0xf8: // TIMING
	case 0xfa: // START
	case 0xfb: // CONTINUE
	case 0xfc: // STOP
	case 0xfe: // ACTIVE SENSING
	case 0xff: // SYSTEM RESET
		ret = 0;
		return ret;
	}

	if ((midi_state == MIDI_STATE_READY)
		|| ((midi_state == MIDI_STATE_DATA2) && (data & 0x80))
		|| ((midi_state == MIDI_STATE_DATA3) && (data & 0x80)))
	{
		midi_buff_ptr = 0;
		if (data & 0x80)
		{
			midi_last_data = data;
			switch (data & 0xf0)
			{
			case 0xc0: // PROGRAM CHANGE
			case 0xd0: // AFTER TOUCH
				midi_state = MIDI_STATE_DATA2;
				break;
			case 0x80: // NOTE OFF
			case 0x90: // NOTE ON
			case 0xa0: // POLYPHONIC KEY PRESSURE
			case 0xb0: // CONTROL CHANGE
			case 0xe0: // PITCH BENDER
				midi_state = MIDI_STATE_DATA3;
				break;
			default:
				switch (data)
				{
				case 0xf0: // EXCLUSIVE
					midi_state = MIDI_STATE_EXCLUSIVE;
					break;
				case 0xf1: // TIME CODE
					midi_state = MIDI_STATE_TIMECODE;
					break;
				case 0xf2: // SONG POS
					midi_state = MIDI_STATE_SYSTEMDATA3;
					break;
				case 0xf3: // SONG SELECT
					midi_state = MIDI_STATE_SYSTEMDATA2;
					break;
				case 0xf6: // TUNE REQUEST
					midi_state = MIDI_STATE_SYSTEMDATA1;
					break;
				default:
					ret = 0;
					return ret;
				}
				break;
			}
		}
		else
		{
			midi_buff[midi_buff_ptr++] = midi_last_data;
			midi_state = MIDI_STATE_DATA3;
		}
	}

	midi_buff[midi_buff_ptr++] = data;

	switch (midi_state)
	{
	case MIDI_STATE_DATA2:
		if (midi_buff_ptr >= 2)
		{
			MT32->playMsg(midi_data(midi_buff[0], midi_buff[1], 0));
#ifdef DEBUG_WITH_MIDI
			::midiOutShortMsg(hMidi, midi_data(midi_buff[0], midi_buff[1], 0));
#endif /* DEBUG_WITH_MIDI */
			midi_state = MIDI_STATE_READY;
		}
		break;
	case MIDI_STATE_DATA3:
		if (midi_buff_ptr >= 3)
		{
			MT32->playMsg(midi_data(midi_buff[0], midi_buff[1], midi_buff[2]));
#ifdef DEBUG_WITH_MIDI
			::midiOutShortMsg(hMidi, midi_data(midi_buff[0], midi_buff[1], midi_buff[2]));
#endif /* DEBUG_WITH_MIDI */
			midi_state = MIDI_STATE_READY;
		}
		break;
	case MIDI_STATE_EXCLUSIVE:
		if (data == 0xf7)
		{
#ifdef DEBUG_OUTPUT_LOG
			{
				static char s[32];
				::wsprintf(s, "Exclusive(size): %d\n", midi_buff_ptr);
				::OutputDebugString(s);
				int addr = (midi_buff[5]<<16) + (midi_buff[6]<<8) + (midi_buff[7]);
				::wsprintf(s, "Exclusive(addr): %08x\n", addr);
				::OutputDebugString(s);
			}
#endif /* DEBUG_OUTPUT_LOG */
			MT32->playSysex(midi_buff, midi_buff_ptr);
			midi_state = MIDI_STATE_READY;
		}
		else if (midi_buff_ptr >= MIDI_BUFF_SIZE)
		{
#ifdef DEBUG_OUTPUT_LOG
			::OutputDebugString("Exclusive overflow\n");
#endif /* DEBUG_OUTPUT_LOG */
			midi_state = MIDI_STATE_READY;
		}
		break;
	case MIDI_STATE_SYSTEMDATA1:
		if (midi_buff_ptr >= 1)
		{
			midi_state = MIDI_STATE_READY;
		}
		break;
	case MIDI_STATE_SYSTEMDATA2:
		if (midi_buff_ptr >= 2)
		{
			midi_state = MIDI_STATE_READY;
		}
		break;
	case MIDI_STATE_SYSTEMDATA3:
		if (midi_buff_ptr >= 3)
		{
			midi_state = MIDI_STATE_READY;
		}
		break;
	}

	ret = 0;

	return ret;
}


int WINAPI
MT32Sound_ShortMsg(unsigned long data)
{
	int ret = -1;

	if (MT32 == NULL) return ret;

	MT32->playMsg(data);
#ifdef DEBUG_WITH_MIDI
	::midiOutShortMsg(hMidi, data);
#endif /* DEBUG_WITH_MIDI */

	ret = 0;

	return ret;
}


int WINAPI
MT32Sound_LongMsg(const unsigned char *sysex, unsigned long len)
{
	int ret = -1;

	if (MT32 == NULL) return ret;

	MT32->playSysex(sysex, len);

	ret = 0;

	return ret;
}


int WINAPI
MT32Sound_Mix(short *buff, unsigned long samples)
{
	int ret = -1;

	if (MT32 == NULL) return ret;

	while (samples--)
	{
		while (interp_pos >= INTERP_SIZE)
		{
			interp_prev_sample[0] = interp_cur_sample[0];
			interp_prev_sample[1] = interp_cur_sample[1];
			MT32->render((MT32Emu::Bit16s*)interp_cur_sample, 1);
			interp_pos -= INTERP_SIZE;
		}
		interp_pos += interp_ratio;

		*buff++ = ((int)interp_prev_sample[0] * (INTERP_SIZE - interp_pos)
				   + (int)interp_cur_sample[0] * (interp_pos)) / INTERP_SIZE;
		*buff++ = ((int)interp_prev_sample[1] * (INTERP_SIZE - interp_pos)
				   + (int)interp_cur_sample[1] * (interp_pos)) / INTERP_SIZE;
	}

	ret = 0;

	return ret;
}


extern "C"
BOOL APIENTRY
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		::GetModuleFileName((HMODULE)hinstDLL, moduledir, sizeof(moduledir)/sizeof(TCHAR));
		::CutFileName(moduledir);
#ifdef DEBUG_OUTPUT_LOG
		{
			static char buff[1024];
			::OutputDebugString("mt32sound.dll attached\n");
			::snprintf(buff, sizeof(buff), "Directory: %s\n", moduledir);
			::OutputDebugString(buff);
		}
#endif /* DEBUG_OUTPUT_LOG */
		break;

	case DLL_PROCESS_DETACH:
		::MT32Sound_Close();
#ifdef DEBUG_OUTPUT_LOG
		{
			::OutputDebugString("mt32sound.dll detached\n");
		}
#endif /* DEBUG_OUTPUT_LOG */
		break;
	}

	return TRUE;
}
