#ifndef SPRITE_H_
#define SPRITE_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */

#include "math/trig/vec2.h"
#include "keypad.h"
#include "bufferConsole.h"
#include "file/img/img.h"


#ifdef __cplusplus
extern "C" {
#endif


/* uncomment for debug */
//#define SPRITECONSOLE_DEBUG


/******************************************************************************/

/**
 * User wrapped functions and options.
 */
#define sprite_memset(out,val,sz) memset(out,val,sz)


/******************************************************************************/

/**
 * structs
 */
typedef struct col_s {
  uint32_t flags;
  vec2 pos;   /* pos */
  vec2 vel;   /* velocity */
  vec2 size;  /* size */
  float mass; /* mass */
} col_t;


typedef struct sprite_s {
  uint32_t flags;
  keypad_t *keypad_hook;
  img_t *image;

  float *ydecay_ratio; /* y array decay (on texture) */
  float decay_speed;
  uint32_t decay_cumul; /* fixed .16 */

  vec2 pos;
  vec2 size;
  col_t col;

  float scale;
  float gravity;
  float wind;

  float xframe, yframe;
  float xframe_speed, yframe_speed;
  float xframe_count, yframe_count;

  int opacity; /* 0-255 */
  char active;
} sprite_t;



/******************************************************************************/

/**
 * prototypes
 */
sprite_t sprite_new(img_t *image,
                                  vec2 pos, vec2 size, uint32_t flags);
void sprite_delete(sprite_t *const self);

sprite_t sprite_randInBuffer(char tex, bufferConsole_t *buf);
int sprite_update(sprite_t *const self, bufferConsole_t *out,
                         float dt);
void sprite_fill(sprite_t *const self, bufferConsole_t *out);

int sprite_collide(sprite_t *const self, sprite_t *other);
void sprite_setCollisionNone(sprite_t *const self);
void sprite_setCollisionSquare(sprite_t *const self,
                                      vec2 pos, vec2 size);
void sprite_setCollisionCircle(sprite_t *const self,
                                      vec2 pos, float size);

int col_isActive(col_t *const self);
int col_isResponse(col_t *const self);
int col_isSquare(col_t *const self);
int col_isCircle(col_t *const self);



#ifdef __cplusplus
}
#endif


#endif /* SPRITECONSOLE_H_ */


