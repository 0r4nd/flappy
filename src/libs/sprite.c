

#include "sprite.h"

//#define SPRITECONSOLE_DEBUG 1


#define COL_DETECTION (1<<0) /* detect a collision */
#define COL_RESPONSE  (1<<1) /* move the item when collided */
#define COL_ISSQUARE  (1<<2) /* collision is a square */
#define COL_ISCIRCLE  (1<<3) /* collision is a circle */


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
sprite_t sprite_new(img_t *image, vec2 pos, vec2 size, uint32_t flags)
{
  sprite_t ret;
  sprite_memset(&ret,0,sizeof(ret));
  ret.flags = flags;
  ret.image = image;
  ret.pos = pos;
  ret.scale = 1.0;

  ret.xframe_count = 1;
  ret.yframe_count = 1;

  ret.size = size;
  ret.opacity = 0;
  ret.active = 1;
  return ret;
}

void sprite_delete(sprite_t *const self)
{
  assert(self);
  self->active = 0;
}



/******************************************************************************/

/**
 * set a random sprite into a buffer
 *
 * @param tex - char to draw into the console
 * @param buf - the buffer
 * @return the sprite_t structure
 */
/*sprite_t sprite_randInBuffer(char tex, bufferConsole_t *buf)
{
  int x,y,len;
  uint8_t color;
  int width = buf->size.X;
  int height = buf->size.Y;
  int pitch = buf->pitch;

  do {
    x = rand()%(width-2) + 1;
    y = rand()%(height-2) + 1;
  } while (buf->cur_[(width+pitch)*y + x] != ' ');

  len = rand()&1;
  color.Char.UnicodeChar = tex;
  color.Attributes = 0xf7;
  if (1 == len) color.Attributes = 0xf8;
  return sprite(x,y,color,len+1);
}*/


/******************************************************************************/

/**
 * physic update
 *
 * @param self - a sprite_t structure
 * @param buf  - buffer to draw
 * @param dt   - delta time
 * @return -
 */
int sprite_update(sprite_t *const self, bufferConsole_t *out, float dt)
{
  int collide = 0;
  float xframe_count,yframe_count;

  assert(self);
  if (0 == self->active) return 0;

  /*if (NULL != self->keypad_hook) {}*/

  self->col.vel.y += self->gravity;
  if (self->col.vel.y > 1.5) self->col.vel.y = 1.5;
  else if (self->col.vel.y < -1.5) self->col.vel.y = -1.5;

  self->pos.x += self->wind * dt;
  //if (self->dx > 1.0) self->dx = 1.0;
  //else if (self->dx < -1.0) self->dx = -1.0;

  self->pos.x += self->col.vel.x * dt;
  self->pos.y += self->col.vel.y * dt;

  /* collisions */
  if (col_isActive(&self->col)) {

    if (self->pos.y + self->col.pos.y + self->col.size.y > out->size.Y+2) {
      self->pos.y = out->size.Y+2 - self->col.pos.y - self->col.size.y;
      self->col.vel.y = 0;
      collide = 1;
    }

    if (self->pos.y + self->col.pos.y < -3) {
      self->pos.y = -3 - self->col.pos.y;
      self->col.vel.y = 0;
      collide = 1;
    }

  }

  xframe_count = self->xframe_count?self->xframe_count:1;
  yframe_count = self->yframe_count?self->yframe_count:1;

  self->xframe += self->xframe_speed;
  if (self->xframe >= xframe_count) self->xframe = 0;
  self->yframe += self->yframe_speed;
  if (self->yframe >= yframe_count) self->yframe = 0;

  self->decay_cumul += self->decay_speed*65536;

  return collide;
}


/******************************************************************************/

/**
 * draw a sprite into a buffer
 *
 * @param self - a sprite_t structure
 * @param buf  - buffer to draw
 * @return -
 *
 * @note - en temps normal cette fonction devrait être utilisée
 *         après avoir effectué un tri depuis la table de hash
 *         vers une structure tampon (le tri s'effectuant selon les z)
 */
void sprite_fill(sprite_t *const self, bufferConsole_t *out)
{
  int u1,u2, v1,v2;
  float xo,yo, xhalf,yhalf, scale,hscale;
  float xframe_count,yframe_count;
  //if (NULL == self->pix) return 0;
  //bufferConsole_fillTexQuad(out, self->pix + (int)self->frame * self->width * self->height,
  //                          0, (int)self->x,(int)self->y, self->width,self->height);

  assert(self);
  if (0 == self->active) return 0;

  xframe_count = self->xframe_count?self->xframe_count:1;
  yframe_count = self->yframe_count?self->yframe_count:1;

  u1 = 0;
  u2 = (self->image->texImg.xcount+1) / xframe_count;
  v1 = (int)(((int)self->yframe * self->image->texImg.ycount) / yframe_count);
  v2 = (int)((((int)self->yframe * self->image->texImg.ycount) + self->image->texImg.ycount +1) / yframe_count);
  scale = self->scale;
  hscale = scale*0.5;

  xhalf = self->size.x / 2.0;
  yhalf = self->size.y / 2.0;
  xo = self->pos.x + xhalf;
  yo = self->pos.y + yhalf;
  xhalf *= scale;
  yhalf *= scale;

  /* draw NEAREST or BILINEAR */
  if (self->flags&BUFFERCONSOLE_BILINEAR) {
    bufferConsole_fillScaleBilinTexQuad(out, self->image->texImg.data,
                                        self->decay_cumul, self->ydecay_ratio, self->opacity,
                                        (int)((xo-xhalf)*4096.0), (int)((yo-yhalf)*4096.0),
                                        u1, v1,
                                        (int)((xo+xhalf)*4096.0), (int)((yo+yhalf)*4096.0),
                                        u2, v2,
                                        self->image->texImg.xcount, self->image->texImg.ycount);

  } else {
    bufferConsole_fillScaleTexQuad(out, self->image->texImg.data,
                                   self->decay_cumul, self->ydecay_ratio, self->opacity,
                                   (int)((xo-xhalf)*4096.0), (int)((yo-yhalf)*4096.0),
                                   u1, v1,
                                   (int)((xo+xhalf)*4096.0), (int)((yo+yhalf)*4096.0),
                                   u2, v2,
                                   self->image->texImg.xcount, self->image->texImg.ycount);

  }

  /* debug */
  #ifdef SPRITECONSOLE_DEBUG
  if (!col_isActive(&self->col)) return;
  if (col_isCircle(&self->col)) {
    bufferConsole_drawCircle(out, 255 | 255<<24, 0,
                            (int)((self->pos.x+self->col.pos.x+self->col.size.x/2.0)*4096.0),
                            (int)((self->pos.y+self->col.pos.y+self->col.size.y/2.0)*4096.0),
                            (int)(self->col.size.x*4096.0/2.0));
  } else if (col_isSquare(&self->col)) {
    bufferConsole_fillFlatQuad(out, 255 | 255<<24, 0,
                              (int)((self->pos.x+self->col.pos.x)*4096.0),
                              (int)((self->pos.y+self->col.pos.y)*4096.0),
                              (int)((self->pos.x+self->col.pos.x+self->col.size.x)*4096.0),
                              (int)((self->pos.y+self->col.pos.y+self->col.size.y)*4096.0));
  }
  #endif

}

static int col_checkCirclePoint(col_t *const self, vec2 pos,
                                vec2 self_add)
{
  float cx,cy,d2;
  assert(self);

  cx = self->pos.x + self_add.x;
  cy = self->pos.y + self_add.y;
  d2 = (pos.x - cx)*(pos.x - cx) + (pos.y - cy)*(pos.y - cy);
  if (d2 > self->size.x * self->size.x) return 0;
  return 1;
}

static int col_checkCircle(col_t *const self, col_t *other,
                           vec2 self_add, vec2 other_add)
{
  float x,y,r;

  assert(self);

  x = (self_add.x + self->pos.x) - (other_add.x + other->pos.x);
  y = (self_add.y + self->pos.y) - (other_add.y + other->pos.y);
  r = self->size.x + other->size.x;
  return fabs(x * x + y * y) < r * r;
}
/*
static int col_responseCircle(col_t *const self, col_t *other,
                              vec2 self_add, vec2 other_add)
{
  float cx1 = self->pos.x;
  float cy1 = self->pos.y;
  float cx2 = other->pos.x;
  float cy2 = other->pos.y;
  float d = sqrtf(pow(cx1 - cx2, 2) + pow(cy1 - cy2, 2));
  float nx = (cx2 - cx1) / d;
  float ny = (cy2 - cy1) / d;
  float p = 2 * (self->vel.x * nx + self->vel.y * n_y - other->vel.x * nx - other->vel.y * n_y) /
                (self->mass + other->mass);
  vx1 = self->vel.x - p * self->mass * n_x;
  vy1 = self->vel.y - p * self->mass * n_y;
  vx2 = other->vel.x + p * other->mass * n_x;
  vy2 = other->vel.y + p * other->mass * n_y;
}
*/
static int col_checkSquare(col_t *const self, col_t *other,
                           vec2 self_add, vec2 other_add)
{
  assert(self);

  self_add.x += self->pos.x;
  self_add.y += self->pos.y;
  other_add.x += other->pos.x;
  other_add.y += other->pos.y;
  if (self_add.x + self->size.x > other_add.x &&
      self_add.x < other_add.x + other->size.x &&
      self_add.y + self->size.y > other_add.y &&
      self_add.y < other_add.y + other->size.y) return 1;
  return 0;
}
/*
Point closestpointonline(float lx1, float ly1,
                         float lx2, float ly2, float x0, float y0)
{
  vec2 ret;
  float A1 = ly2 - ly1;
  float B1 = lx1 - lx2;
  float C1 = (ly2 - ly1) * lx1 + (lx1 - lx2) * ly1;
  float C2 = -B1 * x0 + A1 * y0;
  float det = A1 * A1 - -B1 * B1;
  ret.x = x0;
  ret.y = y0;

  if (det != 0) {
    ret.x = (A1 * C1 - B1 * C2) / det;
    ret.y = (A1 * C2 - -B1 * C1) / det;
  }

  return ret;
}
*/

/******************************************************************************/

/**
 * collision sprite vs sprite
 *
 * @param self  - a sprite_t structure
 * @param other - a sprite_t structure
 * @return 1:yes 0:no
 */
int sprite_collide(sprite_t *const self, sprite_t *other)
{
  assert(self);
  if (0 == self->active) return 0;
  if (col_isActive(&self->col)) return 0;
  if (col_isActive(&other->col)) return 0;


  if (col_isSquare(&self->col)) {

    /* square vs square */
    if (col_isSquare(&other->col)) {
      /*if (self->x + self->col.x + self->col.width > other->x + other->col.x &&
          self->x + self->col.x < other->x + other->col.x + other->col.width &&
          self->y + self->col.y + self->col.height > other->y + other->col.y &&
          self->y + self->col.y < other->y + other->col.y + other->col.height) {
        return 1;
      }*/
    }

    /* square vs circle */
    if (col_isCircle(&other->col)) {
      /*if (self->x + self->col.x + self->col.width > other->x + other->col.x &&
          self->x + self->col.x < other->x + other->col.x + other->col.width &&
          self->y + self->col.y + self->col.height > other->y + other->col.y &&
          self->y + self->col.y < other->y + other->col.y + other->col.height) {
        return 1;
      }*/
    }

    return 0;
  }


  if (col_isCircle(&self->col)) {

    /* circle vs circle */
    if (col_isCircle(&other->col)) {
      return col_checkCircle(&self->col, &other->col,
                             self->pos, other->pos);

    }

    /* circle vs square */
    if (col_isSquare(&other->col)) {
      /*if (self->x + self->col.x + self->col.width > other->x + other->col.x &&
          self->x + self->col.x < other->x + other->col.x + other->col.width &&
          self->y + self->col.y + self->col.height > other->y + other->col.y &&
          self->y + self->col.y < other->y + other->col.y + other->col.height) {
        return 1;
      }*/
    }

    return 0;
  }

  return 0;
}



/******************************************************************************/

/**
 * some verifications
 *
 * @param self  - a col_t structure
 * @return 0:no 1:yes
 */
int col_isActive(col_t *const self)
{
  assert(self);
  return (self->flags&COL_DETECTION) != 0; /* force 0 or 1 */
}

int col_isResponse(col_t *const self)
{
  assert(self);
  return (self->flags&COL_RESPONSE) != 0; /* force 0 or 1 */
}

int col_isSquare(col_t *const self)
{
  assert(self);
  return (self->flags&COL_ISSQUARE) != 0; /* force 0 or 1 */
}

int col_isCircle(col_t *const self)
{
  assert(self);
  return (self->flags&COL_ISCIRCLE) != 0; /* force 0 or 1 */
}





/******************************************************************************/

/**
 * unset a collision
 *
 * @param self  - a sprite_t structure
 * @return -
 */
void sprite_setCollisionNone(sprite_t *const self)
{
  assert(self);
  self->col.flags = 0;
}


/******************************************************************************/

/**
 * set collision to square
 *
 * @param self  - a sprite_t structure
 * @return -
 */
void sprite_setCollisionSquare(sprite_t *const self,
                                      vec2 pos, vec2 size)
{
  assert(self);
  self->col.flags = COL_DETECTION | COL_ISSQUARE;
  self->col.pos = pos;
  self->col.size = size;
}



/******************************************************************************/

/**
 * set collision to circle
 *
 * @param self  - a sprite_t structure
 * @return -
 */
void sprite_setCollisionCircle(sprite_t *const self,
                                      vec2 pos, float size)
{
  assert(self);
  self->col.flags = COL_DETECTION | COL_ISCIRCLE;
  self->col.pos = pos;
  self->col.size.x = size;
  self->col.size.y = size;
}




