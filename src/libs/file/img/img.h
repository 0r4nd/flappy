

#ifndef IMG_H_
#define IMG_H_


#include <stdio.h>
#include "img_type.h"
#include "palette.h"
#include "tga.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * about
 */
#define IMG_VERSION  "2016-07-27"


/**
 * configuration
 */

/* pixels max: 200mo */
#define IMG_PIXEL_COUNT_MAX  1000000*50
#define IMG_PIXEL_SIZE_MAX   (IMG_PIXEL_COUNT_MAX*4)



/**
 * callbacks
 */
void img_default_callback_onload(img_t *const self);
void img_default_callback_onerror(img_t *const self);
void img_default_callback_onabort(img_t *const self);


/**
 * private functions
 */
img_t *img_call_onload(img_t *const self);
img_t *img_call_onerror(img_t *const self);
img_t *img_call_onabort(img_t *const self);


/**
 * methods
 */
int img_isnull(img_t *const self);
int img_pix_isnull(img_t *const self);


size_t img_pix_sizeof(img_t *const self);
size_t img_pal_sizeof(img_t *const self);

img_t img_set(void);
img_t img_new(uint8_t format, uint8_t type,
              uint16_t width, uint16_t height, void *src, palette_t *pal);
img_t img_fromMemTGA(const char *buff_tga, uint32_t flags);
img_t img_newFromFile(const char *str_file);
img_t img_newFromStreamFile(const char *str_file);
int img_toFile(img_t *const self, const char *str_file);

int img_convertToRGB565(img_t *const self);
int img_convertToBGR565(img_t *const self);
int img_convertToBGR888(img_t *const self);
int img_convertToRGBA32(img_t *const self);
int img_fillColor(img_t *const self,
                  uint32_t r, uint32_t g, uint32_t b, uint32_t a);
int img_fillIndex(img_t *const self, uint8_t idx);

void img_free(img_t *const self);
void img_delete(img_t *const self);
void img_close(img_t *const self);


//img_t img_load(const char *in);
int img_open(const char *in, img_t *out, const char *mode);
void img_print(img_t *const self);


#ifdef __cplusplus
}
#endif


#endif // IMG_H_




