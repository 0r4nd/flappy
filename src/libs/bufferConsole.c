

#include "bufferConsole.h"
#include "colorUtil.h"

#include "file/file.h"

CHAR_INFO aCHARINFO_LUT[32*32*32];



/******************************************************************************/

/**
 * create a new buffer
 *
 * @param self   - a bufferConsole_t strucure
 * @param hnd    - handle on the console
 * @param width  - buffer width (!= console width)
 * @param height - buffer height (!= console height)
 * @return if (!=0) == error
 */
int bufferConsole_new(bufferConsole_t *const self, HANDLE hnd,
                      short width, short height)
{
  int j;

  assert(self);
  if (width < 8 || height < 8 || width < 8) return -2;

  bufferConsole_memset(self,0,sizeof(*self));

  /* front + back buffer allocation */
  self->front = bufferConsole_realloc(NULL, width*height * sizeof(*self->front) * 2);
  if (NULL == self->front) return -3;
  bufferConsole_memset(self->front, 0, width*height * sizeof(*self->front) * 2);

  /* pixels buffer */
  self->pixels = bufferConsole_realloc(NULL, width*height * sizeof(*self->pixels));
  if (NULL == self->pixels) return -3;
  bufferConsole_memset(self->pixels, 0, width*height * sizeof(*self->pixels));

  self->back = self->front + width*height;
  self->cur = self->front;

  self->size.X = width;
  self->size.Y = height;
  self->region.Right = width-1;
  self->region.Bottom = height-1;
  self->hnd = hnd;

  for (j = 0; j < width * height * 2; ++j) {
    self->front[j].Char.UnicodeChar = ' ';
  }

  self->front[(height-1)*width + width].Char.UnicodeChar = '\0';
  self->back[(height-1)*width + width].Char.UnicodeChar = '\0';

  return 0;
}


/******************************************************************************/

/**
 * delete a buffer
 *
 * @param self - a bufferConsole_t structure
 * @return -
 */
void bufferConsole_delete(bufferConsole_t *const self)
{
  assert(self);
  bufferConsole_realloc(self->front,0);
  bufferConsole_realloc(self->pixels,0);
  self->front = NULL;
  self->back = NULL;
  self->cur = NULL;
  self->pixels = NULL;
}


/* ici des fonctions relatives au traitement des niblles non terminées */
#if 0

/******************************************************************************/

/**
 * clear the current sub-buffer
 *
 * @param self   - a bufferConsole_t structure
 * @param color  - the color
 * @param x      - x console position
 * @param y      - y console position
 * @param width  - width of the quad
 * @param height - height of the quad
 * @return -
 *
 * @note - empty :  ' '
 *         full :   '\xDB'
 *         bottom : '\xDC'
 *         top :    '\xDF'
 *
 * @todo - ne fonctionne pas :
 *       il faut trouver un moyen d'avoir deux nibbles de couleur différente
 *       sur le même char.
 */
void bufferConsole_fillPlainQuad_nibble(bufferConsole_t *const self, char color, int depth,
                                             int x, int y, int width, int height)
{
  int i=x,j=(y&~1)/2,offset;
  int dst;

  assert(self);

  /* first line */
  for (i = x; i < width+x; i++) {
    offset = (self->size.X + self->pitch)*j + i;
    /* keep the background */
    dst = self->ccur_[offset];
    self->ccur_[offset] = (dst&(0xf<<4)) | (color&0xf);

    if ((y&1)==0) self->cur_[offset] = '\xDB';
    else self->cur_[offset] = '\xDC';
  }

  y += 1;
  for (j = (y&~1)/2; j < (height+y)/2; j++)
    for (i = x; i < width+x; i++) {
      offset = (self->size.X + self->pitch)*j + i;
      /* keep the background */
      dst = self->ccur_[offset];
      self->ccur_[offset] = (dst&(0xf<<4)) | ((color+(i&1)+(j&1))&0xf);

      /*
      if ((y&1)==0) self->cur_[offset] = '\xDC';
      else*/
        self->cur_[offset] = '\xDB';
    }

}



void bufferConsole_fillPix_nibble(bufferConsole_t *const self, uint8_t color, int depth,
                                  int x, int y)
{
  int i=x,j=(y&~1)/2,offdst,offsrc;
  int dst,align;

  assert(self);

  offdst = (self->size.X + self->pitch) * j + i;

  /* keep the background */
  if (color < 15) {
    dst = self->ccur_[offdst];
    self->ccur_[offdst] = (dst & (0xf << 4)) | (color & 0xf);

    if ((y & 1) == 0) self->cur_[offdst] = '\xDF';
    else self->cur_[offdst] = '\xDC';
  }

}


static void bufferConsole_fillPix2_nibble(bufferConsole_t *const self, uint8_t color, int depth,
                                          int x, int y)
{
  int i=x,j=(y&~1)/2,offdst,offsrc;
  int dst,align;

  assert(self);

  offdst = (self->size.X + self->pitch) * j + i;

  /* keep the background */
  if (color != 0) {
    dst = self->ccur_[offdst];
    self->ccur_[offdst] = (dst & (0xf << 4)) | (color & 0xf);

    self->cur_[offdst] = '\xDB';
  }

}


/******************************************************************************/

/**
 * @todo -
 * L'affichage avec des nibbles pose un problème :
 * On ne peut pas afficher deux nibbles verticaux (utilisant le même char) tout
 * en ayant une couleur spécifique pour chaqun.
 * J'ai vu des screen de personnes qui - a prioris - avaient des solutions pour
 * ce problème, sans pour autant avoir exposé leur procédé.
 * L'intéret étant de diviser par 2 la hauteur de l'affichage.
 *
 * http://stackoverflow.com/questions/12378642/c-pixels-in-console-window
 *
 * ' '    = U+2588 EMPTY BLOCK
 * '\xDB' = U+2588 FULL BLOCK
 * '\xDC' = U+2584 LOWER HALF BLOCK
 * '\xDF' = U+2580 UPPER HALF BLOCK
 *
 * L'affichage en pixel est beaucoup trop "tricky" et bas niveau, il y a peu de
 * chances que ça fonctionne sur toutes les plateformes.
 *
 * à suivre donc.
 */
void bufferConsole_fillTexQuad_nibble(bufferConsole_t *const self, uint8_t *color, int depth,
                                      int x, int y, int width, int height)
{
  int i=x,j=y,offdst,offsrc;
  int dst,align;

  assert(self);

  for (j = y; j < height+y; j++)
    for (i = x; i < width+x; i++) {

      /*if ((j&1) == 0) {
        bufferConsole_fillPix2_nibble(self, *color, depth, i,j);
      } else {*/
        bufferConsole_fillPix_nibble(self, *color, depth, i,j);
      //}

      color++;
    }

}
#endif



/******************************************************************************/

/**
 * draw a scaled horizontal line with GL_REPEAT/GL_REPEAT behavior
 *
 * @param pdst      - destination in RGBA32
 * @param psrc      - source in RGBA32
 * @param opacity   - 0:opaque 255:invisible
 * @param x         - start x position (fixed .12)
 * @param y         - start y position (fixed .12)
 * @param u         - start u position (fixed .16 in texture)
 * @param du        - delta u
 * @param v         - start v position (fixed .16 in texture)
 * @param dv        - delta v
 * @param len       - len of the line
 * @param srcwidth  - texture width
 * @param srcheight - texture height
 * @return -
 */
static void rgb32_fillHlineFlat(uint32_t *pdst, uint32_t color, int opacity,
                                int x, int len)
{
  int a;
  uint32_t dst, r1,g1,b1, r2,g2,b2;

  a = (color>>24)&0xff;
  a -= opacity;
  if (a <= 0) return;
  if (a >= 255) {
    while (len-- > 0) {
      pdst[x>>12] = color;
      x += 1<<12 ;
    }
    return;
  }

  r2 = (color)&0xff;
  g2 = (color>>8)&0xff;
  b2 = (color>>16)&0xff;

  while (len-- > 0) {
    dst = pdst[x>>12];
    r1 = (dst)&0xff;
    g1 = (dst>>8)&0xff;
    b1 = (dst>>16)&0xff;

    r1 += (r2-r1)*a>>8;
    g1 += (g2-g1)*a>>8;
    b1 += (b2-b1)*a>>8;
    pdst[x>>12] = r1 | g1<<8 | b1<<16;

    x += 1<<12 ;
  }

}



static void rgb32_fillHlineFlatSub(uint32_t *pdst, int color_sub, int x, int len)
{
  uint32_t dst;
  int r1,g1,b1, r2,g2,b2;

  r2 = (color_sub)&0xff;
  g2 = (color_sub>>8)&0xff;
  b2 = (color_sub>>16)&0xff;

  while (len-- > 0) {
    dst = pdst[x>>12];
    r1 = (dst)&0xff;
    g1 = (dst>>8)&0xff;
    b1 = (dst>>16)&0xff;
    pdst[x>>12] = rgb888_clamp(r1-r2, g1-g2, b1-b2);
    x += 1<<12 ;
  }

}


/******************************************************************************/

/**
 * draw a scaled horizontal line with GL_REPEAT/GL_REPEAT behavior
 *
 * @param pdst      - destination in RGBA32
 * @param psrc      - source in RGBA32
 * @param opacity   - 0:opaque 255:invisible
 * @param x         - start x position (fixed .12)
 * @param y         - start y position (fixed .12)
 * @param u         - start u position (fixed .16 in texture)
 * @param du        - delta u
 * @param v         - start v position (fixed .16 in texture)
 * @param dv        - delta v
 * @param len       - len of the line
 * @param srcwidth  - texture width
 * @param srcheight - texture height
 * @return -
 */
static void rgb32_fillScaleHlineTex(uint32_t *pdst, uint32_t *psrc, int opacity,
                                    int x, int y, int u, int du, int v, int dv,
                                    int len, uint32_t srcwidth, uint32_t srcheight)
{
  int a;
  uint32_t src,dst, r1,g1,b1, r2,g2,b2;

  /* not used vars */
  (void)v;
  (void)y;
  (void)dv;
  (void)srcheight;

  while (len-- > 0) {
    src = psrc[(u>>16)%srcwidth];
    a = (src>>24)&0xff;
    a -= opacity;

    if (a > 0) {

      /* with alpha */
      if (a >= 255) {
        pdst[x>>12] = src;
      } else { /* without alpha */
        dst = pdst[x>>12];
        r1 = (dst)&0xff;
        g1 = (dst>>8)&0xff;
        b1 = (dst>>16)&0xff;

        r2 = (src)&0xff;
        g2 = (src>>8)&0xff;
        b2 = (src>>16)&0xff;

        r1 += (r2-r1)*a>>8;
        g1 += (g2-g1)*a>>8;
        b1 += (b2-b1)*a>>8;

        pdst[x>>12] = r1 | g1<<8 | b1<<16;
      }

    }

    u += du ;
    x += 1<<12 ;
  }

}

static void rgb32_fillScaleHlineTexSub(uint32_t *pdst, uint32_t *psrc, uint32_t color_sub,
                                       int x, int y, int u, int du, int v, int dv,
                                       int len, uint32_t srcwidth, uint32_t srcheight)
{
  int a;
  uint32_t src,dst, r1,g1,b1, r2,g2,b2, rd,gd,bd;

  /* not used vars */
  (void)v;
  (void)y;
  (void)dv;
  (void)srcheight;

  r2 = (color_sub)&0xff;
  g2 = (color_sub>>8)&0xff;
  b2 = (color_sub>>16)&0xff;

  while (len-- > 0) {
    src = psrc[(u>>16)%srcwidth];
    a = (src>>24)&0xff;

    if (a > 0) {
      r1 = (src)&0xff;
      g1 = (src>>8)&0xff;
      b1 = (src>>16)&0xff;

      if (a < 255) {
        r1 = (src)&0xff;
        g1 = (src>>8)&0xff;
        b1 = (src>>16)&0xff;

        dst = pdst[x>>12];
        rd = (dst)&0xff;
        gd = (dst>>8)&0xff;
        bd = (dst>>16)&0xff;

        r1 += (rd-r1)*a>>8;
        g1 += (gd-g1)*a>>8;
        b1 += (bd-b1)*a>>8;
      }
      pdst[x>>12] = rgb888_clamp(r1-r2,g1-g2,b1-b2);
    }
    u += du ;
    x += 1<<12 ;
  }

}


/******************************************************************************/

/**
 * draw a scaled bilinear horizontal line with GL_REPEAT/GL_REPEAT behavior
 *
 * @param pdst      - destination in RGBA32
 * @param psrc      - source in RGBA32
 * @param opacity   - 0:opaque 255:invisible
 * @param x         - start x position (fixed .12)
 * @param y         - start y position (fixed .12)
 * @param u         - start u position (fixed .16 in texture)
 * @param du        - delta u
 * @param v         - start v position (fixed .16 in texture)
 * @param dv        - delta v
 * @param len       - len of the line
 * @param srcwidth  - texture width
 * @param srcheight - texture height
 * @return -
 *
 * @note - Pistes d'optimisation :
 *
 *  - Si on décidait par convention que la texture source possède toujours une
 *    taille puissance de 2. On pourrait remplacer l'opérateur modulo par un
 *    masque(&), et la multiplication par un simple décalage (<<)
 *
 *  - 4 pixels sont lu pour chaque pixel affiché, nous pourrions en
 *    lire seulement 2 ou 1 selon les cas.
 *
 *  - Nous pourrions découper le traitement en sous fonctions pour gérer
 *    beaucoup plus de cas.
 *    Les cas spécifiques seraient liés au mix des composantes
 *    src-src puis src_res-dst
 *    par exemple : mix: 50/50 = (r1/2)        + (r2/2)  ou  (r1+r2)/2
 *                  mix: 75/25 = (r1/2)+(r1/4) + (r2/4)
 *    Sans parler des spécificités sur les bords de la texture (clamp, repeat...)
 *    L'implémentation idéale consisterait à découper le tout en une
 *    série de fonctions spécialisés. (il y en aurait beaucoup)
 */
static void rgb32_fillScaleHlineTexBlin(uint32_t *pdst, uint32_t *psrc, int opacity,
                                        int x, int y, int u, int du, int v, int dv,
                                        int len, uint32_t srcwidth, uint32_t srcheight)
{
  int i, a1,a2,a3,a4;
  int src1,src2,src3,src4, dst;
  int r1,g1,b1, r2,g2,b2, r3,g3,b3, r4,g4,b4, rdst,gdst,bdst;
  int v1,v2, horiz,vertic;

  /* this one is the correct way */
  /* u -= du/2; */
  /* v -= dv/2; */

  /* Vertical offset */
  v1 = (((v            ) >> 16) % srcheight) * srcwidth;
  v2 = (((v + (1 << 16)) >> 16) % srcheight) * srcwidth;

  /* Vertical lerp factor */
  vertic = (v>>8) & 0xff;

  for (i = 0; i < len; ++i, x += 1 << 12, u += du) {
    /* Horizontal lerp factor */
    horiz = (u >> 8) & 0xff;

    /* top-left : top-right : bottom-left : bottom-right */
    src1 = psrc[v1 +  ((u              >> 16) % srcwidth)];
    src2 = psrc[v1 + (((u + (1 << 16)) >> 16) % srcwidth)];
    src3 = psrc[v2 +  ((u              >> 16) % srcwidth)];
    src4 = psrc[v2 + (((u + (1 << 16)) >> 16) % srcwidth)];

    /* compute the opacity first */
    a1 = (src1 >> 24) & 0xff;
    a2 = (src2 >> 24) & 0xff;
    a3 = (src3 >> 24) & 0xff;
    a4 = (src4 >> 24) & 0xff;
    a1 += ((a2 - a1) * horiz) >> 8;
    a3 += ((a4 - a3) * horiz) >> 8;
    a1 += ((a3 - a1) * vertic) >> 8;
    a1 -= opacity;
    if (a1 <= 0) continue;

    /* get colors */
    r1 = (src1) & 0xff;
    g1 = (src1 >> 8) & 0xff;
    b1 = (src1 >> 16) & 0xff;
    r2 = (src2) & 0xff;
    g2 = (src2 >> 8) & 0xff;
    b2 = (src2 >> 16) & 0xff;
    r3 = (src3) & 0xff;
    g3 = (src3 >> 8) & 0xff;
    b3 = (src3 >> 16) & 0xff;
    r4 = (src4) & 0xff;
    g4 = (src4 >> 8) & 0xff;
    b4 = (src4 >> 16) & 0xff;

    /* horizontal lerp between top-src pixels */
    r1 += ((r2 - r1) * horiz) >> 8;
    g1 += ((g2 - g1) * horiz) >> 8;
    b1 += ((b2 - b1) * horiz) >> 8;

    /* horizontal lerp between bottom-src pixels */
    r3 += ((r4 - r3) * horiz) >> 8;
    g3 += ((g4 - g3) * horiz) >> 8;
    b3 += ((b4 - b3) * horiz) >> 8;

    /* combine top and bottom pixels */
    r1 += ((r3 - r1) * vertic) >> 8;
    g1 += ((g3 - g1) * vertic) >> 8;
    b1 += ((b3 - b1) * vertic) >> 8;


    /* alpha lerp between final-src and dst pixels */
    if (a1 < 255) {
      dst = pdst[x >> 12];
      rdst = (dst) & 0xff;
      gdst = (dst >> 8) & 0xff;
      bdst = (dst >> 16) & 0xff;
      r1 = rdst + ( ((r1 - rdst) * a1) >> 8 );
      g1 = gdst + ( ((g1 - gdst) * a1) >> 8 );
      b1 = bdst + ( ((b1 - bdst) * a1) >> 8 );
    }

    pdst[x >> 12] = r1 | g1 << 8 | b1 << 16;
  }

}


/******************************************************************************/

/**
 *
 * @param x,y : pos in fixed .12
 * @param u,v : texpos in integer
 *
 */
void bufferConsole_fillFlatQuad(bufferConsole_t *const self,
                                uint32_t color, int opacity,
                                int x1, int y1, int x2, int y2)
{
  int y, width,height;
  uint32_t *pdst;

  assert(self);

  /* culling */
  if (x1 >= x2) return;
  if (y1 >= y2) return;
  if ((x1>>12) > self->size.X) return;
  if (x2 < 0) return;
  if ((y1>>12) > self->size.Y) return;
  if (y2 < 0) return;

  /* clipping */
  if ((x2>>12) > self->size.X) x2 = self->size.X<<12;
  if ((x1>>12) < 0) x1 = 0;
  if ((y2>>12) > self->size.Y) y2 = self->size.Y<<12;
  if ((y1>>12) < 0) y1 = 0;

  width  = (x2>>12) - (x1>>12);
  height = (y2>>12) - (y1>>12);

  /* destination pixels */
  pdst = self->pixels;

  for (y = 0; y < height; ++y) {
    rgb32_fillHlineFlat(pdst + (y1>>12) * self->size.X, color, opacity, x1, width);
    y1 += 1<<12;
  }

}

void bufferConsole_fillFlatSubQuad(bufferConsole_t *const self,
                                   int color_sub, int factor,
                                   int x1, int y1, int x2, int y2)
{
  int y, width,height, r,g,b;
  uint32_t *pdst;

  assert(self);

  /* no need */
  if ((color_sub&0xffffff) == 0xffffff || factor <= 0) return;

  /* culling */
  if (x1 >= x2) return;
  if (y1 >= y2) return;
  if ((x1>>12) > self->size.X) return;
  if (x2 < 0) return;
  if ((y1>>12) > self->size.Y) return;
  if (y2 < 0) return;

  /* clipping */
  if ((x2>>12) > self->size.X) x2 = self->size.X<<12;
  if ((x1>>12) < 0) x1 = 0;
  if ((y2>>12) > self->size.Y) y2 = self->size.Y<<12;
  if ((y1>>12) < 0) y1 = 0;

  width  = (x2>>12) - (x1>>12);
  height = (y2>>12) - (y1>>12);

  /* destination pixels */
  pdst = self->pixels;

  /* inv */
  r = 255 -  (color_sub&0xff);
  g = 255 - ((color_sub>>8)&0xff);
  b = 255 - ((color_sub>>16)&0xff);
  if (factor > 255) factor = 255;
  color_sub = ((r*factor)>>8)
            | ((g*factor)>>8)<<8
            | ((b*factor)>>8)<<16;

  for (y = 0; y < height; ++y) {
    rgb32_fillHlineFlatSub(pdst + (y1>>12) * self->size.X, color_sub, x1, width);
    y1 += 1<<12;
  }

}



/******************************************************************************/

/**
 *
 * @param x,y : pos in fixed .12
 * @param u,v : texpos in integer
 *
 */
void bufferConsole_fillScaleTexQuad(bufferConsole_t *const self, uint32_t *psrc,
                                    int decay_cumul, float *ydecay, int opacity,
                                    int x1, int y1, int u1, int v1,
                                    int x2, int y2, int u2, int v2,
                                    uint32_t srcwidth, uint32_t srcheight)
{
  int y, dx,dy, du,dv, tmp, width,height;
  uint32_t *pdst;

  assert(self);

  /* culling */
  if (x1 >= x2) return;
  if (y1 >= y2) return;
  if ((x1>>12) > self->size.X) return;
  if (x2 < 0) return;
  if ((y1>>12) > self->size.Y) return;
  if (y2 < 0) return;

  /* lerp on texture : fixed .16 */
  dx = (x2 - x1) >> 12;
  dy = (y2 - y1) >> 12;
  dx = 65536 / (dx?dx:1);
  dy = 65536 / (dy?dy:1);
  du = (u2 - u1) * dx;
  dv = (v2 - v1) * dy;
  u1 <<= 16;
  v1 <<= 16;

  /* clipping */
  if ((x2>>12) > self->size.X) x2 = self->size.X<<12;
  if ((x1>>12) < 0) {
    u1 += ((0 - x1) >> 12) * du;
    x1 = 0;
  }
  if ((y2>>12) > self->size.Y) y2 = self->size.Y<<12;
  if (y1 < 0) {
    v1 += ((0 - y1) >> 12) * dv;
    y1 = 0;
  }

  width  = (x2>>12) - (x1>>12);
  height = (y2>>12) - (y1>>12);

  /* destination pixels */
  pdst = self->pixels;

  if (NULL == ydecay) {

    for (y = 0; y < height; ++y) {
      tmp = (v1 >> 16) * srcwidth;
      rgb32_fillScaleHlineTex(pdst + (y1>>12) * self->size.X, psrc + tmp, opacity,
                              x1, y1,
                              u1+decay_cumul, du,
                              v1, dv,
                              width, srcwidth,srcheight);

      v1 += dv;
      y1 += 1<<12;
    }

  } else {

    for (y = 0; y < height; ++y) {
      tmp = (v1 >> 16) * srcwidth;
      rgb32_fillScaleHlineTex(pdst + (y1>>12) * self->size.X, psrc + tmp, opacity,
                              x1, y1,
                              u1+(int)(decay_cumul*ydecay[y]),du,
                              v1, dv,
                              width, srcwidth,srcheight);
      v1 += dv;
      y1 += 1<<12;
    }

  }

}

void bufferConsole_fillScaleTexSubQuad(bufferConsole_t *const self, uint32_t *psrc,
                                       int decay_cumul, float *ydecay,
                                       int color_sub, int factor,
                                       int x1, int y1, int u1, int v1,
                                       int x2, int y2, int u2, int v2,
                                       uint32_t srcwidth, uint32_t srcheight)
{
  int y, dx,dy, du,dv, tmp, width,height, r,g,b;
  uint32_t *pdst;

  assert(self);

  /* no need */
  if ((color_sub&0xffffff) == 0xffffff || factor <= 0) return;

  /* culling */
  if (x1 >= x2) return;
  if (y1 >= y2) return;
  if ((x1>>12) > self->size.X) return;
  if (x2 < 0) return;
  if ((y1>>12) > self->size.Y) return;
  if (y2 < 0) return;

  /* lerp on texture : fixed .16 */
  dx = (x2 - x1) >> 12;
  dy = (y2 - y1) >> 12;
  dx = 65536 / (dx?dx:1);
  dy = 65536 / (dy?dy:1);
  du = (u2 - u1) * dx;
  dv = (v2 - v1) * dy;
  u1 <<= 16;
  v1 <<= 16;

  /* clipping */
  if ((x2>>12) > self->size.X) x2 = self->size.X<<12;
  if ((x1>>12) < 0) {
    u1 += ((0 - x1) >> 12) * du;
    x1 = 0;
  }
  if ((y2>>12) > self->size.Y) y2 = self->size.Y<<12;
  if (y1 < 0) {
    v1 += ((0 - y1) >> 12) * dv;
    y1 = 0;
  }

  width  = (x2>>12) - (x1>>12);
  height = (y2>>12) - (y1>>12);

  /* destination pixels */
  pdst = self->pixels;

  /* inv */
  r = 255 -  (color_sub&0xff);
  g = 255 - ((color_sub>>8)&0xff);
  b = 255 - ((color_sub>>16)&0xff);
  if (factor > 255) factor = 255;
  color_sub = ((r*factor)>>8)
            | ((g*factor)>>8)<<8
            | ((b*factor)>>8)<<16;


  if (NULL == ydecay) {

    for (y = 0; y < height; ++y) {
      tmp = (v1 >> 16) * srcwidth;
      rgb32_fillScaleHlineTexSub(pdst + (y1>>12) * self->size.X, psrc + tmp,
                                 color_sub,
                                 x1, y1,
                                 u1+decay_cumul, du,
                                 v1, dv,
                                 width, srcwidth,srcheight);
      v1 += dv;
      y1 += 1<<12;
    }

  } else {

    for (y = 0; y < height; ++y) {
      tmp = (v1 >> 16) * srcwidth;
      rgb32_fillScaleHlineTexSub(pdst + (y1>>12) * self->size.X, psrc + tmp,
                                 color_sub,
                                 x1, y1,
                                 u1+(int)(decay_cumul*ydecay[y]),du,
                                 v1, dv,
                                 width, srcwidth,srcheight);
      v1 += dv;
      y1 += 1<<12;
    }

  }

}



/******************************************************************************/

/**
 *
 * @param x,y : pos in fixed .12
 * @param u,v : texpos in integer
 *
 */
void bufferConsole_fillScaleBilinTexQuad(bufferConsole_t *const self, uint32_t *psrc,
                                         int decay_cumul, float *ydecay, uint32_t opacity,
                                         int x1, int y1, int u1, int v1,
                                         int x2, int y2, int u2, int v2,
                                         uint32_t srcwidth, uint32_t srcheight)
{
  int y, dx,dy, du,dv, width,height;
  uint32_t *pdst;

  assert(self);

  /* culling */
  if (x1 >= x2) return;
  if (y1 >= y2) return;
  if ((x1>>12) > self->size.X) return;
  if (x2 < 0) return;
  if ((y1>>12) > self->size.Y) return;
  if (y2 < 0) return;

  /* lerp on texture : fixed .16 */
  dx = (x2 - x1) >> 12;
  dy = (y2 - y1) >> 12;
  dx = 65536 / (dx?dx:1);
  dy = 65536 / (dy?dy:1);
  du = (u2 - u1) * dx;
  dv = (v2 - v1) * dy;
  u1 <<= 16;
  v1 <<= 16;

  /* clipping */
  if ((x2>>12) > self->size.X+1) x2 = (self->size.X+1)<<12;
  if ((x1>>12) < 0) {
    u1 += ((0 - x1) >> 12) * du;
    x1 = 0;
  }
  if ((y2>>12) > self->size.Y+1) y2 = (self->size.Y+1)<<12;
  if (y1 < 0) {
    v1 += ((0 - y1) >> 12) * dv;
    y1 = 0;
  }

  width  = (x2>>12) - (x1>>12);
  height = (y2>>12) - (y1>>12);

  /* destination pixels */
  pdst = self->pixels;

  /* trick */
  height -= 1;
  width -= 1;


  if (NULL == ydecay) {

    for (y = 0; y < height; ++y) {
      rgb32_fillScaleHlineTexBlin(pdst + (y1>>12) * self->size.X, psrc, opacity,
                                  x1, y1,
                                  u1+decay_cumul, du,
                                  v1, dv,
                                  width, srcwidth,srcheight);
      v1 += dv;
      y1 += 1<<12;
    }

  } else {

    for (y = 0; y < height; ++y) {
      rgb32_fillScaleHlineTexBlin(pdst + (y1>>12) * self->size.X, psrc, opacity,
                                  x1, y1,
                                  u1+(int)(decay_cumul*ydecay[y]),du,
                                  v1, dv,
                                  width, srcwidth,srcheight);
      v1 += dv;
      y1 += 1<<12;
    }

  }

}


int subClamp(int col, int sub)
{
  col -= sub;
  return col <= 0 ? 0 : col;
}

/******************************************************************************/

/**
 * draw a pixel
 *
 * @param self    - a bufferConsole_t structure
 * @param col     - color of the pixel in RGBA32
 * @param x       - x position on the buffer (fixed .12)
 * @param y       - y position on the buffer (fixed .12)
 * @param opacity - 0:opaque 255:invisible
 * @return -
 */
void bufferConsole_drawPixel(bufferConsole_t *const self,
                             uint32_t col, int x, int y, int opacity)
{
  int a;
  uint32_t dstwidth;
  uint32_t *pdst,dst;
  int32_t r,g,b, rd,gd,bd;

  assert(self);

  /* culling */
  if ((x>>12) > self->size.X-2) return;
  if (x < 0) return;
  if ((y>>12) > self->size.Y-2) return;
  if (y < 0) return;

  /* destination pixels */
  pdst = self->pixels;
  dstwidth = self->size.X;

  pdst += (y >> 12) * dstwidth + (x >> 12) ;

  /* final pixel alpha */
  a = (col>>24)&0xff;
  a -= opacity;
  if (a <= 0) return;

  /* without alpha */
  if (a >= 255) {
    *pdst = col;
  } else { /* with alpha */
    dst = *pdst;
    rd = (dst)&0xff;
    gd = (dst>>8)&0xff;
    bd = (dst>>16)&0xff;

    r = (col)&0xff;
    g = (col>>8)&0xff;
    b = (col>>16)&0xff;

    rd += (r-rd)*a>>8;
    gd += (g-gd)*a>>8;
    bd += (b-bd)*a>>8;
    *pdst = rd | gd<<8 | bd<<16;
  }

}



/******************************************************************************/

/**
 * draw a wuPixel
 *
 * @param self    - a bufferConsole_t structure
 * @param col     - color of the pixel in RGBA32
 * @param x       - x position on the buffer (fixed .12)
 * @param y       - y position on the buffer (fixed .12)
 * @param opacity - 0:opaque 255:invisible
 * @return -
 */
void bufferConsole_drawWuPixel(bufferConsole_t *const self,
                               uint32_t col, int x, int y, int opacity)
{
  int a,ao, xmod,ymod;
  uint32_t dstwidth;
  uint32_t r1,g1,b1, r2,g2,b2;
  uint32_t *pdst;

  assert(self);

  /* culling */
  if ((x>>12) > self->size.X-2) return;
  if (x < 0) return;
  if ((y>>12) > self->size.Y-2) return;
  if (y < 0) return;

  /* alpha */
  ao = (col >> 24) & 0xff;
  ao -= opacity;
  if (ao <= 0 || ao > 255) return;

  /* destination pixels */
  pdst = self->pixels;
  dstwidth = self->size.X;

  pdst += (y >> 12) * dstwidth + (x >> 12);
  xmod = (x >> 4) & 0xff;
  ymod = (y >> 4) & 0xff;
  r2 = (col) & 0xff;
  g2 = (col >> 8) & 0xff;
  b2 = (col >> 16) & 0xff;

  if (ymod < 255) {

    /* top-left */
    if (xmod < 255) {
      a = (255 - xmod) * (255 - ymod) >> 8;
      a = a*ao>>8;

      if (a >= 255) {
        *pdst = col;
      } else if (a > 0) {
        b1 = *pdst;
        r1 = (b1) & 0xff;
        g1 = (b1 >> 8) & 0xff;
        b1 = (b1 >> 16) & 0xff;
        *pdst = (r1 + (a * (r2 - r1) >> 8))
              | (g1 + (a * (g2 - g1) >> 8)) << 8
              | (b1 + (a * (b2 - b1) >> 8)) << 16;
      }

    }

    /* top-right */
    if (xmod > 0) {
      a = xmod * (255 - ymod) >> 8;
      a = a*ao>>8;

      if (a >= 255) {
        pdst[1] = col;
      } else if (a > 0) {
        b1 = pdst[1];
        r1 = (b1) & 0xff;
        g1 = (b1 >> 8) & 0xff;
        b1 = (b1 >> 16) & 0xff;
        pdst[1] = (r1 + (a * (r2 - r1) >> 8))
                | (g1 + (a * (g2 - g1) >> 8)) << 8
                | (b1 + (a * (b2 - b1) >> 8)) << 16;
      }

    }

  }


  if (ymod <= 0) return;

  if (xmod < 255) {
    a  = (255 - xmod) * ymod >> 8;
    a = a * ao >> 8;

    if (a >= 255) {
      pdst[dstwidth] = col;
    } else if (a > 0) {
      b1 = pdst[dstwidth];
      r1 = (b1) & 0xff;
      g1 = (b1 >> 8) & 0xff;
      b1 = (b1 >> 16) & 0xff;
      pdst[dstwidth] = (r1 + (a * (r2 - r1) >> 8))
                     | (g1 + (a * (g2 - g1) >> 8)) << 8
                     | (b1 + (a * (b2 - b1) >> 8)) << 16;
    }

  }

  /* bottom-right */
  if (xmod > 0) {
    a = xmod * ymod >> 8;
    a = a * ao >> 8;

    if (a >= 255) {
      pdst[dstwidth + 1] = col;
    } else if (a > 0) {
      b1 = pdst[dstwidth + 1];
      r1 = (b1) & 0xff;
      g1 = (b1 >> 8) & 0xff;
      b1 = (b1 >> 16) & 0xff;
      pdst[dstwidth + 1] = (r1 + (a * (r2 - r1) >> 8))
                         | (g1 + (a * (g2 - g1) >> 8)) << 8
                         | (b1 + (a * (b2 - b1) >> 8)) << 16;
    }

  }


}


/******************************************************************************/

/**
 * draw a wuPixel
 *
 * @param self    - a bufferConsole_t structure
 * @param col     - color of the pixel in RGBA32
 * @param x       - x position on the buffer (fixed .12)
 * @param y       - y position on the buffer (fixed .12)
 * @param opacity - 0:opaque 255:invisible
 * @return -
 */
void bufferConsole_drawCircle(bufferConsole_t *const self,
                              uint32_t col, int opacity, int x0, int y0, int radius)
{
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    bufferConsole_drawPixel(self, col, x0 + x, y0 + y, opacity);
    bufferConsole_drawPixel(self, col, x0 + y, y0 + x, opacity);
    bufferConsole_drawPixel(self, col, x0 - y, y0 + x, opacity);
    bufferConsole_drawPixel(self, col, x0 - x, y0 + y, opacity);
    bufferConsole_drawPixel(self, col, x0 - x, y0 - y, opacity);
    bufferConsole_drawPixel(self, col, x0 - y, y0 - x, opacity);
    bufferConsole_drawPixel(self, col, x0 + y, y0 - x, opacity);
    bufferConsole_drawPixel(self, col, x0 + x, y0 - y, opacity);

    y += 1;
    err += 1 + 2 * y;
    if (2 * (err - x) + 1 > 0) {
      x -= 1;
      err += 1 - 2 * x;
    }
  }

}


/******************************************************************************/

/**
 * clear the current sub-buffer
 *
 * @param self - a bufferConsole_t struct
 * @param tex    - the 'char' texture
 * @param x      - x console position
 * @param y      - y console position
 * @param width  - width of the quad
 * @param height - height of the quad
 * @return -
 */
 #if 0
void bufferConsole_fillColArrQuad(bufferConsole_t *const self, uint8_t *tex,
                                  int x, int y, int width, int height)
{
  int in_off,out_off;
  int xsize,ysize,frame;
  int i,j, x0 = 0, y0 = 0;

  assert(self);
  if (NULL == tex) return;

  /* read the header */
  xsize = tex[0];
  ysize = tex[1];
  frame = tex[2];
  tex += 4 ;

  for (j = y; j < height+y; ++j,++y0)
    for (i = x; i < width+x; ++i,++x0) {
      in_off = y0*9 + x0;
      out_off = (self->size.X + self->pitch)*j + i;

      self->ccur_[out_off] = tex[in_off];
      self->cur_[out_off] = ' ';//y0 + '0';
    }

}
#endif


/******************************************************************************/

/**
 * draw a line into a buffer
 *
 * @param self - a bufferConsole_t struct
 * @param tex    - the 'char' texture
 * @param col    - the color
 * @param x    - x first position
 * @param y    - y first position
 * @param x2   - x second position
 * @param y2   - y second position
 * @return -
 *
 * @note - pas utilisé
 */
 #if 0
void bufferConsole_fillLine(bufferConsole_t *const self, char tex, uint8_t col,
                            int x, int y, int x2, int y2)
{
  int offset, len, xsign = 0, ysign = 0 ;
  int dx = x2 - x;
  int dy = y2 - y;

  assert(self);

  if (dx < 0) {
    dx = -dx ;
    xsign = 1 ;
  }
  if (dy < 0) {
    dy = -dy ;
    ysign = 1 ;
  }

  /* fixed point .16 */
  if (((len = dx) - dy) < 0) len = dy ;
  dx = (dx<<16)/len ;
  dy = (dy<<16)/len ;

  if (xsign) dx = -dx ;
  if (ysign) dy = -dy ;

  x <<= 16;
  y <<= 16;

  while (len-- > 0) {
    offset = (self->coord.X + self->pitch)*(y >> 16) + (x >> 16);
    self->cur_[offset] = tex;
    self->ccur_[offset] = col;
    x += dx ;
    y += dy ;
  }

}
#endif


/******************************************************************************/

/**
 * swap front/back buffers
 *
 * @param self - a bufferConsole_t structure
 * @return -
 */
void bufferConsole_swap(bufferConsole_t *const self)
{
  assert(self);
  if (self->cur == self->front) self->cur = self->back;
  else self->cur = self->front;
}


/**
 * Precompute a RGB555toCHARINFO Look-Up Table
 *
 * @param out -
 * @return -
 */
void charinfo_genPixTable(CHAR_INFO out[32*32*32])
{
  int x,y,z, i=0;
  CHAR_INFO *tmp;

  file_debug("const CHAR_INFO aRGB555toCHARINFO_LUT[32*32*32] = {\n  ");
  for (z = 0; z < 32; ++z)
    for (y = 0; y < 32; ++y)
      for (x = 0; x < 32; ++x) {
        tmp = out + z*32*32 + y*32 + x;
        tmp->Char.UnicodeChar = 0;
        tmp->Attributes = 0;
        color_toCHARINFO(RGBA32(x*8,y*8,z*8, 255),
                         (int8_t*)&tmp->Char.AsciiChar, (uint8_t*)&tmp->Attributes);
        file_debug("{0x%X,0x%02.2X}%s",
                   (uint8_t)tmp->Char.AsciiChar, tmp->Attributes, ((++i)%8)?",":",\n  ");
      }
  file_debug("};\n");
}




/******************************************************************************/

/**
 * draw current buffer into console
 *
 * @param self - a bufferConsole_t structure
 * @return -
 */
void bufferConsole_update(bufferConsole_t *const self)
{
  int i,j,off;
  assert(self);

  for (j = 0; j < self->size.Y; ++j)
    for (i = 0; i < self->size.X; ++i) {
      off = j*self->size.X + i;
      self->cur[off] = charinfo_fromRGB888(self->pixels[off]);
    }

  bufferConsole_memset(self->pixels, 0, self->size.X*self->size.Y*sizeof(*self->pixels));
  WriteConsoleOutput(self->hnd, (CHAR_INFO*)self->cur, self->size, self->coord, &self->region);
}











