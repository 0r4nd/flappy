

#include <math.h>

#include "game.h"
#include "libs/colorUtil.h"


/******************************************************************************/

/**
 * game loop for the gameover screen
 *
 * @param sprites - array of sprite_t
 * @param keys    - keypad used
 * @param buf     - buffer for the console
 * @return 0 = gameover, 1 = continue, -1 = error
 */
int game_open_gameover(gameState_t *state, float dt)
{
  return 1;
}


int game_update_gameover(gameState_t *state, float dt)
{
  //static char str_score[64] = {0};
  sprite_t *spr = state->sprites;
  bufferConsole_t *buf = &state->buffer;
  keypad_t *keys = &state->keys;

  if (keypad_update(keys)) return -1;

  if (keys->cur == VK_RETURN) return 0;
  if (keys->cur == VK_ESCAPE) return -1;

  /* background */
  sprite_update(spr+1, buf, dt);
  sprite_fill(spr+1, buf);

/*
  sprintf(str_score, " GAME OVER! ");
  text_fill(str_score, buf, 70, 10, 0x8f);
  sprintf(str_score, " Result: ");
  text_fill(str_score, buf, 69, 12, 0x8f);
  sprintf(str_score, " %d ", state->score[0]);
  text_fill(str_score, buf, 80, 12, 0x8e);
*/

  console_replace(buf->hnd);
  bufferConsole_update(buf);
  bufferConsole_swap(buf);

  return 1;
}

int game_close_gameover(gameState_t *state, float dt)
{
    return 0;
}

