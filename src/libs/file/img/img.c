
// standard materrial
#include <stdint.h>
#include <string.h>

// File
#include "img.h"



#include <stdio.h>
#include "../file.h"

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


// extern const uint8_t gCharUpper[256];
static const unsigned char aCharUpper[256] = {
  '\0',0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,'\t',
  '\n',0x0b,0x0c,'\r',0x0e,0x0f,0x10,0x11,0x12,0x13,
  0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,
  0x1e,0x1f, ' ', '!', '"', '#', '$', '%', '&','\'',
   '(', ')', '*', '+', ',', '-', '.', '/', '0', '1',
   '2', '3', '4', '5', '6', '7', '8', '9', ':', ';',
   '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E',
   'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
   'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
   'Z', '[','\\', ']', '^', '_', '`', 'A', 'B', 'C',
   'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
   'X', 'Y', 'Z', '{', '|', '}', '~',0x7f,0x80,0x81,
  0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,
  0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,
  0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
  0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,
  0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,
  0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,
  0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
  0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,
  0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,
  0xdc,0xdd,0xde,0xdf,0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,
  0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
  0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,
  0xfa,0xfb,0xfc,0xfd,0xfe,0xff
};


static const char *aIMG_ERR[16] = {
  "Unknow error",
  "No such file or directory",
  "I/O error",
  "Out of memory",
  "Invalid argument",
  "Too many open files",
  "File too large",
  "Illegal seek",
  "Not an image ",
  "Not a correct structure",
  "No data available",
  "Value too large for defined data type",
  "Invalid format",
  "Unknow error",
  "Unknow error",
  ""
};


/******************************************************************************/

/**
 * same string ? (match CAPS)
 * @arg   s1 first string
 * @arg   s2 second string
 * @return is same ?
 */
int strSAME(const char *s1, const char *s2)
{
  // assert
  if (NULL == s1 || NULL == s2) return 0;

  do {
    if (*s1!=*s2 && *s1!=aCharUpper[(unsigned char)*s2]) return 0;
    s1++;
    s2++;
  } while (0 != *(s1-1));

  return 1;
}

/******************************************************************************/

/**
 * This function returns a pointer to the 'n' last occurrence of '.'
 * If '.' is not found, the function returns a null pointer.
 *
 * @param  fname the in string
 * @param  n     number '.' to jump
 * @return if found:            nt-string with extension
 *          if not found:        NULL
 *          if '.' is last char: ""
 */
char *fname_extn(const char *fname, int n)
{
  char *s;
  int count = 0;
  size_t pos;

  // assert
  if (NULL == fname || '\0' == *fname) return NULL;

  pos = strlen(fname);
  s = (char*)fname + pos;

  while (pos-- > 0) {
    if ('.' == *s) {
      if (count >= n) break;
      count++;
    }
    s--;
  }

  if (s == fname) {
    if ('.' == *fname && n==count) return (char*)fname+1;
    return NULL;
  }

  return s + 1;
}


/******************************************************************************/

/**
 * callbacks events.
 * This event occurs when an img has been loaded.
 *
 * @param self - current structure
 * @return .
 */
void img_default_callback_onload(img_t *const self)
{
  if (NULL == self) return;
}

/**
 * callbacks events.
 * This event occurs when an error occurred during the loading of an img.
 *
 * @param self - current structure
 * @return .
 */
void img_default_callback_onerror(img_t *const self)
{
  if (NULL == self) return;
  if (0 == self->src.errors) return;

  int i;
  printf("img ErrCodes:\n");
  for (i = 0; i < 15; ++i)
    if (self->src.errors & (1 << i))
      printf("- %s\n", aIMG_ERR[i]);

}

/**
 * callbacks events.
 * The onabort event occurs when the loading of an img is aborted.
 *
 * @param This current structure
 * @return .
 */
void img_default_callback_onabort(img_t *const self)
{
  if (NULL == self) return;
}


/******************************************************************************/

/**
 * private function
 */
img_t *img_call_onload(img_t *const self)
{
  if (NULL == self) return self;
  if (NULL == self->onload) return self;
  self->onload(self);
  return self;
}

/**
 * private function
 */
img_t *img_call_onerror(img_t *const self)
{
  if (NULL == self) return self;
  if (NULL == self->onerror) return self;
  self->onerror(self);
  return self;
}

/**
 * private function
 */
img_t *img_call_onabort(img_t *const self)
{
  if (NULL == self) return self;
  if (NULL == self->onabort) return self;
  self->onabort(self);
  return self;
}


/******************************************************************************/

/**
 *
 */
int img_isnull(img_t *const self)
{
  if (NULL == self) return 1;
  return NULL == self->src.file;
}

/**
 *
 */
int img_pix_isnull(img_t *const self)
{
  if (NULL == self) return 1;
  return NULL == self->texImg.data;
}


/******************************************************************************/

/**
 * Return the sizeof pixels
 * @param img img
 * @return littleEndian:1  bigEndian:0
 */
unsigned img_littleEndian(void)
{
  union {
    uint32_t i;
    uint8_t c[4];
  } u = {1};
  return u.c[0];
}

/******************************************************************************/

/**
 * Return the sizeof pixels
 * @param img img
 * @return sizeof pixels data's
 */
unsigned img_fourcc(void)
{
  return img_littleEndian()? IMG_MAKEFOURCC('I','M','G','#'):
                             IMG_MAKEFOURCC('#','G','M','I');
}


unsigned img_32bits(void)
{
  return sizeof(void*)==4;
}
unsigned img_64bits(void)
{
  return sizeof(void*)==8;
}


/******************************************************************************/

/**
 * Return the sizeof pixels
 * @param self - img
 * @return sizeof pixels data's
 */
size_t img_pix_sizeof(img_t *const self)
{
  if (NULL == self) return 0;
  return (self->texImg.xcount * self->texImg.ycount * self->texImg.bpp)/8;
}


/******************************************************************************/

/**
 * Return the sizeof palette (if used)
 * @param self - img
 * @return sizeof palette data's
 */
size_t img_pal_sizeof(img_t *const self)
{
  if (NULL == self) return 0;
  //if (!(self->flags & (IMG_TYPE_IDX1|IMG_TYPE_IDX4|IMG_TYPE_IDX8))) return 0;

  return (self->pal.texImg.xcount * self->pal.texImg.bpp)/8;
  //return self->pal_sz * (self->pal_bpp/8);
}


/******************************************************************************/

/**
 *
 * @note incomplete type
 * - file or mem ?
 * - size ?
 */
img_t img_set(void)
{
  img_t ret;
  texImg_memset(&ret,0,sizeof(ret));
  ret.Hverif = IMG_VERIF;

  // callbacks by default
  ret.onload = img_default_callback_onload;
  ret.onerror = img_default_callback_onerror;
  ret.onabort = img_default_callback_onabort;
  //ret.flags = /*IMG_FORMAT_2DTEXTURE | */flags;
  return ret;
}


/******************************************************************************/

/**
 * Ouvre et charge un fichier.
 *
 * @note incomplete type
 * - file or mem ?
 */
img_t img_new(uint8_t format, uint8_t type,
              uint16_t width, uint16_t height, void *src, palette_t *pal)
{
  img_t ret;
  texImg_memset(&ret,0,sizeof(ret));
  ret.Hverif = IMG_VERIF;

  // callbacks by default
  ret.onload = img_default_callback_onload;
  ret.onerror = img_default_callback_onerror;
  ret.onabort = img_default_callback_onabort;

  // create texImg
  ret.texImg = texImg2D_new(TIMG_TEXTURE_2D, format,
                            width, height, format, type, NULL);
  if (texImg_isNull(&ret.texImg)) {
    ret.src.errors = IMG_EFORM;
    ret.onerror(&ret);
    return ret;
  }

  //ret.flags = flags;
  /*ret.texImg.xcount = width;
  ret.texImg.ycount = height;
  ret.texImg.zcount = 1;*/
  ret.naturalWidth = width;
  ret.naturalHeight = height;

  ret.src.file = src;
  //ret.naturalSrc = src;
  if (NULL != pal) {
    ret.pal = palette_dup(pal);
    if (palette_isError(&ret.pal)) {
      ret.src.errors = IMG_ENOMEM;
      ret.onerror(&ret);
      return ret;
    }
  }

  return ret;
}


/******************************************************************************/

/*!
 * Create a new img_t struct
 * @return
 */
img_t img_newFromFile(const char *str_file)
{
  img_t ret = img_set();
  ret.flags |= IMG_FLAG_DIRECTCOPY;
  tga_open(str_file, &ret, "rb");
  return ret;
}


/******************************************************************************/

/*!
 * Create a new img_t struct
 * @return
 */
img_t img_newFromStreamFile(const char *str_file)
{
  img_t ret = img_set();
  tga_open(str_file, &ret, "rs");
  return ret;
}


/******************************************************************************/

/*!
 * Export an img structure into a file
 * @return
 */
int img_toFile(img_t *const self, const char *str_file)
{
  return tga_open(str_file, self, "wb");
}


/******************************************************************************/

/*!
 * Create a new palette_t struct
 * @return
 */
void img_free(img_t *const self)
{
  tga_free(self);
}


/******************************************************************************/

/*!
 * Delete a img_t struct
 * @return
 */
void img_delete(img_t *const self)
{
  tga_close(self);
}


/******************************************************************************/

/*!
 * Delete a img_t struct
 * @return
 */
void img_close(img_t *const self)
{
  tga_close_nofree(self);
}


/******************************************************************************/

/**
 * Load a tga header from memory
 */
img_t img_fromMemTGA(const char *buff_tga, uint32_t flags)
{
  #if 0
  img_t ret = img_set(flags);
  tga_header_t header;
  const size_t sz_header = sizeof(header);

  // assert
  if (NULL == buff_tga) return ret;

  // Header.
  memcpy(&header, buff_tga, sz_header);

  /**< size */
  ret.Hverif = TGA_VERIF;
  ret.texImg.xcount = header.width;
  ret.texImg.ycount = header.height;
  ret.texImg.zcount = 1;
  ret.naturalWidth = header.width;
  ret.naturalHeight = header.height;

  // conserve le nombre de bpp
  ret.texImg.bpp = header.bpp;
  ret.texImg.target = TEXIMG_TEXTURE_2D;
  ret.bitsOffset = 0;
  ret.mode = 1;
  //ret.flags |= IMG_FORMAT_2DTEXTURE;

  if (0 != (header.img_descriptor & (1<<4))) ret.flags |= IMG_FLAG_XSWAP;
  if (0 == (header.img_descriptor & (1<<5))) ret.flags |= IMG_FLAG_YSWAP;


  switch (header.type) {
  case 1: // uncompressed color-mapped img (palette)
    break;

  case 2: // uncompressed true-color img
    if (header.bpp == 24) ret.flags |= IMG_TYPE_BGR888;
    else if (header.bpp == 32) ret.flags |= IMG_TYPE_BGRA32;
    break;

  case 3: // uncompressed black-and-white (grayscale) img
    ret.flags |= IMG_TYPE_R8;
    break;

  case 9: // run-length encoded color-mapped img (palette)
    break;

  case 10: // run-length encoded true-color img
    if (header.bpp == 24) ret.flags |= IMG_TYPE_BGR888 | IMG_FORMAT_RLE8;
    else if (header.bpp == 32) ret.flags |= IMG_TYPE_BGRA32 | IMG_FORMAT_RLE8;
    break;

  case 11: // run-length encoded black-and-white (grayscale) img
    ret.flags |= IMG_TYPE_R8 | IMG_FORMAT_RLE8;
    break;

  default:
    break;
  }

  return ret;
  #endif
}


/******************************************************************************/

/**
 * Open and load an img into a structure of type img_t.
 * Height types of images (.bmp, .tga, .dds, .gif, .jpg, .png)
 * may be loaded
 *
 * You can also load images from a data URI, such as this 1x1px tiny gif:
 * "data:image/gif;base64,R0lGODlhAQABAIAAAAUEBAAAACwAAAAAAQABAAACAkQBADs="
 */
/*img_t img_load(const char *in)
{
  img_t ret = img_new(8,8, NULL,NULL, IMG_FLAG_DIRECTCOPY);
  img_open(in, &ret, "rb");
  return ret;
}*/


/******************************************************************************/

/**
 * Ouvre et charge un fichier.
 */
int img_open(const char *in, img_t *out, const char *mode)
{
  char *str;

  // assert
  if (NULL == (str = fname_extn(in,0))) {
    debug_printf("no extension");
    return -1;
  }

  if (strSAME(str,"tga")) {
    return tga_open(in, out, mode);
  }

  return -1;
}



void img_print(img_t *const self)
{
  if (NULL == self) return;

  printf("verif: '%c','%c','%c','%c'\n", self->Hverif&0xff, (self->Hverif>>8)&0xff,
                                        (self->Hverif>>16)&0xff, (self->Hverif>>24)&0xff);
  printf("flags: %d\n", self->flags);
  printf("naturalFlags: %d\n", self->naturalFlags);
  printf("width: %d\n", self->texImg.xcount);
  printf("height: %d\n", self->texImg.ycount);
  printf("naturalWidth: %d\n", self->naturalWidth);
  printf("naturalHeight: %d\n", self->naturalHeight);
  printf("pitch: %d\n", self->pitch);
  printf("bitsPerPixel: %d\n", self->texImg.bpp);
  //printf("pal_bpc: %d\n", self->pal_bpc);
  printf("bitsOffset: %d\n", self->bitsOffset);

  printf("src: %d\n", self->src.file!=0);
  printf("pix: %d\n", self->texImg.data!=0);
  //printf("pal: %d\n", self->pal!=0);

  //printf("seek_pix: %d\n", self->seek_pix);
  //printf("seek_pal: %d\n", self->seek_pal);
  printf("pix_size: %d\n", self->texImg.size);
  //printf("pal_sz: %d\n", self->pal_sz);
}

/******************************************************************************/

/**
 * Update pixels container
 * @param self - an img_t structure
 * @return
 */
static int img_update_pix_container(img_t *const self)
{
  void *data_new;
  size_t size = img_pix_sizeof(self);
  if (size == 0) return 0;

  /* realloc the container */
  if (size < self->texImg.size) {
    if ((self->texImg.flags & TIMG_FLAG_ISALLOC) && self->texImg.data) {
      data_new = texImg_realloc(self->texImg.data, size);
      if (NULL == data_new) return IMG_ENOMEM;
      self->texImg.data = data_new;
    }
    self->texImg.size = size;
  }

  return 0;
}


/******************************************************************************/

/**
 * Convert an img_t to rgb888 format
 * @param self - a img_t structure
 * @return
 */
int img_convertToRGB565(img_t *const self)
{
  if (NULL == self) return -1;
  if (NULL == self->texImg.data) return -1;
/*
  switch ((self->flags&IMG_TYPE_MASK)>>IMG_TYPE_SHIFT) {
    case IMG_TYPE_IDX1: break;
    case IMG_TYPE_IDX4: break;
    case IMG_TYPE_IDX8: break;

    case IMG_TYPE_RGB444: break;
    case IMG_TYPE_RGBA4444: break;
    case IMG_TYPE_RGBT4444: break;
    case IMG_TYPE_BGR444: break;
    case IMG_TYPE_BGRA4444: break;
    case IMG_TYPE_BGRT4444: break;

    case IMG_TYPE_RGB555: break;
    case IMG_TYPE_RGBA5551: break;
    case IMG_TYPE_BGR555: break;
    case IMG_TYPE_BGRA5551: break;

    case IMG_TYPE_RGB565: break;
    case IMG_TYPE_BGR565:
      texImg_RGB565toBGR565(self->texImg.data, self->texImg.xcount*self->texImg.ycount);
      self->flags = (self->flags&~IMG_TYPE_MASK) | IMG_TYPE_RGB565;
      self->texImg.bpp = 16;
      printf("HELLO!\n\n");
      break;

    case IMG_TYPE_RGB888:  break;
    case IMG_TYPE_RGBA32:
      texImg_RGBA32toRGB565(self->texImg.data, self->texImg.xcount*self->texImg.ycount);
      self->flags = (self->flags&~IMG_TYPE_MASK) | IMG_TYPE_RGB565;
      self->texImg.bpp= 16;
      break;
    case IMG_TYPE_RGBT32: break;
    case IMG_TYPE_BGR888: break;
    case IMG_TYPE_BGRA32:
      texImg_RGBA32toRGB565(self->texImg.data, self->texImg.xcount*self->texImg.ycount);
      self->flags = (self->flags&~IMG_TYPE_MASK) | IMG_TYPE_BGR565;
      self->texImg.bpp= 16;
      img_update_pix_container(self);
      break;

    case IMG_TYPE_BGRT32: break;

    case IMG_TYPE_R8: break;
    case IMG_TYPE_G8: break;
    case IMG_TYPE_B8: break;
    case IMG_TYPE_A8: break;

    case IMG_TYPE_DXT1C: break;
    case IMG_TYPE_DXT1A: break;
    case IMG_TYPE_DXT3: break;
    case IMG_TYPE_DXT5: break;
    case IMG_TYPE_3DC: break;
    case IMG_TYPE_DEPTH24_STENCIL8: break;
    case IMG_TYPE_ATI2: break;
    case IMG_TYPE_BUFFER: break;
  }
*/
  return 0;
}

/******************************************************************************/

/**
 * Convert an img_t to rgb888 format
 * @param self - a img_t structure
 * @return
 */
int img_convertToBGR565(img_t *const self)
{
  if (NULL == self) return -1;
  if (NULL == self->texImg.data) return -1;
/*
  switch ((self->flags&IMG_TYPE_MASK)>>IMG_TYPE_SHIFT) {
    case IMG_TYPE_IDX1: break;
    case IMG_TYPE_IDX4: break;
    case IMG_TYPE_IDX8: break;

    case IMG_TYPE_RGB444: break;
    case IMG_TYPE_RGBA4444: break;
    case IMG_TYPE_RGBT4444: break;
    case IMG_TYPE_BGR444: break;
    case IMG_TYPE_BGRA4444: break;
    case IMG_TYPE_BGRT4444: break;

    case IMG_TYPE_RGB555: break;
    case IMG_TYPE_RGBA5551: break;
    case IMG_TYPE_BGR555: break;
    case IMG_TYPE_BGRA5551: break;

    case IMG_TYPE_RGB565: break;
    case IMG_TYPE_BGR565: break;

    case IMG_TYPE_RGB888:  break;
    case IMG_TYPE_RGBA32:
      texImg_RGBA32toRGB565(self->data, self->width*self->height);
      self->flags = (self->flags&~IMG_TYPE_MASK) | IMG_TYPE_RGB565;
      self->bitsPerPixel = 16;
      break;
    case IMG_TYPE_RGBT32: break;
    case IMG_TYPE_BGR888: break;
    case IMG_TYPE_BGRA32:
      texImg_RGBA32toRGB565(self->data, self->width*self->height);
      self->flags = (self->flags&~IMG_TYPE_MASK) | IMG_TYPE_BGR565;
      self->bitsPerPixel = 16;
      img_update_pix_container(self);
      break;

    case IMG_TYPE_BGRT32: break;

    case IMG_TYPE_R8: break;
    case IMG_TYPE_G8: break;
    case IMG_TYPE_B8: break;
    case IMG_TYPE_A8: break;

    case IMG_TYPE_DXT1C: break;
    case IMG_TYPE_DXT1A: break;
    case IMG_TYPE_DXT3: break;
    case IMG_TYPE_DXT5: break;
    case IMG_TYPE_3DC: break;
    case IMG_TYPE_DEPTH24_STENCIL8: break;
    case IMG_TYPE_ATI2: break;
    case IMG_TYPE_BUFFER: break;
  }
*/
  return 0;
}






/******************************************************************************/

/**
 * Convert an img_t to rgb888 format
 * @param self - a img_t structure
 * @return
 */
int img_convertToBGR888(img_t *const self)
{
  if (NULL == self) return -1;
  if (NULL == self->texImg.data) return -1;
/*
  switch ((self->flags&IMG_TYPE_MASK)>>IMG_TYPE_SHIFT) {
    case IMG_TYPE_IDX1: break;
    case IMG_TYPE_IDX4: break;
    case IMG_TYPE_IDX8: break;

    case IMG_TYPE_RGB444: break;
    case IMG_TYPE_RGBA4444: break;
    case IMG_TYPE_RGBT4444: break;
    case IMG_TYPE_BGR444: break;
    case IMG_TYPE_BGRA4444: break;
    case IMG_TYPE_BGRT4444: break;

    case IMG_TYPE_RGB555: break;
    case IMG_TYPE_RGBA5551: break;
    case IMG_TYPE_BGR555: break;
    case IMG_TYPE_BGRA5551: break;

    case IMG_TYPE_RGB565: break;
    case IMG_TYPE_BGR565: break;

    case IMG_TYPE_RGB888: break;
    case IMG_TYPE_RGBA32:
      texImg_RGBA32toRGB888(self->data, self->width*self->height);
      self->flags = (self->flags&~IMG_TYPE_MASK) | IMG_TYPE_RGB888;
      self->bitsPerPixel = 24;
      img_update_pix_container(self);
      break;

    case IMG_TYPE_RGBT32: break;
    case IMG_TYPE_BGR888: break;
    case IMG_TYPE_BGRA32:
      texImg_RGBA32toRGB888(self->data, self->width*self->height);
      self->flags = (self->flags&~IMG_TYPE_MASK) | IMG_TYPE_BGR888;
      self->bitsPerPixel = 24;
      img_update_pix_container(self);
      break;

    case IMG_TYPE_BGRT32: break;

    case IMG_TYPE_R8: break;
    case IMG_TYPE_G8: break;
    case IMG_TYPE_B8: break;
    case IMG_TYPE_A8: break;

    case IMG_TYPE_DXT1C: break;
    case IMG_TYPE_DXT1A: break;
    case IMG_TYPE_DXT3: break;
    case IMG_TYPE_DXT5: break;
    case IMG_TYPE_3DC: break;
    case IMG_TYPE_DEPTH24_STENCIL8: break;
    case IMG_TYPE_ATI2: break;
    case IMG_TYPE_BUFFER: break;
  }
*/
  return 0;
}

/******************************************************************************/

/**
 * Convert an img_t to rgb888 format
 * @param self - a img_t structure
 * @return
 */
int img_convertToRGBA32(img_t *const self)
{
  if (NULL == self) return -1;
  if (NULL == self->texImg.data) return -1;
/*
  switch ((self->flags&IMG_TYPE_MASK)>>IMG_TYPE_SHIFT) {
    case IMG_TYPE_IDX1: break;
    case IMG_TYPE_IDX4: break;
    case IMG_TYPE_IDX8: break;

    case IMG_TYPE_RGB444: break;
    case IMG_TYPE_RGBA4444: break;
    case IMG_TYPE_RGBT4444: break;
    case IMG_TYPE_BGR444: break;
    case IMG_TYPE_BGRA4444: break;
    case IMG_TYPE_BGRT4444: break;

    case IMG_TYPE_RGB555: break;
    case IMG_TYPE_RGBA5551: break;
    case IMG_TYPE_BGR555: break;
    case IMG_TYPE_BGRA5551: break;

    case IMG_TYPE_RGB565: break;
    case IMG_TYPE_BGR565: break;

    case IMG_TYPE_RGB888: break;
    case IMG_TYPE_RGBA32: break;
    case IMG_TYPE_RGBT32: break;
    case IMG_TYPE_BGR888: break;
    case IMG_TYPE_BGRA32:
      texImg_RGBA32toBGRA32(self->data, self->width*self->height);
      //texImg_RGBA32toRGB888(self->data, self->width*self->height);
      self->flags = (self->flags&~IMG_TYPE_MASK) | IMG_TYPE_RGBA32;
      self->bitsPerPixel = 32;
      break;

    case IMG_TYPE_BGRT32: break;

    case IMG_TYPE_R8: break;
    case IMG_TYPE_G8: break;
    case IMG_TYPE_B8: break;
    case IMG_TYPE_A8: break;

    case IMG_TYPE_DXT1C: break;
    case IMG_TYPE_DXT1A: break;
    case IMG_TYPE_DXT3: break;
    case IMG_TYPE_DXT5: break;
    case IMG_TYPE_3DC: break;
    case IMG_TYPE_DEPTH24_STENCIL8: break;
    case IMG_TYPE_ATI2: break;
    case IMG_TYPE_BUFFER: break;
  }
*/
  return 0;
}


/******************************************************************************/

/**
 * Convert an img_t to rgb888 format
 * @param self - a img_t structure
 * @return
 */
int img_fillColor(img_t *const self,
                  uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
  if (NULL == self) return -1;
  if (NULL == self->texImg.data) return -1;

  if (texImg_isNull(&self->pal.texImg)) {
    texImg_fillColor(&self->texImg, r,g,b,a);
  } else {
    texImg_fillColor(&self->pal.texImg, r,g,b,a);
  }

  return 0;
}

/******************************************************************************/

/**
 * Convert an img_t to rgb888 format
 * @param self - a img_t structure
 * @return
 */
int img_fillIndex(img_t *const self, uint8_t idx)
{
  if (NULL == self) return -1;
  if (NULL == self->texImg.data) return -1;

  if (!texImg_isNull(&self->pal.texImg)) {
    texImg_fillIndex(&self->texImg, idx);
  }

  return 0;
}



