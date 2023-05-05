

#include <math.h>

#include "game.h"
#include "libs/colorUtil.h"



/******************************************************************************/

/**
 * game title screen
 *
 * @note - La structure img_t n'est pas pensée dans le but d'être utilisée
 *         directement. C'est une structure tampon qui permet d'alimenter une
 *         structure de type texImg_t (conçue pour les perf).
 *         Dans l'idéal les structures texImg_t devraient se trouver dans une
 *         hashTable performante.
 */
int game_open_title(gameState_t *state, float dt)
{
  //float z;
  int i,tmp;
  sprite_t *spr = state->sprites;
  bufferConsole_t *buf = &state->buffer;
  keypad_t *keys = &state->keys;

  /* title */
  aIMAGES[TITLE_ID] = img_newFromFile("data/image/title.tga");
  texImg_convertToRGBA32(&aIMAGES[TITLE_ID].texImg, 0,1);

  /* background levels */
  aIMAGES[BG1_ID] = img_newFromFile("data/image/bg1.tga");
  texImg_convertToRGBA32(&aIMAGES[BG1_ID].texImg, 0,1);
  aIMAGES[BG2_ID] = img_newFromFile("data/image/bg2.tga");
  texImg_convertToRGBA32(&aIMAGES[BG2_ID].texImg, 0,1);
  aIMAGES[BG3_ID] = img_newFromFile("data/image/bg3.tga");
  texImg_convertToRGBA32(&aIMAGES[BG3_ID].texImg, 0,1);

  /* cadre */
  //aIMAGES[CADRE_ID] = img_newFromFile("data/image/cadre.tga");
  //texImg_convertToRGBA32(&aIMAGES[CADRE_ID].texImg, 0,1);

  /* ui */
  aIMAGES[PRESS_SPACE_ID] = img_newFromFile("data/image/press_space.tga");
  texImg_convertToRGBA32(&aIMAGES[PRESS_SPACE_ID].texImg, 0,1);
  aIMAGES[RETRY_LEAVE_ID] = img_newFromFile("data/image/retry_leave.tga");
  texImg_convertToRGBA32(&aIMAGES[RETRY_LEAVE_ID].texImg, 0,1);

  /* bird */
  aIMAGES[BIRD_ID] = img_newFromFile("data/image/bird.tga");
  texImg_convertToRGBA32(&aIMAGES[BIRD_ID].texImg, 0,1);

  /* text font 8x8 */
  aIMAGES[FONT_ID] = img_newFromFile("data/image/CGA8x8.tga");
  texImg_convertToRGBA32(&aIMAGES[FONT_ID].texImg, 0,1);

  /* saw */
  aIMAGES[SAW_BACK_ID] = img_newFromFile("data/image/saw_back.tga");
  texImg_convertToRGBA32(&aIMAGES[SAW_BACK_ID].texImg, 0,1);
  aIMAGES[SAW_FRONT_TOP_ID] = img_newFromFile("data/image/saw_front_top.tga");
  texImg_convertToRGBA32(&aIMAGES[SAW_FRONT_TOP_ID].texImg, 0,1);
  aIMAGES[SAW_FRONT_MIDDLE_ID] = img_newFromFile("data/image/saw_front_middle.tga");
  texImg_convertToRGBA32(&aIMAGES[SAW_FRONT_MIDDLE_ID].texImg, 0,1);
  aIMAGES[SAW_FRONT_BOTTOM_ID] = img_newFromFile("data/image/saw_front_bottom.tga");
  texImg_convertToRGBA32(&aIMAGES[SAW_FRONT_BOTTOM_ID].texImg, 0,1);


  /* sprites */
  spr[TITLE_ID] = sprite_new(aIMAGES+TITLE_ID,
                             Vec2(45,10), Vec2(103,38), 0);
  spr[TITLE_ID].active = 0;
  spr[TITLE2_ID] = sprite_new(aIMAGES+TITLE_ID,
                              Vec2(45,10), Vec2(103,38), BUFFERCONSOLE_BILINEAR);
  spr[TITLE2_ID].scale = 0.1;

  spr[BG3_ID] = sprite_new(aIMAGES+BG3_ID,
                           Vec2(0,0), Vec2(190,60), 0);
  spr[BG3_ID].ydecay_ratio = aDECAY_BG3;
  spr[BG3_ID].decay_speed = 1.0;

  spr[PRESS_SPACE_ID] = sprite_new(aIMAGES+PRESS_SPACE_ID,
                                   Vec2(72,53+20), Vec2(48,16), BUFFERCONSOLE_BILINEAR);
  spr[PRESS_SPACE_ID].yframe_count = 4;
  spr[PRESS_SPACE_ID].yframe_speed = 0.3;
  spr[PRESS_SPACE_ID].scale = 0.1;


  /* cadre */
  spr[CADRE_ID] = sprite_new(aIMAGES+CADRE_ID,
                             Vec2(45,10), Vec2(104,16), 0);
  //spr[CADRE_ID].active = 0;


  /* particles */
  for (i = 0; i < PARTICLES_MAX; ++i) {
    aPARTICLES[i] = particle_new(0,0, RGBA32(255,255,255,rand_minMax(128,255)), 0);

    /* here tmp is alpha and zdepth */
    tmp = (aPARTICLES[i].color>>24)&0xff;
    particle_randInQuad(aPARTICLES+i, 0, buf->size.X+buf->size.X/2, 0, buf->size.Y,
                        -1.0,1.0,
                        (randf_minMax(0.3,0.7)*(float)tmp) / 255.0,
                        (randf_minMax(0.4,0.8)*(float)tmp) / 255.0);
  }


  return 1;
}


int game_update_title(gameState_t *state, float dt)
{
  int i;
  sprite_t *spr = state->sprites;
  bufferConsole_t *buf = &state->buffer;
  keypad_t *keys = &state->keys;

  if (keypad_update(keys)) return -1;

  /* background */
  sprite_update(spr+BG3_ID, buf, dt);
  sprite_fill(spr+BG3_ID, buf);

  sprite_update(spr+PRESS_SPACE_ID, buf, dt);
  sprite_fill(spr+PRESS_SPACE_ID, buf);

  sprite_update(spr+TITLE_ID, buf, dt);
  sprite_fill(spr+TITLE_ID, buf);

  sprite_update(spr+TITLE2_ID, buf, dt);
  sprite_fill(spr+TITLE2_ID, buf);


  //sprite_update(spr+CADRE_ID, buf, dt);
  //sprite_fill(spr+CADRE_ID, buf);



  /* title vertical anim */
  static float anim=0;
  anim += 2.0;
  spr[TITLE_ID].pos.y = 4 + cos(anim * 3.1415926f / 180.0f)*3;
  spr[TITLE2_ID].pos.y = 4 + cos(anim * 3.1415926f / 180.0f)*3;


  /* zoom intro effect */
  spr[TITLE2_ID].scale += 0.03;
  if (spr[TITLE2_ID].scale > 1.0) {
    if (spr[TITLE2_ID].opacity == 0) {
      spr[TITLE_ID].active = 1;
      PlaySound("data/audio/title.wav", NULL, SND_LOOP | SND_ASYNC);
      //PlaySound(TEXT("SystemStart"), NULL, SND_ALIAS);
      //PlaySound("data/audio/start.wav", NULL, SND_ASYNC);
    }
    spr[TITLE2_ID].opacity += 10;
    //spr[TITLE2_ID].scale += 0.02;

    if (spr[TITLE2_ID].opacity >= 255) {
      spr[TITLE2_ID].opacity = 255;
      spr[TITLE2_ID].active = 0;
    }
  }


  /* press space intro effect */
  if (aSPRITES[PRESS_SPACE_ID].pos.y > 44) {
    aSPRITES[PRESS_SPACE_ID].pos.y -= 1.0;
    if (aSPRITES[PRESS_SPACE_ID].pos.y < 44) aSPRITES[PRESS_SPACE_ID].pos.y = 44;
  }



  /* alpha anim */
  static int add = 5;
  spr[PRESS_SPACE_ID].opacity += add;
  spr[PRESS_SPACE_ID].scale = 0.8 + spr[PRESS_SPACE_ID].opacity/500.0;
  if (spr[PRESS_SPACE_ID].opacity > 150) {
    spr[PRESS_SPACE_ID].opacity = 150;
    add = -5;
  } else if (spr[PRESS_SPACE_ID].opacity <= 0) {
    spr[PRESS_SPACE_ID].opacity = 0;
    add = 5;
  }



  /* snow effect */
  for (i = 0; i < PARTICLES_MAX; ++i) {
    particle_update(aPARTICLES+i, buf, 1);
    particle_fill(aPARTICLES+i, buf);
  }

  if (keys->cur == VK_SPACE) return 0;

  return 1;
}


int game_close_title(gameState_t *state, float dt)
{
  sprite_t *spr = state->sprites;
  bufferConsole_t *buf = &state->buffer;
  keypad_t *keys = &state->keys;
  static int opacity = 255;
  if (keypad_update(keys)) return -1;

  /* start sfx */
  if (opacity == 255) {
    PlaySound("data/audio/start.wav", NULL, SND_ASYNC);
  }

  opacity -= 6;
  if (opacity <= 0) {
    opacity = 0;
    return 0;
  }
  game_update_title(state, dt);
  bufferConsole_fillFlatQuad(buf, RGBA32(255,255,255,255), opacity,
                             0<<12,0<<12, buf->size.X<<12, buf->size.Y<<12);
  return 1;
}

