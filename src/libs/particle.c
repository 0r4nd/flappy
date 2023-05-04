

#include "particle.h"
#include "colorUtil.h"


/******************************************************************************/

/**
 * utils
 */
float randf_minMax(float low, float high)
{
  return (((float)rand() / (float)RAND_MAX) * (high - low)) + low;
}
static int rand_minMax(int low, int high)
{
  return rand() % (high - low + 1) + low;
}


/******************************************************************************/

/**
 * set a sprite
 *
 * @param x     - x position
 * @param y     - y position
 * @param tex   - char to draw into the console
 * @param color - color of the char into the console
 * @param len   - length of the sprite (for snake)
 * @return the sprite_t structure
 */
particle_t particle_new(int x, int y, uint32_t color, uint32_t flags)
{
  particle_t ret;
  particle_memset(&ret,0,sizeof(ret));
  ret.flags = flags;
  ret.color = color;
  ret.x = x;
  ret.y = y;
  ret.active = 1;
  return ret;
}


/******************************************************************************/

/**
 * physic update
 *
 * @param self - a sprite_t structure
 * @param buf  - buffer to draw
 * @param dt   - delta time
 * @return -
 */
void particle_randInQuad(particle_t *const self,
                                int xmin, int xmax, int ymin, int ymax,
                                float fx, float fy,
                                float dxmax, float dymax)
{
  assert(self);
  if (0 == self->active) return 0;
  self->x = randf_minMax((float)xmin,(float)xmax);
  self->y = randf_minMax((float)ymin,(float)ymax);
  self->fx = fx;
  self->fy = fy;
  self->dxmax = dxmax;
  self->dymax = dymax;
}


/******************************************************************************/

/**
 * physic update
 *
 * @param self - a sprite_t structure
 * @param buf  - buffer to draw
 * @param dt   - delta time
 * @return -
 */
void particle_update(particle_t *const self, bufferConsole_t *out, float dt)
{
  int tmp;
  assert(self);
  if (0 == self->active) return 0;

  self->dx += self->fx;
  if (self->dx > self->dxmax) self->dx = self->dxmax;
  else if (self->dx < -self->dxmax) self->dx = -self->dxmax;

  self->dy += self->fy;
  if (self->dy > self->dymax) self->dy = self->dymax;
  else if (self->dy < -self->dymax) self->dy = -self->dymax;

  self->x += self->dx * dt;
  self->y += self->dy * dt;

  /* wrap */
  /*if (self->x < 0) {
    particle_randInQuad(self, 0, out->size.X, 0, 0,
                                             0.03, 0.03);
  } else if (self->x > out->size.X);
  */
  if (self->x < 0 || self->y > out->size.Y) {
    self->color = RGBA32(255,255,255,rand_minMax(128,255));
    tmp = (self->color>>24)&0xff;
    particle_randInQuad(self, 0, out->size.X+out->size.X/4, 0, 0,
                               randf_minMax(-0.01,-0.03), randf_minMax(0.009,0.01),
                               (randf_minMax(0.3,0.7)*(float)tmp) / 255.0,
                               (randf_minMax(0.4,0.8)*(float)tmp) / 255.0);
  }

}


/******************************************************************************/

/**
 * draw a sprite into a buffer
 *
 * @param self - a sprite_t structure
 * @param buf  - buffer to draw
 * @return -
 */
void particle_fill(particle_t *const self, bufferConsole_t *out)
{

  assert(self);
  if (0 == self->active) return 0;

  bufferConsole_drawWuPixel(out, self->color, self->x*4096.0,self->y*4096.0, 0);
}




