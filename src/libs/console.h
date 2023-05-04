#ifndef CONSOLE_H_
#define CONSOLE_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */


#include "bufferConsole.h"


#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/

/**
 * User wrapped functions and options.
 */


/******************************************************************************/

/**
 * prototypes
 */
void console_set_size(HANDLE hnd, short xsize, short ysize);
void console_set_canvasSize(HANDLE hnd, short xsize, short ysize);
void console_cls(HANDLE hnd);
void console_replace(HANDLE hnd);



#ifdef __cplusplus
}
#endif


#endif /* CONSOLE_H_ */


