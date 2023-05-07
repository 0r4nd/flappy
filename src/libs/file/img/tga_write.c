
// standard materrial
#include <stdint.h>

// image
#include "img.h"
#include "tga.h"

// DEBUG local, debug_printf("assert error!");
#include "../file.h"




/******************************************************************************/

/**
 *
 */
static int tga_imageToHeader(img_t *const self, tga_header_t *header)
{
  // assert
  //if (self->texImg.xcount > 32767 || self->texImg.ycount > 32767) return IMG_EFBIG;
  if (self->texImg.xcount <= 0 || self->texImg.ycount <= 0) return IMG_EINVAL;

  texImg_memset(header, 0, sizeof(tga_header_t));

  // Le header.
  header->width = self->texImg.xcount;
  header->height = self->texImg.ycount;

  /* 32 bpp */
  if (self->texImg.bpp == 32) {

    if (self->texImg.format == TIMG_RGBA) {

      switch (self->texImg.type) {
        case TIMG_UBYTE:
        case TIMG_UINT_8_8_8_8:
        case TIMG_UINT_8_8_8_8_REV:
          header->type = 2;
          header->bpp = 32;
          header->image_descriptor = 0x8;
          break;
        case TIMG_UINT_10_10_10_2: break;
        case TIMG_UINT_2_10_10_10_REV: break;
      }

    } else if (self->texImg.format == TIMG_BGRA) {

      switch (self->texImg.type) {
        case TIMG_UBYTE:
        case TIMG_UINT_8_8_8_8:
        case TIMG_UINT_8_8_8_8_REV:
          header->type = 2;
          header->bpp = 32;
          header->image_descriptor = 0x8;
          break;
        case TIMG_UINT_10_10_10_2: break;
        case TIMG_UINT_2_10_10_10_REV: break;
      }

    } else if (self->texImg.format == TIMG_ABGR) {

      switch (self->texImg.type) {
        case TIMG_UBYTE:
        case TIMG_UINT_8_8_8_8:
          header->type = 2;
          header->bpp = 32;
          header->image_descriptor = 0x8;
          break;
        case TIMG_UINT_10_10_10_2: break;
      }

    } else if (self->texImg.format == TIMG_COMPRESSED_RGBA_RLE) {

      switch (self->texImg.type) {
        case TIMG_UBYTE:
        case TIMG_UINT_8_8_8_8:
          header->type = 10;
          header->bpp = 32;
          header->image_descriptor = 0x8;
          break;
        case TIMG_UINT_10_10_10_2: break;
      }

    } else if (self->texImg.format == TIMG_COMPRESSED_BGRA_RLE) {

      switch (self->texImg.type) {
        case TIMG_UBYTE:
        case TIMG_UINT_8_8_8_8:
          header->type = 10;
          header->bpp = 32;
          header->image_descriptor = 0x8;
          break;
        case TIMG_UINT_10_10_10_2: break;
      }

    }


  /* 24 bpp */
  } else if (self->texImg.bpp == 24) {

    switch (self->texImg.format) {
      case TIMG_RGB:
      if (self->texImg.type == TIMG_UBYTE) {
        header->type = 2;
        header->bpp = 24;
      }
      break;

      case TIMG_BGR:
      if (self->texImg.type == TIMG_UBYTE) {
        header->type = 2;
        header->bpp = 24;
      }
      break;

      case TIMG_COMPRESSED_RGB_RLE:
      if (self->texImg.type == TIMG_UBYTE) {
        header->type = 10;
        header->bpp = 24;
      }
      break;

      case TIMG_COMPRESSED_BGR_RLE:
      if (self->texImg.type == TIMG_UBYTE) {
        header->type = 10;
        header->bpp = 24;
      }
      break;
    }


  /* 16 bpp */
  } else if (self->texImg.bpp == 16) {

    switch (self->texImg.format) {
      case TIMG_RGB:
        switch (self->texImg.type) {
          case TIMG_USHORT_5_6_5:
          case TIMG_USHORT_5_6_5_REV:
            header->type = 2;
            header->bpp = 16;
            header->image_descriptor = 0x1;
            break;
        }
        break;

      case TIMG_BGR:
        switch (self->texImg.type) {
          case TIMG_USHORT_5_6_5:
          case TIMG_USHORT_5_6_5_REV:
            header->type = 2;
            header->bpp = 16;
            header->image_descriptor = 0x1;
            break;
        }
        break;

      case TIMG_RGBA:
        switch (self->texImg.type) {
          case TIMG_USHORT_5_5_5_1: break;
          case TIMG_USHORT_1_5_5_5_REV:
          case TIMG_USHORT_4_4_4_4: break;
          case TIMG_USHORT_4_4_4_4_REV: break;
        }
        break;

      case TIMG_BGRA:
        switch (self->texImg.type) {
          case TIMG_USHORT_5_5_5_1: break;
          case TIMG_USHORT_1_5_5_5_REV:
          case TIMG_USHORT_4_4_4_4: break;
          case TIMG_USHORT_4_4_4_4_REV: break;
        }
        break;

      case TIMG_ABGR:
        switch (self->texImg.type) {
          case TIMG_USHORT_5_5_5_1: break;
          case TIMG_USHORT_4_4_4_4: break;
        }
        break;

      case TIMG_COMPRESSED_RGB_RLE:
      break;

      case TIMG_COMPRESSED_BGR_RLE:
      break;
    }


  /* 8 bpp */
  } else if (self->texImg.bpp == 8) {

    switch (self->texImg.format) {
      case TIMG_RGB:
        switch (self->texImg.type) {
          case TIMG_UBYTE_3_3_2: break;
          case TIMG_UBYTE_2_3_3_REV: break;
        }
      break;

      case TIMG_INDEX:
        switch (self->texImg.type) {
          case TIMG_UBYTE:
          case TIMG_BYTE:
            header->pal = 1;
            header->type = 1;
            header->bpp = 8;
            header->image_descriptor = 0x8;
            header->pal_size = self->pal.texImg.xcount;
            header->pal_bpc = self->pal.texImg.bpp;
            //printf("INDEX %u %u %u\n", header->pal_size, header->pal_bpc,
            //         header->image_descriptor);
            break;
        }
      break;

      case TIMG_COMPRESSED_INDEX_RLE:
        switch (self->texImg.type) {
          case TIMG_UBYTE:
          case TIMG_BYTE:
            header->pal = 1;
            header->type = 32;
            header->bpp = 8;
            header->image_descriptor = 0x8;
            header->pal_size = self->pal.texImg.xcount;
            header->pal_bpc = self->pal.texImg.bpp;
            break;
        }
      break;
    }

  }


#if 0
  switch (self->texImg.type/* & (IMG_TYPE_MASK | IMG_FORMAT_RLE8)*/) {
  case TIMG_USHORT_5_6_5:
  case TIMG_USHORT_5_6_5:
    header->type = 2;
    header->bpp = 16;
    header->image_descriptor |= 0x1;
    break;
  case IMG_TYPE_RGB565|IMG_FORMAT_RLE8:
  case IMG_TYPE_BGR565|IMG_FORMAT_RLE8:
    header->type = 10;
    header->bpp = 16;
    header->image_descriptor |= 0x1;
    break;

  case IMG_TYPE_RGB888:
  case IMG_TYPE_BGR888:
    header->type = 2;
    header->bpp = 24;
    break;

  case IMG_TYPE_RGB888|IMG_FORMAT_RLE8:
  case IMG_TYPE_BGR888|IMG_FORMAT_RLE8:
    header->type = 10;
    header->bpp = 24;
    break;

  case IMG_TYPE_RGBA32:
  case IMG_TYPE_BGRA32:
    header->type = 2;
    header->bpp = 32;
    header->image_descriptor |= 0x8;
    break;

  case IMG_TYPE_RGBA32|IMG_FORMAT_RLE8:
  case IMG_TYPE_BGRA32|IMG_FORMAT_RLE8:
    header->type = 10;
    header->bpp = 32;
    header->image_descriptor |= 0x8;
    break;

  case IMG_TYPE_R8:
    header->type = 3;
    header->bpp = 16;
    header->image_descriptor |= 0x8;
    break;

  case IMG_TYPE_R8|IMG_FORMAT_RLE8:
    header->type = 11;
    header->bpp = 16;
    header->image_descriptor |= 0x8;
    break;

  case IMG_TYPE_IDX8:
    header->type = 1;
    header->bpp = 8;
    header->image_descriptor |= 0x8;
    header->pal_size = self->pal.texImg.xcount;
    header->pal_bpc = self->pal.texImg.bpp;
    break;

  case IMG_TYPE_IDX8|IMG_FORMAT_RLE8:
    header->type = 32;
    header->bpp = 8;
    header->image_descriptor |= 0x8;
    header->pal_size = self->pal.texImg.xcount;
    header->pal_bpc = self->pal.texImg.bpp;
    break;


  default:
    //debug_printf("error flags");
    return IMG_ENOSTR;
    break;
  }
#endif



  if (0 != (self->flags&IMG_FLAG_XSWAP)) header->image_descriptor |= (1<<4);
  if (0 == (self->flags&IMG_FLAG_YSWAP)) header->image_descriptor |= (1<<5);
  if (self->texImg.xcount*self->texImg.ycount*(header->bpp/8) > IMG_PIXEL_SIZE_MAX) return IMG_EFBIG;
  return 0;
}


#if 0
/******************************************************************************/

/**
 *
 */
static int tga_headerToImage(tga_header_t *header, img_t *img)
{
  if (NULL == header) return IMG_ENODATA;
  if (NULL == img) return IMG_ENODATA;

  /**< size */
  img->width = header->width;
  img->height = header->height;
  img->naturalWidth = header->width;
  img->naturalHeight = header->height;

  // conserve le nombre de bpp
  img->bitsPerPixel = header->bpp;
  //img->pal_bpc = header->pal_bpc;
  //img->pal_sz = header->pal_size;
  img->bitsOffset = 0;
  img->flags |= IMG_FORMAT_2DTEXTURE;

  img->pal.bpc = header->pal_bpc;
  img->pal.size = header->pal_size;

  if (0 != (header->image_descriptor & (1 << 4))) img->flags |= IMG_FORMAT_XSWAP;
  if (0 == (header->image_descriptor & (1 << 5))) img->flags |= IMG_FORMAT_YSWAP;


  switch (header->type) {

  case 9: /* Runlength encoded 8bits palette */
    img->flags |= IMG_FORMAT_RLE8;
  case 1: /* Uncompressed, 8bits palette */
    img->flags |= IMG_TYPE_IDX8;
    break;

  case 10: /* Runlength encoded, BGR 16-24-32 bits */
    img->flags |= IMG_FORMAT_RLE8;
  case 2: /* Uncompressed, BGR 16-24-32 bits */
    if (header->bpp == 16) img->flags |= IMG_TYPE_BGR565;
    else if (header->bpp == 24) img->flags |= IMG_TYPE_BGR888;
    else if (header->bpp == 32) img->flags |= IMG_TYPE_BGRA32;
    break;

  case 11: /* Runlength encoded, Grayscale 8 bits */
    img->flags |= IMG_FORMAT_RLE8;
  case 3: /* Uncompressed, Grayscale 8 bits */
    img->flags |= IMG_TYPE_R8;
    break;

  case 32: /* Compressed color-mapped data, using Huffman, Delta, and
              runlength encoding */
  case 33: /* Compressed color-mapped data, using Huffman, Delta, and
              runlength encoding.  4-pass quadtree-type process. */
  case 0: /* No image data included.*/
  default:
    return IMG_EFORM;
    break;
  }



  return 0;
}
#endif



/******************************************************************************/

/**
 *
 * @return errors
 */
static int tga_file_write_pix(img_t *const self)
{
  // assert
  if (NULL == self) return IMG_ENODATA;
  if (IMG_VERIF != self->Hverif && TGA_VERIF != self->Hverif) return IMG_ENOSTR;
  if (NULL == self->src.file) return IMG_ENODATA;
  if (0 == self->texImg.size) return IMG_ENODATA;

  //if (0 != file_seek(self->src, self->seek_pix, SEEK_SET)) return IMG_ESPIPE;

  // RLE compressed
  if (0/*self->flags & IMG_FORMAT_RLE8*/) {

    //memset(self->src, 0, self->size);
    //printf("taille: %d\n", self->pix_sz);
    //if (self->data_size != file_write(self->texImg.data, 1, self->texImg.size, self->src)) return IMG_EIO;

  // uncompressed
  } else {
    //size = img_pix_sizeof(self);
    //int i;
    //for (i=0 ; i < self->data_size; i++) {
    //  file_putc(self->data[i], self->src);
    //  //file_putlong(self->src, 255<<16 | 255<<24 );
    //}
    if (self->texImg.size != stream_write(self->texImg.data, 1, self->texImg.size, &self->src)) return IMG_EIO;
  }

  //if (0 != file_seek(self->src, self->seek_pix, SEEK_SET)) return IMG_ESPIPE;
  return 0;
}


/******************************************************************************/

/**
 *
 * @return errors
 */
static int tga_file_write_palette(img_t *const self)
{
  uint32_t size;
  //printf("pal size: %u\n", self->pal.texImg.size);

  // assert
  //if (NULL == self) return IMG_ENODATA;
  if (IMG_VERIF != self->Hverif && TGA_VERIF != self->Hverif) return IMG_ENOSTR;
  if (0 == self->pal.texImg.size) return 0;
  if (NULL == self->src.file) return IMG_ENODATA;
  //if (0 == (self->flags & (IMG_TYPE_IDX1|IMG_TYPE_IDX4|IMG_TYPE_IDX8))) return 0;
  //if (0 != file_seek(self->src, self->seek_pix, SEEK_SET)) return IMG_ESPIPE;

  // RLE compressed
  if (0/*self->flags & IMG_FORMAT_RLE8*/) {
    //memset(self->src, 0, self->size);
    //printf("taille: %d\n", self->pix_sz);
    //if (self->data_size != file_write(self->data, 1, self->data_size, self->src)) return IMG_EIO;

  // uncompressed
  } else {
    //size = img_pix_sizeof(self);
/*    int i;
    for (i=0 ; i < self->pal.texImg.xcount; i++) {
      //file_putc(self->data[i], self->src);
      //file_putlong(self->src, 255<<16 | 255<<24 );
      printf("%u: %u,%u,%u\n", i+4,
                 self->pal.texImg.data[i*3+0],
                 self->pal.texImg.data[i*3+1],
                 self->pal.texImg.data[i*3+2]);
    }
*/
    size = self->pal.texImg.size;
    if (size != stream_write(self->pal.texImg.data, 1, size, &self->src)) {
      return IMG_EIO;
    }

  }

  //if (0 != file_seek(self->src, self->seek_pix, SEEK_SET)) return IMG_ESPIPE;
  return 0;
}


/******************************************************************************/

/**
 *
 * @return errors
 */
static int tga_mem_write_pix(img_t *const self)
{
  size_t size;
  #if 0
  // assert
  if (NULL == self) return IMG_ENODATA;
  if (TGA_VERIF != self->Hverif) return IMG_ENOSTR;
  if (NULL == self->src) return IMG_ENODATA;

  // RLE compressed
  if (self->flags & (IMG_FORMAT_RLE4 | IMG_FORMAT_RLE8)) {

  // uncompressed
  } else {
    size = img_pix_sizeof(self);
    memcpy((char*)self->src + sizeof(tga_header_t), self->texImg.data, size);
  }
  #endif
  return 0;
}


/******************************************************************************/

/**
 * écriture fichier tga.
 * @return errors
 */
int tga_write(img_t *const self)
{
  tga_header_t header;
  int error = 0;

  /* assert */
  if (NULL == self) return IMG_ENODATA;
  if (IMG_VERIF != self->Hverif && TGA_VERIF != self->Hverif) return IMG_ENOSTR;
  if (NULL == self->src.file) return IMG_ENODATA;

  /* header */
  error = tga_imageToHeader(self, &header);
  if (0 != error) return error;

  tga_debug_header(&header);

  /* file mode */
  //if (0 == self->mode) {
    printf("ecriture header\n");
    if (0 != stream_seek(&self->src, 0, SEEK_SET)) return IMG_ESPIPE;
    if (sizeof(header) != stream_write(&header, 1, sizeof(header), &self->src)) return IMG_EIO;
    if (0 != (error = tga_file_write_palette(self))) return error;
    if (0 != (error = tga_file_write_pix(self))) return error;

  /* mem mode */
  //} else {
    //texImg_memcpy(self->src, &header, sizeof(header));
    //if (0 != (error = tga_mem_write_pix(self))) return error;
  //}

  return 0;
}





