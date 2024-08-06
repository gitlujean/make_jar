#ifndef  __watermarkAPI_h__
#define  __watermarkAPI_h__

#ifdef __cplusplus
extern "C" 
	{
#endif
	short AddWatermark256(unsigned char* imageBuffer, const char* watermark, int strength);
	short DetectWatermark256(unsigned char* imageBuffer, char* watermark);
#ifdef __cplusplus
	}
#endif

#endif
