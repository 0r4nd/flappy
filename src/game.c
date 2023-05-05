

#include <math.h>

#include "game.h"
#include "libs/colorUtil.h"



/**
 * sprite pool
 *
 * @note -
 *  0 -  9 : sprites principaux
 * 10 - 19 : ennemis
 * 20 - 49 : particules
 *
 * Notons que c'est une très mauvaise manière de gérer ses sprites.
 * En général il convient mieux d'utiliser un tableau associatif pour ensuite
 * ajouter ses sprites par exemple de la manière suivante :
 *
 * pour ajouter:
 * sprite_new(Hash("actor"));
 *
 * pour récupérer:
 * sprite_t *s = sprite_get(Hash("actor"));
 *
 *
 * Pour les sprites qui font partie du même groupe on ajoute simplement un
 * numéro à la suite du nom en base 10,16 ou 64 :
 *
 * pour ajouter:
 * for (i=0; i<10; ++i) sprite_new(Hash(mystrcat("particle",base10_toString(i));
 *
 * pour récupérer la particule n°6 :
 * sprite_t *s = sprite_get(Hash("particle5"));
 *
 * ou pour récupérer tout le groupe :
 * sprite_t *s[10];
 * for (i=0; i<10; ++i) s[i] = sprite_get(Hash(mystrcat("particle",base10_toString(i));
 *
 *
 */
sprite_t aSPRITES[SPRITES_MAX];
particle_t aPARTICLES[PARTICLES_MAX];
img_t aIMAGES[IMAGES_MAX];
gameState_t gGAMESTATE;

const float aDECAY_BG1[60] = {
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,

  1/3.6, 1/3.4, 1/3.2, 1/3.0, 1/2.8, 1/2.6, 1/2.4, 1/2.2, 1/2.0, 1/1.8,
  1/1.6, 1/1.4, 1/1.2, 1/1.0, 1/0.9, 1/0.9, 1/0.9, 1/0.9, 1/0.9, 1/0.9,
};

const float aDECAY_BG2[60] = {
  0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,

  1/3.6, 1/3.4, 1/3.2, 1/3.0, 1/2.8, 1/2.6, 1/2.4, 1/2.2, 1/2.0, 1/1.8,
  1/1.6, 1/1.4, 1/1.2, 1/1.0, 1/0.9, 1/0.8, 1/0.7, 1/0.6, 1/0.5, 1/0.4,
};

const float aDECAY_BG3[60] = {
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,
  1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0, 1/6.0,

  1/3.6, 1/3.4, 1/3.2, 1/3.0, 1/2.8, 1/2.6, 1/2.4, 1/2.2, 1/2.0, 1/1.8,
  1/1.6, 1/1.4, 1/1.2, 1/1.0, 1/0.9, 1/0.8, 1/0.7, 1/0.6, 1/0.5, 1/0.4,
};


/******************************************************************************/

/**
 * utils
 */
int rand_minMax(int low, int high)
{
  return rand() % (high - low + 1) + low;
}


/******************************************************************************/

/**
 * update ennemy process
 */
void ennemy_update(gameState_t *state)
{
  int i, xpos,ypos, front_id;

  assert(state);

  sprite_t *spr = state->sprites;
  bufferConsole_t *buf = &state->buffer;
  //keypad_t *keys = &state->keys;
  sprite_t *back,*front;

  for (i = 0 ; i < state->ennemy_max; ++i) {
    back = spr + SAW_BACK_ID+i;
    front = spr + SAW_FRONT_BOTTOM_ID+i;

    if (back->active) {
      if (back->pos.x + back->size.x < 0) {
        sprite_delete(back);
        sprite_delete(front);
      } else {
        sprite_update(back, buf, state->dt);
        sprite_update(front, buf, state->dt);
      }
      continue;
    }

    xpos = rand_minMax(0, buf->size.X-20);
    ypos = rand_minMax(0,1) * buf->size.Y;

    if (ypos == buf->size.Y) {
      ypos -= 15;
      front_id = SAW_FRONT_TOP_ID;
    } else {
      front_id = SAW_FRONT_BOTTOM_ID;
    }

    *back = sprite_new(aIMAGES+SAW_BACK_ID,
                       Vec2(xpos,ypos), Vec2(24,16), BUFFERCONSOLE_BILINEAR);
    back->yframe_count = 4;
    back->yframe_speed = 1.0;

    *front = sprite_new(aIMAGES+front_id,
                        Vec2(xpos,ypos), Vec2(24,16), BUFFERCONSOLE_BILINEAR);
    sprite_setCollisionCircle(front, Vec2(6,2), 12);
  }


}


/******************************************************************************/

/**
 * fill ennemy process
 */
void ennemy_fill(gameState_t *state)
{
  int i;

  assert(state);

  sprite_t *spr = state->sprites;
  bufferConsole_t *buf = &state->buffer;
  //keypad_t *keys = &state->keys;
  sprite_t *back,*front;

  for (i = 0 ; i < state->ennemy_max; ++i) {
    back = spr + SAW_BACK_ID+i;
    front = spr + SAW_FRONT_BOTTOM_ID+i;
    if (back->active) {
      sprite_fill(back, buf);
      sprite_fill(front, buf);
    }

  }

}


/******************************************************************************/

/**
 * game loop
 *
 * @param sprites - array of sprite_t
 * @param keys    - keypad used
 * @param buf     - buffer for the console
 * @return 0 = gameover, 1 = continue, -1 = error
 */
int game_open_loop(gameState_t *state, float dt)
{
  int i;
  sprite_t *spr = state->sprites;
  bufferConsole_t *buf = &state->buffer;
  keypad_t *keys = &state->keys;
  static int opacity = 0;
  if (keypad_update(keys)) return -1;

  /* start sfx */
  if (opacity == 0) {
    PlaySound("data/audio/musicloop.wav", NULL, SND_LOOP | SND_ASYNC);

    /* Init */
    state->ennemy_max = 4;
    state->ennemy_speed = 0.8;
    console_cls(buf->hnd);
    memset(state->sprites, 0, state->sprites_max);

    /* background */
    spr[BG1_ID] = sprite_new(aIMAGES+BG1_ID, Vec2(0,0), Vec2(190,60), 0);
    spr[BG1_ID].ydecay_ratio = aDECAY_BG1;
    spr[BG1_ID].decay_speed = 1.0;

    spr[BG2_ID] = sprite_new(aIMAGES+BG2_ID, Vec2(0,0), Vec2(190,60), 0);
    spr[BG2_ID].ydecay_ratio = aDECAY_BG2;
    spr[BG2_ID].decay_speed = 1.0;

    spr[BG3_ID] = sprite_new(aIMAGES+BG3_ID, Vec2(0,0), Vec2(190,60), 0);
    spr[BG3_ID].ydecay_ratio = aDECAY_BG3;
    spr[BG3_ID].decay_speed = 1.0;

    //aSPRITES[1] = sprite_new(0, 0, aIMAGES+BG1_ID, 160, 60, 0);
    //aSPRITES[2] = sprite_new(161, 0, aIMAGES+BG1_ID, 160, 60, 0);
    /*spr[BG1_ID] = sprite_new(0, 0, aIMAGES+BG1_ID, 160, 60, 0);
    spr[BG1_ID].ydecay_ratio = aDECAY_BG1;
    spr[BG1_ID].decay_speed = 1.0;*/

    /* main sprite */
    spr[BIRD_ID] = sprite_new(aIMAGES+BIRD_ID,
                              Vec2(80,-5), Vec2(40,60), BUFFERCONSOLE_BILINEAR);
    spr[BIRD_ID].yframe_count = 8;
    spr[BIRD_ID].yframe_speed = 0.3;
    //sprite_setCollisionSquare(spr+BIRD_ID, Vec2(16,27), Vec2(12,8));
    sprite_setCollisionCircle(spr+BIRD_ID, Vec2(17,27), 8);

    /*spr[BIRD_ID].col.x = 16;
    spr[BIRD_ID].col.y = 27;
    spr[BIRD_ID].col.width = 12;
    spr[BIRD_ID].col.height = 8;
    spr[BIRD_ID].col.active = 1;
    */
    spr[BIRD_ID].keypad_hook = &gGAMESTATE.keys;
    spr[BIRD_ID].scale = 10;
    //spr[BIRD_ID].gravity = 0.06;
  }


  /* background */
  sprite_update(spr+BG1_ID, buf, dt);
  sprite_fill(spr+BG1_ID, buf);


  /* background */
  /*sprite_update(spr+SAW_BACK_ID, buf, dt);
  sprite_fill(spr+SAW_BACK_ID, buf);
  sprite_update(spr+SAW_FRONT_BOTTOM_ID, buf, dt);
  sprite_fill(spr+SAW_FRONT_BOTTOM_ID, buf);
*/

  //ennemy_update(&gGAMESTATE);
  //ennemy_fill(&gGAMESTATE);



  opacity += 6;
  if (opacity >= 255) opacity = 255;

  if (spr[BIRD_ID].scale > 1.0) {
    /* bird intro */
    spr[BIRD_ID].scale -= 0.1;
    spr[BIRD_ID].pos.x = -5 + (spr[BIRD_ID].scale-1.0)*20;
    spr[BIRD_ID].pos.y = -5 + (spr[BIRD_ID].scale-1.0)*10;
    //if (spr[BIRD_ID].x <= -5.0) spr[BIRD_ID].x = -5.0;

  } else if (spr[BIRD_ID].scale <= 1.0) {
    spr[BIRD_ID].scale = 1.0;

    /* player action */
    if (keys->cur == VK_SPACE && keys->last != VK_SPACE) {
      spr[BIRD_ID].gravity = 0.06; /* à remettre */
      return 0;
    }

    /* maniabilité */
    if (keys->cur == VK_LEFT) spr[BIRD_ID].pos.x -= 1.0;
    if (keys->cur == VK_RIGHT) spr[BIRD_ID].pos.x += 1.0;
    if (keys->cur == VK_UP) spr[BIRD_ID].pos.y -= 1.0;
    if (keys->cur == VK_DOWN) spr[BIRD_ID].pos.y += 1.0;


    /* press space */
    static int id = 0;
    if (id++>8) {
      text_fill("press space key", aIMAGES+FONT_ID, buf,
                41,26, 8.0,RGBA32(128,128,0,0), 0);
      text_fill("press space key", aIMAGES+FONT_ID, buf,
                40,25, 8.0,RGBA32(255,255,0,0), 0);
      if (id>16) id = 0;
    }
    //return 0;
  }





  /* bird */
  sprite_update(spr+BIRD_ID, buf, dt);
  sprite_fill(spr+BIRD_ID, buf);


  /* fade-in */
  bufferConsole_fillFlatQuad(buf, RGBA32(255,255,255,255), opacity,
                             0<<12,0<<12, buf->size.X<<12, buf->size.Y<<12);

  /* blend */
  bufferConsole_fillFlatSubQuad(buf, state->blend[0], 255,
                                0<<12,0<<12, buf->size.X<<12, buf->size.Y<<12);



//color_lerpRGB(int32_t col1, int32_t col2, int32_t t)


#if 0
  /* main sprite */
  aSPRITES[0] = sprite_new(-5, -20, aIMAGES+BIRD_ID, 40, 60, 0);
  aSPRITES[0].yframe_count = 8;
  aSPRITES[0].yframe_speed = 0.3;
  aSPRITES[0].col.x = 16;
  aSPRITES[0].col.y = 27;
  aSPRITES[0].col.width = 12;
  aSPRITES[0].col.height = 8;
  aSPRITES[0].col.active = 1;
  aSPRITES[0].keypad_hook = &gGAMESTATE.keys;
  aSPRITES[0].gravity = 0.06;
#endif



#if 0
  /* first ennemy */
  aSPRITES[10] = sprite_new(gGAMESTATE.buffer.size.X - 10,
                                   rand_minMax(0, gGAMESTATE.buffer.size.Y - 16 - 4),
                                   aIMAGES+ENNEMY1_ID, 16, 16, 0);
  aSPRITES[10].xframe_count = 2;
  aSPRITES[10].xframe_speed = 0.3;
  aSPRITES[10].col.x = 0;
  aSPRITES[10].col.y = 0;
  aSPRITES[10].col.width = 16;
  aSPRITES[10].col.height = 16;
  aSPRITES[10].col.active = 1;
  aSPRITES[10].wind = -gGAMESTATE.ennemy_speed;
#endif

  return 1;
}


int game_update_loop(gameState_t *state, float dt)
{
  //static char str_score[64] = {0};
  static float transition = 0.0;
  int blend,distance,bg_id=BG1_ID;
  sprite_t *spr = state->sprites;
  bufferConsole_t *buf = &state->buffer;
  keypad_t *keys = &state->keys;

  if (keypad_update(keys)) return -1;


  /* player action */
  if (keys->cur == VK_SPACE && keys->last != VK_SPACE) {
    spr[BIRD_ID].col.vel.y = -1.1;
  }

  /* recreate ennemy */
  /*if (spr[10].x + spr[10].width < 0) {
    spr[10] = sprite_new(buf->size.X - 10, rand_minMax(0, buf->size.Y - 16 - 4),
                                aIMAGES+ENNEMY1_ID, 16, 16, 0);
    spr[10].yframe_count = 2;
    spr[10].yframe_speed = 0.3;
    spr[10].col.x = 0;
    spr[10].col.y = 0;
    spr[10].col.width = 16;
    spr[10].col.height = 16;
    spr[10].col.active = 1;
    //spr[10].wind = -state->ennemy_speed;
  }*/


  /* increase the difficulty */
  if (state->score[0] > 1000) state->ennemy_speed = 1.1;
  if (state->score[0] > 2000) state->ennemy_speed = 1.4;
  if (state->score[0] > 3000) state->ennemy_speed = 1.7;
  if (state->score[0] > 4000) state->ennemy_speed = 2.0;
  if (state->score[0] > 5000) state->ennemy_speed = 2.3;
  if (state->score[0] > 6000) state->ennemy_speed = 2.6;
  if (state->score[0] > 7000) state->ennemy_speed = 2.9;
  if (state->score[0] > 8000) state->ennemy_speed = 3.2;
  if (state->score[0] > 9000) state->ennemy_speed = 3.5;
  if (state->score[0] > 10000) state->ennemy_speed = 4.0;

  /* blend */
  state->distance += 0.5;
  //blend = rgba32_lerp(state->blend[0], state->blend[1], ((int)state->distance%255));

  if (state->distance < 255) {
    distance = 255 - (int)state->distance%255;
    blend = rgba32_lerp(state->blend[0], state->blend[1], 255-distance);
    bg_id = BG1_ID-1;
  }/* else if (state->distance < 255*2) {
    distance = 255 - (int)(state->distance-255)%255;
    blend = rgba32_lerp(state->blend[1], state->blend[2], 255-distance);
    bg_id = BG1_ID-1;
  } else if (state->distance < 255*3) {
    distance = 255 - (int)(state->distance-255*2)%255;
    blend = rgba32_lerp(state->blend[2], state->blend[3], 255-distance);
    bg_id = BG1_ID-1;
  } else if (state->distance < 255*4) {
    distance = 255 - (int)(state->distance-255*3)%255;
    blend = rgba32_lerp(state->blend[3], state->blend[4], 255-distance);
    bg_id = BG2_ID-1;
    transition = 255.0;
  } else if (state->distance < 255*5) {
    distance = 255 - (int)(state->distance-255*4)%255;
    blend = rgba32_lerp(state->blend[4], state->blend[5], 255-distance);
    bg_id = BG2_ID-1;
  } else if (state->distance < 255*6) {
    distance = 255 - (int)(state->distance-255*5)%255;
    blend = rgba32_lerp(state->blend[5], state->blend[6], 255-distance);
    bg_id = BG2_ID-1;
  } else if (state->distance < 255*7) {
    distance = 255 - (int)(state->distance-255*6)%255;
    blend = rgba32_lerp(state->blend[6], state->blend[7], 255-distance);
    bg_id = BG3_ID-1;
    transition = 255.0;
  } else if (state->distance < 255*8) {
    distance = 255 - (int)(state->distance-255*7)%255;
    blend = rgba32_lerp(state->blend[7], state->blend[8], 255-distance);
    bg_id = BG3_ID-1;
  } else if (state->distance < 255*9) {
    distance = 255 - (int)(state->distance-255*8)%255;
    blend = rgba32_lerp(state->blend[8], state->blend[9], 255-distance);
    bg_id = BG3_ID-1;
  } else if (state->distance < 255*10) {
    distance = 255 - (int)(state->distance-255*9)%255;
    blend = rgba32_lerp(state->blend[9], state->blend[9], 255-distance);
    bg_id = BG3_ID-1;
  } else {
    distance = 255;
    blend = state->blend[9];
    bg_id = BG3_ID-1;
  }*/

  /* background */
  if (bg_id >= BG1_ID) {
    spr[bg_id].opacity = 0;
    sprite_update(spr+bg_id, buf, dt);
    sprite_fill(spr+bg_id, buf);
  }

  if (bg_id < BG3_ID) {
    spr[bg_id+1].opacity = (int)transition;
    sprite_update(spr+bg_id+1, buf, dt);
    sprite_fill(spr+bg_id+1, buf);
  }

  transition -= 3.0;
  if (transition < 0.0) transition = 0.0;



  /* press space */
  /*static int id = 0;
  if (id++>8) {
    text_fill("press space key", aIMAGES+FONT_ID, buf,
                      40,26, 8.0,RGBA32(128,128,0,0), 0);
    text_fill("press space key", aIMAGES+FONT_ID, buf,
                      40,25, 8.0,RGBA32(255,255,0,0), 0);
    if (id>16) id = 0;
  }*/

  /* bird */
  sprite_update(spr+BIRD_ID, buf, dt);
  sprite_fill(spr+BIRD_ID, buf);



  bufferConsole_fillFlatSubQuad(buf, blend, 255,
                                0<<12,0<<12, buf->size.X<<12, buf->size.Y<<12);


//  bufferConsole_fillPlainQuad(buf, RGBA32(255,0,0,255), 1,67, 78,3);


  /* background */
  /*spr[1].wind = -state->ennemy_speed;
  sprite_update(spr+1, buf, dt);
  sprite_fill(spr+1, buf);
  if (spr[1].x + spr[1].width <= 0) spr[1].x = buf->size.X;

  spr[2].wind = -state->ennemy_speed;
  sprite_update(spr+2, buf, dt);
  sprite_fill(spr+2, buf);
  if (spr[2].x + spr[2].width <= 0) spr[2].x = buf->size.X;
*/

  /* */
  /*if (sprite_update(spr+0, buf, dt)) return 0;
  sprite_fill(spr+0, buf);

  sprite_update(spr+10, buf, dt);
  sprite_fill(spr+10, buf);

  if (sprite_collide(spr+0, spr+10)) return 0;
*/


  /* draw score */
  /*spr[0].score++;
  sprintf(str_score, " Score: ");
  text_fill(str_score, buf, 3, 2, 0x8f);
  sprintf(str_score, " %d ", spr[0].score);
  text_fill(str_score, buf, 11, 2, 0x8e);
*/

/*
  console_replace(buf->hnd);
  bufferConsole_update(buf);
  bufferConsole_swap(buf);
*/


  return spr[BIRD_ID].active;
}

int game_close_loop(gameState_t *state, float dt)
{

  return 0;
}


