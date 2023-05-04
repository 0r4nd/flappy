

#include "text.h"




/******************************************************************************/

/**
 * draw a sprite into a buffer
 *
 * @param self  - a sprite_t structure
 * @param buf   - buffer to draw
 * @param flags -  BUFFER_BILINEAR
 * @return -
 */
void text_fill(const char *str, img_t *image, bufferConsole_t *out,
                      int x, int y, float size, uint32_t color, uint32_t flags)
{
  int u1,u2, v1,v2;
  float xo,yo, xhalf,yhalf, scale,hscale;
  float xframe_count=16,yframe_count=16;
  uint32_t xframe,yframe;
  //if (NULL == self->pix) return 0;
  //bufferConsole_fillTexQuad(out, self->pix + (int)self->frame * self->width * self->height,
  //                          0, (int)self->x,(int)self->y, self->width,self->height);
  assert(str);

  scale = 1.0;
  hscale = scale*0.5;

  xhalf = size / 2.0;
  yhalf = size / 2.0;
  xhalf *= scale;
  yhalf *= scale;
  //size -= space;


  /* draw NEAREST or BILINEAR */
  if (flags&BUFFERCONSOLE_BILINEAR) {

    while (*str) {
      xframe = (*str)%16;
      yframe = (*str)/16;
      xo = x + xhalf;
      yo = y + yhalf;
      u1 = (int)((xframe * image->texImg.xcount) / xframe_count);
      u2 = (int)(((xframe * image->texImg.xcount) + image->texImg.xcount +1) / xframe_count);
      v1 = (int)((yframe * image->texImg.ycount) / yframe_count);
      v2 = (int)(((yframe * image->texImg.ycount) + image->texImg.ycount +1) / yframe_count);
      bufferConsole_fillScaleBilinTexQuad(out, image->texImg.data,
                                          0, NULL, 0,
                                          (int)((xo-xhalf)*4096.0), (int)((yo-yhalf)*4096.0),
                                          u1, v1,
                                          (int)((xo+xhalf)*4096.0), (int)((yo+yhalf)*4096.0),
                                          u2, v2,
                                          image->texImg.xcount, image->texImg.ycount);
      str++;
      x += size;
    }

  } else {

    while (*str) {
      xframe = (*str)%16;
      yframe = (*str)/16;
      xo = x + xhalf;
      yo = y + yhalf;
      u1 = (int)((xframe * image->texImg.xcount) / xframe_count);
      u2 = (int)(((xframe * image->texImg.xcount) + image->texImg.xcount +1) / xframe_count);
      v1 = (int)((yframe * image->texImg.ycount) / yframe_count);
      v2 = (int)(((yframe * image->texImg.ycount) + image->texImg.ycount +1) / yframe_count);

      bufferConsole_fillScaleTexSubQuad(out, image->texImg.data,
                                        0, NULL, color,255,
                                        (int)((xo-xhalf)*4096.0), (int)((yo-yhalf)*4096.0),
                                        u1, v1,
                                        (int)((xo+xhalf)*4096.0), (int)((yo+yhalf)*4096.0),
                                        u2, v2,
                                        image->texImg.xcount, image->texImg.ycount);
      str++;
      x += size;
    }

  }


}






