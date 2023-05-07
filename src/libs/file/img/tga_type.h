
#ifndef IMG_TGA_TYPES_H_
#define IMG_TGA_TYPES_H_


#ifdef __cplusplus
extern "C" {
#endif


/* Standard library. */
#include <stdio.h>
#include <stdint.h>

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


/**
 * header format tga: 18 octets
 * @note the header is not aligned
 */
#pragma pack(push, 1)
typedef struct tga_header_s tga_header_t;
struct tga_header_s {
  char id_size;          /* size of image id */
  char pal;              /* 1 is has a palette */
  char type;             /* compression type */

  short pal_idx;         /* palette origin */
  short pal_size;        /* palette length */
  char pal_bpc;          /* palette size */

  short xo;              /* bottom left x coord origin */
  short yo;              /* bottom left y coord origin */

  short width;           /* width (in pixels) */
  short height;          /* height (in pixels) */
  char bpp;              /* bits per pixel: 8, 16, 24 or 32 */
  char image_descriptor; /* 24 bits = 0x00; 32 bits = 0x80 */
};
/* __attribute__ ((packed))*/
#pragma pack(pop)


/* Need to be unaligned */
static char const assert_sizeof_tga_header[(sizeof(tga_header_t) == 18)? 1 : -1] = {'!'};


/**
 * fourcc
 */
#define TGA_MAKEFOURCC(b0,b1,b2,b3) ((b0)|((b1)<<8)|((b2)<<16)|((b3)<<24))
#define TGA_MAKETWOCC(b0,b1)        ((b0)|((b1)<<8))
#define TGA_VERIF                   TGA_MAKEFOURCC('T','G','A','#')



#ifdef __cplusplus
}
#endif


#endif // IMG_TGA_H_




