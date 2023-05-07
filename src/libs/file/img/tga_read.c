
// standard materrial
#include <stdint.h>
#include <string.h>

// image
#include "img.h"
#include "tga.h"

// DEBUG local, debug_printf("assert error!");
#include "../file.h"



/* proto */
int img_set_getters(img_t *const self,
                    int (**get_pixel)(img_t*,char*,int),
                    int (**get_flag)(img_t*, char*, int),
                    int (**jump)(img_t*, int, int));


/******************************************************************************/

/**
 *
 */
static int tga_headerToImage(tga_header_t *header, img_t *img)
{
  if (NULL == header) return IMG_ENODATA;
  if (NULL == img) return IMG_ENODATA;

  /**< size */
  img->texImg.xcount = header->width;
  img->texImg.ycount = header->height;
  img->naturalWidth = header->width;
  img->naturalHeight = header->height;

  // conserve le nombre de bpp
  img->texImg.target = TIMG_TEXTURE_2D;
  img->texImg.bpp = header->bpp;
  img->bitsOffset = 0;

  img->pal.texImg.bpp = header->pal_bpc;
  img->pal.texImg.xcount = header->pal_size;

  if (0 != (header->image_descriptor & (1<<4))) img->flags |= IMG_FLAG_XSWAP;
  if (0 == (header->image_descriptor & (1<<5))) img->flags |= IMG_FLAG_YSWAP;

  switch (header->type) {

  case 9: /* Runlength encoded 8bits palette */
    img->texImg.iformat = TIMG_COMPRESSED_INDEX_RLE;
    img->texImg.type = TIMG_UBYTE;

    img->pal.texImg.iformat = TIMG_BGR;
    img->pal.texImg.type = TIMG_UBYTE;
    img->pal.texImg.target = TIMG_TEXTURE_1D;
    break;

  case 1: /* Uncompressed, 8bits palette */
    img->texImg.iformat = TIMG_INDEX;
    img->texImg.type = TIMG_UBYTE;
    img->texImg.size = img->texImg.xcount * img->texImg.ycount * (img->texImg.bpp/8);

    img->pal.texImg.iformat = TIMG_BGRA;
    img->pal.texImg.type = TIMG_UBYTE;
    img->pal.texImg.target = TIMG_TEXTURE_1D;
    break;

  case 10: /* Runlength encoded, BGR 16-24-32 bits */
    img->texImg.iformat = TIMG_COMPRESSED_BGR_RLE;
    if (header->bpp == 16) {
      img->texImg.type = TIMG_USHORT_5_6_5;
    } else if (header->bpp == 24) {
      img->texImg.type = TIMG_UBYTE;
    } else if (header->bpp == 32) {
      img->texImg.iformat = TIMG_COMPRESSED_BGRA_RLE;
      img->texImg.type = TIMG_UBYTE;
    }
    break;

  case 2: /* Uncompressed, BGR 16-24-32 bits */
    img->texImg.iformat = TIMG_BGR;
    img->texImg.size = img->texImg.xcount * img->texImg.ycount * (img->texImg.bpp/8);
    if (header->bpp == 16) {
      img->texImg.type = TIMG_USHORT_5_6_5;
    } else if (header->bpp == 24) {
      img->texImg.type = TIMG_UBYTE;
    } else if (header->bpp == 32) {
      img->texImg.iformat = TIMG_BGRA;
      img->texImg.type = TIMG_UBYTE;
    }
    break;

  case 11: /* Runlength encoded, Grayscale 8 bits */
    img->texImg.iformat = TIMG_COMPRESSED_RED_RLE;
    img->texImg.type = TIMG_UBYTE;
    break;
  case 3: /* Uncompressed, Grayscale 8 bits */
    img->texImg.size = img->texImg.xcount * img->texImg.ycount * (img->texImg.bpp/8);
    img->texImg.iformat = TIMG_RED;
    img->texImg.type = TIMG_UBYTE;
    break;

  case 32: /* Compressed color-mapped data, using Huffman, Delta, and
              runlength encoding. */
  case 33: /* Compressed color-mapped data, using Huffman, Delta, and
              runlength encoding.  4-pass quadtree-type process. */
  case 0:  /* No image data included. */
  default:
    texImg_memset(&img->texImg, 0, sizeof(img->texImg));
    texImg_memset(&img->pal.texImg, 0, sizeof(img->pal.texImg));
    img->src.errors |= IMG_EFORM;
    return img->src.errors;
  }

  /* Same internat and external format */
  img->texImg.format = img->texImg.iformat;
  img->pal.texImg.format = img->pal.texImg.iformat;
  return 0;
}


/******************************************************************************/

/**
 * Lecture données fichier tga.
 *
 *  00000000
 *  |||||||+-- 0:normal 1:packed
 *  +++++++--- number pix to copy (0 to 127)
 *
 * @return errors
 */
static int tga_read_rle_pix(img_t *const self)
{
  int val, packet, packet_sz, sz;
  int num_pix;
  char *out;
  int (*get_pixel)(img_t*, char*, int);
  int (*get_flag)(img_t*, char*, int);
  int (*jump)(img_t*, int, int);
  int step = self->texImg.bpp / 8;
  int pos = 0;

  printf ("step: %d\n", step);

  /* set function pointers */
  img_set_getters(self, &get_pixel, &get_flag, &jump);

  num_pix = self->texImg.xcount * self->texImg.ycount;
  out = self->texImg.data;


  for (sz = 0; num_pix > 0; ) {
    packet    = get_flag(self, out++, pos++);
    packet_sz = (packet & 0x7f) + 1;
    num_pix -= packet_sz;

    // packed
    if (packet & 0x80) {
      val = get_pixel(self, out++, pos);
      pos += step;

      if (packet_sz > 0) {
        packet_sz *= step;
        memset(out, val, packet_sz);
        out += packet_sz;
        pos += packet_sz;
        sz++;
      }

    // Normal
    } else
    while (packet_sz-- > 0) {
      get_pixel(self,out,pos);
      out += step;
      pos += step;
      sz++;
    }

  }

  return pos;
}


/******************************************************************************/

/**
 * Lecture données fichier tga.
 * @return errors
 */
static int tga_read_pix(img_t *const self)
{
  size_t tmp_size,src_size;

  /* Assert */
  if (NULL == self) return IMG_ENODATA;
  if (TGA_VERIF != self->Hverif) return IMG_ENOSTR;
  if (NULL == self->src.file) return IMG_ENODATA;
  //if (0 == self->size) return IMG_ENODATA;

  if (self->texImg.size == 0) return 0;
  if (self->texImg.size > IMG_PIXEL_SIZE_MAX) return IMG_EFBIG;

  if (NULL == self->texImg.data) {
    if (NULL == (self->texImg.data = texImg_realloc(NULL,self->texImg.size))) return IMG_ENOMEM;
    self->texImg.flags |= TIMG_FLAG_ISALLOC;
  }


  /* file */
  //if (0 == self->mode) {

    /*
    data_seek = file_tell(self->src);
    if (self->tmp_sz != file_read(self->texImg.data, 1, self->tmp_sz, self->src)) return IMG_EIO;
    self-> = data_seek;
    */
    /* Read pixelmap data */
    if (0 != stream_seek(&self->src, self->src.fbeg, SEEK_SET)) goto error_spipe;
    src_size = stream_read(self->texImg.data, 1, self->texImg.size, &self->src);

/*
    if (img_pix_sizeof(self)
    img->naturalWidth = header->width;
    img->naturalHeight = header->height;
*/

    if (self->texImg.size != src_size) {
      if (self->texImg.flags & TIMG_FLAG_ISALLOC) {
        texImg_realloc(self->texImg.data,0);
        self->texImg.data = NULL;
      }
      return IMG_EIO;
    }
    if (0 != stream_seek(&self->src, self->src.fbeg, SEEK_SET)) goto error_spipe;

    //self->data_seek = data_seek;

  /* mem */
  //} else {
  //  memcpy(self->texImg.data, self->src, self->texImg.size);
  //}


  /* good end */
  return 0;

  /* bad end */
  error_spipe:
  if (self->texImg.flags & TIMG_FLAG_ISALLOC) {
    texImg_realloc(self->texImg.data,0);
    self->texImg.data = NULL;
  }
  return IMG_ESPIPE;
}


/******************************************************************************/

/**
 * Read pixels
 * @return errors
 */
static int tga_read_pal(img_t *const self)
{
  //size_t tmp_size;
  //long seek_pal;

  /* Assert */
  if (NULL == self) return IMG_ENODATA;
  if (NULL == self->src.file) return IMG_ENODATA;
  //if (0 == (self->flags & (IMG_TYPE_IDX8 | IMG_TYPE_IDX4 | IMG_TYPE_IDX1))) return 0;

  /* verif */
  if (texImg_isNull(&self->pal.texImg)) return 0;

  /* Palette allocation */
  //tmp_size = img_pal_sizeof(self);
  if (self->pal.texImg.xcount > PALETTE_COUNT_MAX) return IMG_EFBIG;

  //self->pal = palette_new(self->pal.texImg.data, 0/*PAL_TYPE_BGRA32*/, self->pal.texImg.xcount);
  self->pal = palette_new(TIMG_BGR, self->pal.texImg.xcount, TIMG_BGR, TIMG_UBYTE, NULL);
  if (palette_isError(&self->pal)) {
    self->src.errors |= IMG_ENOMEM;
    return self->src.errors;
  }
  /*
  if (NULL == self->pal.texImg.data) {
    if (NULL == (self->pal.texImg.data = texImg_realloc(0,tmp_size)))
      return IMG_ENOMEM;
    self->flags |= IMG_FORMAT_PAL_MALLOC;
  }*/

  /* Read colormap data */
  //seek_pal = file_tell(self->src);
  if (self->pal.texImg.size != stream_read(self->pal.texImg.data, 1, self->pal.texImg.size, &self->src)) {
    palette_delete(&self->pal);
    /*
    if (self->flags & IMG_FORMAT_PAL_MALLOC) {
      texImg_realloc(self->pal.texImg.data,0);
      self->pal.texImg.data = NULL;
    }*/
    self->src.errors |= IMG_EIO;
    return IMG_EIO;
  }

  //self->seek_pal = seek_pal;
  return 0;
}


/******************************************************************************/

/**
 * Lecture fichier tga.
 * @return errors
 */
int tga_read(img_t *const self)
{
  int error = 0;
  tga_header_t header;

  /* Assert */
  if (NULL == self) return IMG_ENODATA;
  if (TGA_VERIF != self->Hverif) return IMG_ENOSTR;
  if (NULL == self->src.file) return IMG_ENODATA;

  //printf("tga_read assert ok\n");
  //printf("lecture %d\n", self->mode);

  /* Header */
  if (0 != stream_seek(&self->src, 0, SEEK_SET)) return IMG_ESPIPE;
  if (sizeof(header) != stream_read(&header, 1, sizeof(header), &self->src)) return IMG_EIO;
  //tga_debug_header(&header);

  // get header
  if (0 != (error = tga_headerToImage(&header, self))) return error;

  /* load palette */
  self->pal.src.fbeg = stream_tell(&self->pal.src);
  if (0 != stream_seek(&self->pal.src, 0, SEEK_END)) return IMG_ESPIPE;
  self->pal.texImg.size = stream_tell(&self->pal.src) - self->pal.src.fbeg;
  if (0 != stream_seek(&self->pal.src, self->pal.src.fbeg, SEEK_SET)) return IMG_ESPIPE;
  /* direct copy or stream */
  if (self->flags & IMG_FLAG_DIRECTCOPY) {
    if (0 != (error = tga_read_pal(self))) return error;
  }


  /* load pixels */
  self->src.fbeg = stream_tell(&self->src);
  if (0 != stream_seek(&self->src, 0, SEEK_END)) return IMG_ESPIPE;
  self->texImg.size = stream_tell(&self->src) - self->src.fbeg;
  if (0 != stream_seek(&self->src, self->src.fbeg, SEEK_SET)) return IMG_ESPIPE;
  /* direct copy or stream */
  if (self->flags & IMG_FLAG_DIRECTCOPY) {
    if (0 != (error = tga_read_pix(self))) return error;
  }

  return error;
}





