#ifndef TEXT_H_
#define TEXT_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */


#include "bufferConsole.h"
#include "file/img/img.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/

/**
 * User wrapped functions and options.
 */
#define text_memset(out,val,sz) memset(out,val,sz)



/******************************************************************************/

/**
 * prototypes
 */
void text_fill(const char *str, img_t *image, bufferConsole_t *out,
                      int x, int y, float size, uint32_t color, uint32_t flags);



#ifdef __cplusplus
}
#endif


#endif /* TEXTCONSOLE_H_ */


