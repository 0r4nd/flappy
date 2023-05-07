
#ifndef IMG_TGA_H_
#define IMG_TGA_H_


#include "tga_type.h"
#include "img.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * prototypes
 */
int tga_open(const char *in, img_t *out, const char *mode);
img_t *tga_free(img_t *const self);
img_t *tga_close(img_t *const self);
img_t *tga_close_nofree(img_t *const self);

void tga_debug_header(tga_header_t *header);


/* tga_read.c */
int tga_read(img_t *const self);

/* tga_write.c */
int tga_write(img_t *const self);


#ifdef __cplusplus
}
#endif


#endif // IMG_TGA_PROTO_H_




