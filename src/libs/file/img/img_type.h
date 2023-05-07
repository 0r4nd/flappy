
#ifndef IMG_TYPE_H_
#define IMG_TYPE_H_


/* Standard library */
#include "palette_type.h"


/**
 * stdint
 */
#ifndef _WIN32
#  include <stdint.h>
#  include <stdbool.h>
#else
  typedef unsigned char uint8_t;
  typedef signed char int8_t;
  typedef unsigned short uint16_t;
  typedef signed short int16_t;
  typedef unsigned int uint32_t;
  typedef signed int int32_t;
  typedef unsigned __int64 uint64_t;
  typedef signed __int64 int64_t;
#ifndef __cplusplus
typedef unsigned char bool;
#define true 1
#define false 0
#endif
#endif /* _WIN32 */

#include <errno.h>

/**< tga */
#include "tga_type.h"


#ifdef __cplusplus
extern "C" {
#endif

/**< Errors */
enum img_err_e {
  IMG_EUNKN     = (1<<0),  /**< Unknow error */
  IMG_ENOENT    = (1<<1),  /**< No such file or directory */
  IMG_EIO       = (1<<2),  /**< I/O error */
  IMG_ENOMEM    = (1<<3),  /**< Out of memory */
  IMG_EINVAL    = (1<<4),  /**< Invalid argument */
  IMG_EMFILE    = (1<<5),  /**< Too many open files */
  IMG_EFBIG     = (1<<6),  /**< File too large */
  IMG_ESPIPE    = (1<<7),  /**< Illegal seek */
  IMG_ENOIMG    = (1<<8),  /**< Not an image */
  IMG_ENOSTR    = (1<<9),  /**< Not a correct structure */
  IMG_ENODATA   = (1<<10), /**< No data available */
  IMG_EOVERFLOW = (1<<11), /**< Value too large for defined data type */
  IMG_EFORM     = (1<<12)  /**< Invalid format */
};


/* Type de format/compression. (les 8 premiers bits) */
/*
#define IMG_TYPE_SHIFT 0
#define IMG_TYPE_MASK (0xff<<(IMG_TYPE_SHIFT))

#define IMG_FORMAT_SHIFT 8
#define IMG_FORMAT_MASK (0xffffff<<(IMG_FORMAT_SHIFT))
*/
enum img_flags_e {
/*
  IMG_TYPE_NONE             =    0,  // Prend le format du niveau inférieur

  IMG_TYPE_IDX1             =    1,  // Format indexé sur palette (index sur 1bit).
  IMG_TYPE_IDX4             =    2,  // Format indexé sur palette (index sur 4bits).
  IMG_TYPE_IDX8             =    3,  // Format indexé sur palette (index sur 8bits).

  IMG_TYPE_RGB444           =    4,  // Le champ alpha n'est pas utilisé (même si il existe)
  IMG_TYPE_RGBA4444         =    5,  // Format avec alpha. (bpp: 1,4,8,12,16,24,32)
  IMG_TYPE_RGBT4444         =    6,  // Le bit de poid fort du champ alpha indique si le pixel est à afficher ou pas. (en mode 16 bits le format est 5551)
  IMG_TYPE_BGR444           =    7,  // Le champ alpha n'est pas utilisé (même si il existe)
  IMG_TYPE_BGRA4444         =    8,  // Format avec alpha. (bpp: 1,4,8,12,16,24,32)
  IMG_TYPE_BGRT4444         =    9,  // Le bit de poid fort du champ alpha indique si le pixel est à afficher ou pas. (en mode 16 bits le format est 5551)

  IMG_TYPE_RGB555           =   10,  // Le champ alpha n'est pas utilisé (même si il existe)
  IMG_TYPE_RGBA5551         =   11,  // Format avec alpha. (bpp: 1,4,8,12,16,24,32)
  IMG_TYPE_BGR555           =   12,  // Le champ alpha n'est pas utilisé (même si il existe)
  IMG_TYPE_BGRA5551         =   13,  // Format avec alpha. (bpp: 1,4,8,12,16,24,32)

  IMG_TYPE_RGB565           =   14,  // Le champ alpha n'est pas utilisé (même si il existe)
  IMG_TYPE_BGR565           =   15,  // Le champ alpha n'est pas utilisé (même si il existe)

  IMG_TYPE_RGB888           =   16,  // Le champ alpha n'est pas utilisé (même si il existe)
  IMG_TYPE_RGBA32         =   17,  // Format avec alpha. (bpp: 1,4,8,12,16,24,32)
  IMG_TYPE_RGBT32         =   18,  // Le bit de poid fort du champ alpha indique si le pixel est à afficher ou pas. (en mode 16 bits le format est 5551)
  IMG_TYPE_BGR888           =   19,  // Le champ alpha n'est pas utilisé (même si il existe)
  IMG_TYPE_BGRA32         =   20,  // Format avec alpha. (bpp: 1,4,8,12,16,24,32)
  IMG_TYPE_BGRT32         =   21,  // Le bit de poid fort du champ alpha indique si le pixel est à afficher ou pas. (en mode 16 bits le format est 5551)

  IMG_TYPE_R8               =   22,  // R seul.
  IMG_TYPE_G8               =   23,  // G seul.
  IMG_TYPE_B8               =   24,  // B seul.
  IMG_TYPE_A8               =   25,  // Alpha seul.

  IMG_TYPE_DXT1C            =   26,  // Compression s3tc couleur.
  IMG_TYPE_DXT1A            =   27,  // Compression s3tc avec 1 bit pour l'alpha.
  IMG_TYPE_DXT3             =   28,  // Compression s3tc avec une zone alpha.
  IMG_TYPE_DXT5             =   29,  // Compression s3tc avec une zone alpha compressée.
  IMG_TYPE_3DC              =   30,  // Compression des normal maps.
  IMG_TYPE_DEPTH24_STENCIL8 =   31,  // Depth buffer et stencil sur 8 bits.
  IMG_TYPE_ATI2             =   32,  // Compression des normal maps.
  IMG_TYPE_BUFFER           =   33,  // Pour passer des données à la place des textures.

*/
/*
  IMG_FORMAT_1DTEXTURE   = (1<< 9),  // Texture en 1D.
  IMG_FORMAT_2DTEXTURE   = (1<<10),  // Texture en 2D
  IMG_FORMAT_3DTEXTURE   = (1<<12),  // Texture en voxel.

  IMG_FORMAT_SPHEREMAP   = (1<<11),  // Texture en mode fish-eye.
  IMG_FORMAT_CUBEMAP     = (1<<12),  // 6 Textures qui se suivent (horizontal ou vertical selon la taille).
*/
  IMG_FLAG_GENMIPMAP   = (1<<13),  // Génération d'une mipmap
  IMG_FLAG_REPEAT      = (1<<14),  // Texture qui se répete.
  IMG_FLAG_CLAMP       = (1<<15),  // Texture qui clamp.
  IMG_FLAG_NEAREST     = (1<<16),  // Mode de filtrage nearest
  IMG_FLAG_BILINEAR    = (1<<17),  // Mode de filtrage Bilinéaire
  IMG_FLAG_TRILINEAR   = (1<<18), // Mode de filtrage Trilinéaire (il faut une mipmap)
  IMG_FLAG_ANISO       = (1<<19), // Mode de filtrage Anisotropique (il faut une mipmap)

  //IMG_FLAG_PIX_MALLOC  = (1<<20),  /* Dynamic allocation used for pixels */
  //IMG_FLAG_PAL_MALLOC  = (1<<21),  /* Dynamic allocation used for palette */
  IMG_FLAG_RBO         = (1<<22),  // Texture qui possède un RenderBufferObject.
  IMG_FLAG_AUTOMIPMAP  = (1<<23),  // Texture qui possède un RenderBufferObject.
  IMG_FLAG_XSWAP       = (1<<24),  // Swap sur les x ?.
  IMG_FLAG_YSWAP       = (1<<25),  // Swap sur les y ?.

  IMG_FLAG_RLE4        = (1<<26),  // Compression rle sur 4 bits.
  IMG_FLAG_RLE8        = (1<<27),  // Compression rle sur 8 bits.

  IMG_FLAG_DIRECTCOPY  = (1<<28),  // Charge la texture directement (sans attendre la prochaine frame)
  IMG_FLAG_HEADER      = (1<<29)
  //#define IMG_ALPHASD      (1<<18) // Convertit le canal alpha en radial signed-dist
};



//##################################################################################################
//## structure texture
//## .state: bit 0-9   = niveau mipmap réclamé.
//##         bit 10-19 = niveau mipmap alloué en mémoire.
//##         bit 20-29 = niveau mipmap chargé en mémoire.
//##################################################################################################
typedef struct img_s img_t;
struct  img_s {
  uint32_t Hverif;       /**< Read-only */

  uint32_t flags;        /**< format and flags */
  uint32_t naturalFlags; /**< original format and flags */

  //int16_t width;         /**< width attribute of an img */
  //int16_t height;        /**< height attribute of an img */
  int16_t naturalWidth;  /**< original width of an img */
  int16_t naturalHeight; /**< original height of an img */

  uint16_t pitch;        /**< Read-only */
  //uint8_t bitsPerPixel;  /*REMPLACER PAR pix_bpc */ /**< 1,4,8,16,24,32 */
  //uint8_t pal_bpc;

  uint8_t bitsOffset;

  /**< events callbacks */
  void (*onload) (img_t*);
  void (*onerror)(img_t*);
  void (*onabort)(img_t*);

  palette_t pal;
  texImg_t texImg;

  /**< sources and datas */
  stream_t src;
  //void *src;
  //void *naturalSrc;
  //char *pal;              /**< Read-only */

  /**< data begin */
  //uint8_t *data;          /**< Read-only */
  //long data_seek;         /**< Read-only */
  //size_t data_size;       /**< Read-only */

  //long pal_seek;           /**< Read-only */
  //size_t pal_sz;          /**< Read-only */
  //int16_t errors;        /**<  */
  int16_t aborts;        /**<  */
  //uint16_t mode;         /**< Read-only 0:file  1:buffer */
  uint16_t complete;     /**< 0:not loaded  1: loaded */
};


/**
 * fourcc structure verification
 */
#define IMG_MAKEFOURCC(b0,b1,b2,b3) ((b0)|((b1)<<8)|((b2)<<16)|((b3)<<24))
#define IMG_MAKETWOCC(b0,b1)        ((b0)|((b1)<<8))
#define IMG_VERIF                   TGA_MAKEFOURCC('I','M','G','#')



#ifdef __cplusplus
}
#endif


#endif // IMG_TYPE_H_








