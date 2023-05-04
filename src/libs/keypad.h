#ifndef KEYPAD_H_
#define KEYPAD_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */



#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/

/**
 * User wrapped functions and options.
 */
#define keypad_memset(out,val,sz) memset(out,val,sz)


/******************************************************************************/

/**
 * structs
 */
typedef struct keypad_s {
  /*unsigned char key[4];  up,down,left,right */
  uint8_t cur;
  uint8_t last;
} keypad_t;


/******************************************************************************/

/**
 * prototypes
 */
keypad_t keypad(int dir);
int keypad_update(keypad_t *const self);



#ifdef __cplusplus
}
#endif


#endif /* KEYPADCONSOLE_H_ */


