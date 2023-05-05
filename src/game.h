#ifndef GAME_H_
#define GAME_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */
#include <mmsystem.h> /* playsound() function */



#include "libs/file/img/img.h"
#include "libs/colorUtil.h"
#include "libs/bufferConsole.h"
#include "libs/console.h"
#include "libs/keypad.h"
#include "libs/sprite.h"
#include "libs/particle.h"
#include "libs/text.h"
#include "libs/tinycthread/tinycthread.h"


typedef struct gameState_s {
  keypad_t keys;
  bufferConsole_t buffer;
  sprite_t *sprites;
  float dt; /* current delta time */

  int sprites_max;
  int ennemy_max;
  float ennemy_speed;

  float distance;
  uint32_t blend[12];

  int score[4]; /* first,second,third, best */
} gameState_t;



enum Color {
  BLACK      = 0,
  DARKBLUE   = 1,
  DARKGREEN  = 2,
  DARKTEAL   = 3,
  DARKRED    = 4,
  DARKPINK   = 5,
  DARKYELLOW = 6,
  GRAY       = 7,
  DARKGRAY   = 8,
  BLUE       = 9,
  GREEN      = 10,
  TEAL       = 11,
  RED        = 12,
  PINK       = 13,
  YELLOW     = 14,
  WHITE      = 15,
  ALPHA      = 16
};


/* need hashTable :D */
#define TITLE_ID        0
#define TITLE2_ID       1
#define BG1_ID          2
#define BG2_ID          3
#define BG3_ID          4
#define PRESS_SPACE_ID  5
#define RETRY_LEAVE_ID  6
#define BIRD_ID         7
#define FONT_ID         8

#define ENNEMY1_ID      9
#define CADRE_ID       10

/* saw */
#define SAW_BACK_ID          20
#define SAW_FRONT_TOP_ID     30
#define SAW_FRONT_MIDDLE_ID  40
#define SAW_FRONT_BOTTOM_ID  50

/* sprites */
#define SPRITES_MAX 128
extern sprite_t aSPRITES[SPRITES_MAX];
/* particles */
#define PARTICLES_MAX 128
extern particle_t aPARTICLES[PARTICLES_MAX];
/* textures */
#define IMAGES_MAX 32
extern img_t aIMAGES[IMAGES_MAX];

extern gameState_t gGAMESTATE;
extern const float aDECAY_BG1[60];
extern const float aDECAY_BG2[60];
extern const float aDECAY_BG3[60];



#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/

/**
 * prototypes
 */
int rand_minMax(int low, int high);
void ennemy_update(gameState_t *state);
void ennemy_fill(gameState_t *state);


int game_open_title(gameState_t *state, float dt);
int game_update_title(gameState_t *state, float dt);
int game_close_title(gameState_t *state, float dt);

int game_open_loop(gameState_t *state, float dt);
int game_update_loop(gameState_t *state, float dt);
int game_close_loop(gameState_t *state, float dt);

int game_open_gameover(gameState_t *state, float dt);
int game_update_gameover(gameState_t *state, float dt);
int game_close_gameover(gameState_t *state, float dt);



#ifdef __cplusplus
}
#endif


#endif /* GAME_H_ */


