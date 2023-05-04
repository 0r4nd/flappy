#ifndef COLORUTIL_H_
#define COLORUTIL_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */



#define RGBA32(r,g,b,a) ( (r) | (g)<<8 | (b)<<16 | (a)<<24 )
#define RGBA16(r,g,b,a) ( (r) | (g)<<4 | (b)<<8  | (a)<<12 )


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/**
 * prototypes
 */
uint32_t rgba32_css(const char *name);
uint32_t rgba16_css(const char *name);
int32_t rgba32_lerp(int32_t col1, int32_t col2, int32_t t);
int32_t rgba16_lerp(int32_t col1, int32_t col2, int32_t t);

uint32_t rgb555_clamp(int r, int g, int b);
uint32_t rgb565_clamp(int r, int g, int b);
uint32_t rgba16_clamp(int r, int g, int b, int a);
uint32_t rgb888_clamp(int r, int g, int b);
uint32_t rgba32_clamp(int r, int g, int b, int a);


float color_diffLAB(uint32_t color1, uint32_t color2);
float color_diffXYZ(uint32_t color1, uint32_t color2);

int32_t color_lerpYUV(int32_t col1, int32_t col2, float t);
int32_t color_lerpRGB(int32_t col1, int32_t col2, int32_t t);
int32_t color_lerpRGBFl(int32_t col1, int32_t col2, float t);

uint32_t color_RGB555toRGBA32(uint32_t col, uint32_t a);
uint32_t color_RGB888toRGB555(uint32_t col);
uint32_t color_RGBA32toRGBA16(uint32_t col);
CHAR_INFO charinfo_fromRGB555(uint32_t col);
CHAR_INFO charinfo_fromRGB888(uint32_t col);

int color_toCHARINFO(uint32_t color_base, int8_t *out_ascii, uint8_t *out_color);

#ifdef __cplusplus
}
#endif


#endif /* COLORUTIL_H_ */


