
// standard materrial
#include <stdint.h>

// image
#include "img.h"
#include "tga.h"

// DEBUG local, debug_printf("assert error!");
#include "../file.h"

#include <stdio.h>


/*
#define LOCAL_DEBUG 1
#define LOCAL_LOGFILE 0

#ifdef GLOBAL_DEBUG_NO_LOGFILE
#undef LOCAL_LOGFILE
#define LOCAL_LOGFILE 0
#endif
#ifdef GLOBAL_NO_DEBUG
#undef LOCAL_DEBUG
#define LOCAL_DEBUG 0
#undef LOCAL_LOGFILE
#define LOCAL_LOGFILE 0
#endif

// set LOCAL_DEBUG to 0 and the compiller erase all debug logs
#define debug_printf(fmt, ...) \
  do { \
    if (LOCAL_DEBUG) { \
      fprintf(stderr, "EXCEPT: %s() line:%d : " fmt"\n", __func__, __LINE__,  ##__VA_ARGS__); \
      if (LOCAL_LOGFILE) file_debug("EXCEPT: %s() line:%d : " fmt"\n", __func__, __LINE__,  ##__VA_ARGS__); \
    } \
  } while (0)

*/


/* prototypes */
int tga_read(img_t *const self);
int tga_write(img_t *const self);


/******************************************************************************/

/*
 (Color map type (field 2))
char pal;
    - 0 if image file contains no color map
    - 1 if present
    - 2–127 reserved by Truevision
    - 128–255 available for developer use

Image type (field 3)
char type;
    - 0 no image data is present
    - 1 uncompressed color-mapped image
    - 2 uncompressed true-color image
    - 3 uncompressed black-and-white (grayscale) image
    - 9 run-length encoded color-mapped image
    - 10 run-length encoded true-color image
    - 11 run-length encoded black-and-white (grayscale) image
*/
/*
  info.bpp       = header[16];
  info.bytes     = (info.bpp + 7) / 8;
  info.alphaBits = header[17] & 0x0f; // Just the low 4 bits
  info.flipHoriz = (header[17] & 0x10) ? 1 : 0;
  info.flipVert  = (header[17] & 0x20) ? 0 : 1;
*/

/******************************************************************************/

/**
 *
 */
void tga_debug_header(tga_header_t *header)
{
  if (NULL == header) return;

  printf("id_size: %d\n", header->id_size);
  printf("pal: %d\n", header->pal);
  printf("type: %d\n", header->type);
  printf("pal_idx: %d\n", header->pal_idx);
  printf("pal_size: %d\n", header->pal_size);
  printf("pal_bpc: %d\n", header->pal_bpc);
  printf("xo: %d\n", header->xo);
  printf("yo: %d\n", header->yo);
  printf("width: %d\n", header->width);
  printf("height: %d\n", header->height);
  printf("bpp: %d\n", header->bpp);
  printf("image_descriptor: %d\n\n", header->image_descriptor);
}


/******************************************************************************/

/**
 * File and buffer polymorphic read.
 */
int img_jump(img_t *in, int offset, int pos)
{
  return offset;
  in = in; /* This line for avoid useless warnings*/
  pos = pos; /* This line for avoid useless warnings*/
}
int img_get_pixel8(img_t *in, char *out, int pos)
{
  char *src = (char*)in->src.file + pos;
  int cumul = *src;
  *out = cumul;
  return cumul;
}
int img_get_pixel16(img_t *in, char *out, int pos)
{
  char *src = (char*)in->src.file + pos;
  int cumul = *(src++);
  cumul |= *(src)<<8;
  *(short*)out = cumul;
  return cumul;
}
int img_get_pixel24(img_t *in, char *out, int pos)
{
  char *src = (char*)in->src.file + pos;
  int cumul = *(src++);
  cumul |= *(src++)<<8;
  cumul |= *(src)<<16;
  *(int*)out = cumul;
  return cumul;
}
int img_get_pixel32(img_t *in, char *out, int pos)
{
  char *src = (char*)in->src.file + pos;
  int cumul = *(src++);
  cumul |= *(src++)<<8;
  cumul |= *(src++)<<16;
  cumul |= *(src)<<24;
  *(int*)out = cumul;
  return cumul;
}


int img_fjump(img_t *in, int offset, int pos)
{
  if (0 != stream_seek(&in->src, offset, SEEK_CUR)) return IMG_ESPIPE;
  return 0;
  pos = pos;  /* This line for avoid useless warnings*/
}
int img_fget_pixel8(img_t *in, char *out, int pos)
{
  int cumul = stream_getc(&in->src);
  *out = cumul;
  return cumul;
  pos = pos;  /* This line for avoid useless warnings*/
}
int img_fget_pixel16(img_t *in, char *out, int pos)
{
  int cumul = stream_getc(&in->src);
  cumul |= stream_getc(&in->src)<<8;
  *(short*)out = cumul;
  return cumul;
  pos = pos;  /* This line for avoid useless warnings*/
}
int img_fget_pixel24(img_t *in, char *out, int pos)
{
  int cumul = stream_getc(&in->src);
  cumul |= stream_getc(&in->src)<<8;
  cumul |= stream_getc(&in->src)<<16;
  *(int*)out = cumul;
  return cumul;
  pos = pos;  /* This line for avoid useless warnings*/
}
int img_fget_pixel32(img_t *in, char *out, int pos)
{
  int cumul = stream_getc(&in->src);
  cumul |= stream_getc(&in->src)<<8;
  cumul |= stream_getc(&in->src)<<16;
  cumul |= stream_getc(&in->src)<<24;
  *(int*)out = cumul;
  return cumul;
  pos = pos;  /* This line for avoid useless warnings*/
}


/* Set some creepy functions */
int img_set_getters(img_t *const self,
                    int (**get_pixel)(img_t*,char*,int),
                    int (**get_flag)(img_t*, char*, int),
                    int (**jump)(img_t *, int, int))
{
  int step = self->texImg.bpp / 8;

  switch (step) {
  case 1:
    *get_pixel = img_fget_pixel8;
    *get_flag = img_fget_pixel8;
    *jump = img_fjump;
    break;

  case 2:
    *get_pixel = img_fget_pixel16;
    *get_flag = img_fget_pixel8;
    *jump = img_fjump;
    break;

  case 3:
    *get_pixel = img_fget_pixel24;
    *get_flag = img_fget_pixel8;
    *jump = img_fjump;
    break;

  case 4:
    *get_pixel = img_fget_pixel32;
    *get_flag = img_fget_pixel8;
    *jump = img_fjump;
    break;

  default:
    return -1;
  }

  return 0;
}


/******************************************************************************/

/**
 * Open and read/write a file/buffer
 * @param iout file or buffer
 * @param img an img stucture to import or export
 * @param mode  file:"rb"/"wb"   memory:"rm"/"wm"
 * @return errors
 */
int tga_open(const char *inout, img_t *img, const char *format)
{
  img_t tmp;

  if (NULL == img) {
    tmp = img_set();
    img = &tmp;
    tmp.Hverif = TGA_VERIF;
  }

  // assert
  if (NULL == inout || NULL == format) {
    img->src.errors = IMG_EINVAL;
    img_call_onerror(img);
    return IMG_EINVAL;
  }

  // read mode
  if ('r' == format[0]) {
    if (img != &tmp) {
      //*img = tmp;
      img->Hverif = TGA_VERIF;
    }

  // write mode
  } else if ('w' == format[0]) {

    /**< In write mode we need a structure */
    if (img == &tmp) {
      img->src.errors = IMG_EINVAL | IMG_ENODATA;
      img_call_onerror(img);
      return img->src.errors;
    }
    /**< @todo tester tous les autres PNG_VERIF || JPG_VERIF ... */
    if (IMG_VERIF != img->Hverif && TGA_VERIF != img->Hverif) {
      img->src.errors = IMG_ENOIMG;
      img_call_onerror(img);
      return img->src.errors;
    }

    //img->pix = (char*)inout;
  } else {
    img->src.errors = IMG_EINVAL;
    img_call_onerror(img);
    return img->src.errors;
  }


  /* File mode. */
  if ('b' == format[1] || '\0' == format[1] || 's' == format[1]) {

    if (NULL == (img->src.file = file_open(inout, format))) {
      img->src.errors = IMG_ENOENT;
      return img->src.errors;
    }
    img->src.flags = STREAM_FLAG_FILE | STREAM_FLAG_TEXT;
    if ('s' != format[1]) img->src.flags |= STREAM_FLAG_DIRECTCOPY;

  /* Buffer mode. */
  } else if ('m' == format[1]) {
    img->src.file = (void*)inout;
    img->src.flags = STREAM_FLAG_BUFFER | STREAM_FLAG_TEXT;
    if ('s' != format[2]) img->src.flags |= STREAM_FLAG_DIRECTCOPY;

  /* Args error */
  } else {
    img->src.errors = IMG_EINVAL;
    return img->src.errors;
  }


  // Mode fichier.
  /*if ('b' == format[1] || '\0' == format[1]) {
    if (NULL == (img->src.file = file_open(inout, format))) {
      img->src.errors = IMG_ENOENT;
      img_call_onerror(img);
      return img->src.errors;
    }

  // Mode buffer mémoire.
  } else if ('m' == format[1]) {
    img->src.file = (void*)inout;
  }*/








  // Accès lecture ou écriture.
  switch (*(uint16_t*)format) {

  case TGA_MAKETWOCC('r','s'):
  case TGA_MAKETWOCC('r','m'):
  case TGA_MAKETWOCC('r','b'):
  case TGA_MAKETWOCC('r','\0'):
    //printf("call tga_read\n");
    img->src.errors = tga_read(img);
    if (0 != img->src.errors) {
      img_call_onerror(img);
      stream_close(&img->src);
      return img->src.errors;
    }
    break;

    //if (in == img_pix) img->pix = img_pix;
    //else /*tga_mread(img, img_pix, texture_flags)*/;
    //break;
  case TGA_MAKETWOCC('w','s'):
  case TGA_MAKETWOCC('w','m'):
  case TGA_MAKETWOCC('w','b'):
  case TGA_MAKETWOCC('w','\0'):
    img->src.errors = tga_write(img);
    if (0 != img->src.errors) {
      img_call_onerror(img);
      stream_close(&img->src);
      return img->src.errors;
    }
    break;


  default: // unknow flag
    img->src.errors = IMG_EINVAL;
    stream_close(&img->src);
    return img->src.errors;
  }

  /* close the file if: */
  /* - is file mode */
  /* - is DIRECTCOPY */
  if (img->flags&IMG_FLAG_DIRECTCOPY) {
    stream_close(&img->src);
  }

  return 0;
}


/******************************************************************************/

/**
 * refactored code for close/free functions
 */
static int tga_clean(img_t *const self)
{
  if (NULL == self) return IMG_ENODATA;

  self->aborts = 0;
  if (TGA_VERIF != self->Hverif) self->aborts |= IMG_ENOSTR;
  if (NULL == self->src.file) self->aborts |= IMG_ENODATA;
  return self->aborts;
}


/**
 * free pixels (if allocated)
 */
img_t *tga_free(img_t *const self)
{
  if (0 != tga_clean(self)) return img_call_onabort(self);
  if ((self->texImg.flags & TIMG_FLAG_ISALLOC) && self->texImg.data) {
    texImg_realloc(self->texImg.data,0);
    self->texImg.data = NULL;
  }
  palette_delete(&self->pal);
  return self;
}

/******************************************************************************/

/**
 * close file and free pixels (if allocated)
 */
img_t *tga_close(img_t *const self)
{
  if (0 != tga_clean(self)) return img_call_onabort(self);
  /*if ((self->texImg.flags & TIMG_FLAG_ISALLOC) && self->texImg.data) {
    texImg_realloc(self->texImg.data,0);
    self->texImg.data = NULL;
  }*/
  texImg_delete(&self->texImg);
  stream_close(&self->src);

  return self;
}


/******************************************************************************/

/**
 * close file (without free pixels)
 */
img_t *tga_close_nofree(img_t *const self)
{
  if (0 != tga_clean(self)) return img_call_onabort(self);
  stream_close(&self->src);
  return self;
}






