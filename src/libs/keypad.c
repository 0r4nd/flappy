

#include "keypad.h"



/******************************************************************************/

/**
 * set a keypad_t structure
 *
 * @param dir - direction of the snake (top:0 bottom:1 left:2 rigth:3)
 * @return a keypad_t structure
 */
keypad_t keypad(int dir)
{
  keypad_t ret;
  keypad_memset(&ret, 0, sizeof(ret));
  ret.cur = 0;
  ret.last = dir;
  return ret;
}


/******************************************************************************/

/**
 * keypad functions
 *
 * @param self - a keypad_t structure
 * @return -
 */
int keypad_update(keypad_t *const self)
{
  assert(self);

  self->last = self->cur;
  self->cur = 0;

  if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
    self->cur = VK_RETURN;
  }

  if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
    self->cur = VK_SPACE;
  }
  /*if (GetAsyncKeyState(40) & 0x8000) {
    self->cur = 40;
  }
  if (GetAsyncKeyState(37) & 0x8000) {
    self->cur = 37;
  }
  if (GetAsyncKeyState(39) & 0x8000) {
    self->cur = 39;
  }*/


  if (GetAsyncKeyState(VK_UP) & 0x8000) {
    self->cur = VK_UP;
  }
  if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
    self->cur = VK_DOWN;
  }
  if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
    self->cur = VK_LEFT;
  }
  if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
    self->cur = VK_RIGHT;
  }

  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
    self->cur = VK_ESCAPE;
    return 1;
  }

  return 0;
}


