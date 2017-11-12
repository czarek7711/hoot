#ifndef __MT32SOUND_H__
#define __MT32SOUND_H__


#define MT32_REVERB_TYPE_ROOM    1
#define MT32_REVERB_TYPE_HALL    2
#define MT32_REVERB_TYPE_PLATE   3
#define MT32_REVERB_TYPE_TAP     4

#define MT32_REVERB_TIME_MIN     0
#define MT32_REVERB_TIME_MAX     7

#define MT32_REVERB_LEVEL_MIN    0
#define MT32_REVERB_LEVEL_MAX    7


#ifdef __cplusplus
extern "C"
{
#endif


int WINAPI
MT32Sound_Open(int sample_rate,
			   int use_reverb, int use_default,
			   int rev_type, int rev_time, int rev_level);
int 
WINAPI
MT32Sound_Close(void);

int WINAPI
MT32Sound_Write(unsigned char data);

int WINAPI
MT32Sound_ShortMsg(unsigned long data);

int WINAPI
MT32Sound_LongMsg(const unsigned char *sysex, unsigned long len);

int WINAPI
MT32Sound_Mix(short *buff, unsigned long samples);


#ifdef __cplusplus
}
#endif

#endif /* __MT32SOUND_H__ */
