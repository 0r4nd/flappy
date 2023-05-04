#ifndef BUFFERCONSOLE_H_
#define BUFFERCONSOLE_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */


extern CHAR_INFO aCHARINFO_LUT[32*32*32];


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/**
 * User wrapped functions and options.
 */
#define bufferConsole_memset(out,val,sz) memset(out,val,sz)
#define bufferConsole_memcpy(out,in,sz) memcpy(out,in,sz)
#define bufferConsole_realloc(base,size) realloc(base, size)


/******************************************************************************/

/**
 * Flags options
 */
#define BUFFERCONSOLE_BILINEAR  (1<<0)


/******************************************************************************/

/**
 * structures
 */
typedef struct bufferConsole_s {
  CHAR_INFO *front,*back,*cur; /* CHAR_INFO buffers */
  uint32_t *pixels; /* rgb888 buffer */
  COORD size,coord;
  SMALL_RECT region;
  HANDLE hnd;
} bufferConsole_t;


/******************************************************************************/

/**
 * prototypes
 */


int bufferConsole_new(bufferConsole_t *const self, HANDLE hnd,
                      short width, short height);
void bufferConsole_delete(bufferConsole_t *const self);


void bufferConsole_fillFlatQuad(bufferConsole_t *const self,
                                uint32_t color, int opacity,
                                int x1, int y1, int x2, int y2);
void bufferConsole_fillFlatSubQuad(bufferConsole_t *const self,
                                   int color_sub, int factor,
                                   int x1, int y1, int x2, int y2);

void bufferConsole_fillTexQuad(bufferConsole_t *const self, uint32_t *color, int depth,
                               int x, int y, int width, int height);
void bufferConsole_fillScaleTexQuad(bufferConsole_t *const self, uint32_t *psrc,
                                    int decay_cumul, float *ydecay, int opacity,
                                    int x1, int y1, int u1, int v1,
                                    int x2, int y2, int u2, int v2,
                                    uint32_t srcwidth, uint32_t srcheight);
void bufferConsole_fillScaleTexSubQuad(bufferConsole_t *const self, uint32_t *psrc,
                                       int decay_cumul, float *ydecay,
                                       int color_sub, int factor,
                                       int x1, int y1, int u1, int v1,
                                       int x2, int y2, int u2, int v2,
                                       uint32_t srcwidth, uint32_t srcheight);


void bufferConsole_fillScaleBilinTexQuad(bufferConsole_t *const self, uint32_t *psrc,
                                         int decay_cumul, float *ydecay, uint32_t opacity,
                                         int x1, int y1, int u1, int v1,
                                         int x2, int y2, int u2, int v2,
                                         uint32_t srcwidth, uint32_t srcheight);


void bufferConsole_drawPixel(bufferConsole_t *const self,
                             uint32_t col, int x, int y, int opacity);
void bufferConsole_drawWuPixel(bufferConsole_t *const self,
                               uint32_t col, int x, int y, int opacity);
void bufferConsole_drawCircle(bufferConsole_t *const self,
                              uint32_t col, int opacity, int x0, int y0, int radius);



void bufferConsole_fillTexArrQuad(bufferConsole_t *const self, uint8_t *color, int depth,
                                  int x, int y, int width, int height);
void bufferConsole_fillLine(bufferConsole_t *const self, char tex, uint8_t col,
                            int x, int y, int x2, int y2);




void bufferConsole_swap(bufferConsole_t *const self);
void bufferConsole_update(bufferConsole_t *const self);


void charinfo_genPixTable(CHAR_INFO out[32*32*32]);


#ifdef __cplusplus
}
#endif


#endif /* BUFFERCONSOLE_H_ */


