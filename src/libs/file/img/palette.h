

#ifndef PALETTE_H_
#define PALETTE_H_



#include <stdio.h>
#include "palette_type.h"
#include "../../texImg/texImg.h"


#ifdef __cplusplus
extern "C" {
#endif



/* palette size max */
#define PALETTE_COUNT_MAX 256
#define PALETTE_SIZE_MAX  (PALETTE_COUNT_MAX*4)


/**
 * prototypes
 */

palette_t palette_new(uint8_t internalFormat, uint16_t size,
                      uint8_t format, uint8_t type, void *data);
palette_t palette_newFromFile(const char *str_file);
palette_t palette_dup(palette_t *const self);

int palette_toFile(palette_t *const self, const char *str_file);

void palette_delete(palette_t *const self);
void palette_free(palette_t *const self);
void palette_onerror_print(palette_t *const self);
int palette_isError(palette_t *const self);

/* conversions */
int palette_convertToRGB888(palette_t *const self);
int palette_convertToBGR888(palette_t *const self);

/* setters/getters */
uint32_t palette_getBpp(palette_t *const self);
uint32_t palette_setColor(palette_t *const self, uint32_t id,
                          uint32_t r, uint32_t g, uint32_t b, uint32_t a);
uint32_t palette_getColor(palette_t *const self, uint32_t id);
uint32_t palette_getR(palette_t *const self, uint32_t id);
uint32_t palette_getG(palette_t *const self, uint32_t id);
uint32_t palette_getB(palette_t *const self, uint32_t id);

/* debug (can be deleted) */
char *palette_type_toString(palette_t *const self, char *s);
char *palette_color_toString(palette_t *const self, char *s, char *space, uint32_t id);




/* palette_read.c */
//int palette_read(palette_t *const self);

/* palette_write.c */
//int palette_write(palette_t *const self);


#ifdef __cplusplus
}
#endif


#endif // PALETTE_H_




