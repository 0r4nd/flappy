
#ifndef PALETTE_TYPES_H_
#define PALETTE_TYPES_H_

/* Standard library */
#include <stdint.h>
#include "../file_type.h"
#include "../../texImg/texImg_type.h"

#if 0
/**< Flags options */
#define PAL_OPT_SHIFT 0
#define PAL_OPT_MASK  (0xff<<(PAL_OPT_SHIFT))

enum palette_flags_e {
  /* options */
  TIMG_FLAG_ISALLOC  = (1<<0),  /**< Allocated */

  #if 0
  /* type */
  PAL_TYPE_RGB888   =  (0<<8), /* 32 bits */
  PAL_TYPE_BGR888   =  (1<<8), /* 32 bits */
  PAL_TYPE_RGBA32   =  (2<<8), /* 32 bits */
  PAL_TYPE_BGRA32   =  (3<<8), /* 32 bits */
  PAL_TYPE_RGB565   =  (4<<8), /* 16 bits */
  PAL_TYPE_BGR565   =  (5<<8), /* 16 bits */
  PAL_TYPE_RGB555   =  (6<<8), /* 16 bits */
  PAL_TYPE_BGR555   =  (7<<8), /* 16 bits */
  PAL_TYPE_RGB5551  =  (8<<8), /* 16 bits */
  PAL_TYPE_BGR5551  =  (9<<8), /* 16 bits */
  PAL_TYPE_RGB444   = (10<<8), /* 16 bits */
  PAL_TYPE_BGR444   = (11<<8), /* 16 bits */
  PAL_TYPE_RGBA4444 = (12<<8), /* 16 bits */
  PAL_TYPE_BGRA4444 = (13<<8), /* 16 bits */
  PAL_TYPE_RGB332   = (14<<8), /*  8 bits */
  PAL_TYPE_BGR332   = (15<<8), /*  8 bits */
  #endif
};
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct palette_s palette_t;
struct palette_s {
  texImg_t texImg;
  stream_t src;
};


#ifdef __cplusplus
}
#endif


#endif /* PALETTE_TYPES_H_ */

