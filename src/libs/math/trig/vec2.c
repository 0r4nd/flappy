
// Standard lib
#include <stdlib.h>
#include <math.h>


#include "vec2.h"




vec2 Vec2(float x, float y)
{
  vec2 ret;
  ret.x = x;
  ret.y = y;
  return ret;
}

vec2 vec2_null(void)
{
  vec2 ret;
  ret.x = 0.0f;
  ret.y = 0.0f;
  return ret;
}

int vec2_isNull (vec2 v)
{
  return v.ix | v.iy;
}

vec2 vec2_zero (void)
{
  vec2 ret;
  ret.x = 0.0f;
  ret.y = 0.0f;
  return ret;
}

vec2 vec2_one (void)
{
  vec2 ret;
  ret.x = 1.0f;
  ret.y = 1.0f;
  return ret;
}



