#ifndef PARTICLE_H_
#define PARTICLE_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */


#include "bufferConsole.h"


#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/

/**
 * User wrapped functions and options.
 */
#define particle_memset(out,val,sz) memset(out,val,sz)


/******************************************************************************/

/**
 * structs
 */
typedef struct particle_s {
  uint32_t flags;
  uint32_t color;
  float x,y;   /* position */
  float dx,dy; /* acceleration */
  float fx,fy; /* force */
  float dxmax,dymax; /* max acceleration */
  char active;
} particle_t;



/******************************************************************************/

/**
 * prototypes
 */
float randf_minMax(float low, float high);
particle_t particle_new(int x, int y, uint32_t color, uint32_t flags);
void particle_randInQuad(particle_t *const self,
                                int xmin, int xmax, int ymin, int ymax,
                                float fx, float fy,
                                float dxmax, float dymax);
void particle_update(particle_t *const self, bufferConsole_t *out, float dt);
void particle_fill(particle_t *const self, bufferConsole_t *out);



#ifdef __cplusplus
}
#endif


#endif /* PARTICLECONSOLE_H_ */


