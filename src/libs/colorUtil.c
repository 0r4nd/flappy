
#include <math.h>
#include "colorUtil.h"

#include "../RGB555toCHARINFO.c"



static const char *css_colors[140*2] = {
  "AliceBlue", "F0F8FF",
  "AntiqueWhite", "FAEBD7",
  "Aqua", "00FFFF",
  "Aquamarine", "7FFFD4",
  "Azure", "F0FFFF",
  "Beige", "F5F5DC",
  "Bisque", "FFE4C4",
  "Black", "000000",
  "BlanchedAlmond", "FFEBCD",
  "Blue", "0000FF",
  "BlueViolet", "8A2BE2",
  "Brown", "A52A2A",
  "BurlyWood", "DEB887",
  "CadetBlue", "5F9EA0",
  "Chartreuse", "7FFF00",
  "Chocolate", "D2691E",
  "Coral", "FF7F50",
  "CornflowerBlue", "6495ED",
  "Cornsilk", "FFF8DC",
  "Crimson", "DC143C",
  "Cyan", "00FFFF",
  "DarkBlue", "00008B",
  "DarkCyan", "008B8B",
  "DarkGoldenRod", "B8860B",
  "DarkGray", "A9A9A9",
  "DarkGreen", "006400",
  "DarkKhaki", "BDB76B",
  "DarkMagenta", "8B008B",
  "DarkOliveGreen", "556B2F",
  "Darkorange", "FF8C00",
  "DarkOrchid", "9932CC",
  "DarkRed", "8B0000",
  "DarkSalmon", "E9967A",
  "DarkSeaGreen", "8FBC8F",
  "DarkSlateBlue", "483D8B",
  "DarkSlateGray", "2F4F4F",
  "DarkTurquoise", "00CED1",
  "DarkViolet", "9400D3",
  "DeepPink", "FF1493",
  "DeepSkyBlue", "00BFFF",
  "DimGray", "696969",
  "DodgerBlue", "1E90FF",
  "FireBrick", "B22222",
  "FloralWhite", "FFFAF0",
  "ForestGreen", "228B22",
  "Fuchsia", "FF00FF",
  "Gainsboro", "DCDCDC",
  "GhostWhite", "F8F8FF",
  "Gold", "FFD700",
  "GoldenRod", "DAA520",
  "Gray", "808080",
  "Green", "008000",
  "GreenYellow", "ADFF2F",
  "HoneyDew", "F0FFF0",
  "HotPink", "FF69B4",
  "IndianRed", "CD5C5C",
  "Indigo", "4B0082",
  "Ivory", "FFFFF0",
  "Khaki", "F0E68C",
  "Lavender", "E6E6FA",
  "LavenderBlush", "FFF0F5",
  "LawnGreen", "7CFC00",
  "LemonChiffon", "FFFACD",
  "LightBlue", "ADD8E6",
  "LightCoral", "F08080",
  "LightCyan", "E0FFFF",
  "LightGoldenRodYellow", "FAFAD2",
  "LightGrey", "D3D3D3",
  "LightGreen", "90EE90",
  "LightPink", "FFB6C1",
  "LightSalmon", "FFA07A",
  "LightSeaGreen", "20B2AA",
  "LightSkyBlue", "87CEFA",
  "LightSlateGray", "778899",
  "LightSteelBlue", "B0C4DE",
  "LightYellow", "FFFFE0",
  "Lime", "00FF00",
  "LimeGreen", "32CD32",
  "Linen", "FAF0E6",
  "Magenta", "FF00FF",
  "Maroon", "800000",
  "MediumAquaMarine", "66CDAA",
  "MediumBlue", "0000CD",
  "MediumOrchid", "BA55D3",
  "MediumPurple", "9370D8",
  "MediumSeaGreen", "3CB371",
  "MediumSlateBlue", "7B68EE",
  "MediumSpringGreen", "00FA9A",
  "MediumTurquoise", "48D1CC",
  "MediumVioletRed", "C71585",
  "MidnightBlue", "191970",
  "MintCream", "F5FFFA",
  "MistyRose", "FFE4E1",
  "Moccasin", "FFE4B5",
  "NavajoWhite", "FFDEAD",
  "Navy", "000080",
  "OldLace", "FDF5E6",
  "Olive", "808000",
  "OliveDrab", "6B8E23",
  "Orange", "FFA500",
  "OrangeRed", "FF4500",
  "Orchid", "DA70D6",
  "PaleGoldenRod", "EEE8AA",
  "PaleGreen", "98FB98",
  "PaleTurquoise", "AFEEEE",
  "PaleVioletRed", "D87093",
  "PapayaWhip", "FFEFD5",
  "PeachPuff", "FFDAB9",
  "Peru", "CD853F",
  "Pink", "FFC0CB",
  "Plum", "DDA0DD",
  "PowderBlue", "B0E0E6",
  "Purple", "800080",
  "Red", "FF0000",
  "RosyBrown", "BC8F8F",
  "RoyalBlue", "4169E1",
  "SaddleBrown", "8B4513",
  "Salmon", "FA8072",
  "SandyBrown", "F4A460",
  "SeaGreen", "2E8B57",
  "SeaShell", "FFF5EE",
  "Sienna", "A0522D",
  "Silver", "C0C0C0",
  "SkyBlue", "87CEEB",
  "SlateBlue", "6A5ACD",
  "SlateGray", "708090",
  "Snow", "FFFAFA",
  "SpringGreen", "00FF7F",
  "SteelBlue", "4682B4",
  "Tan", "D2B48C",
  "Teal", "008080",
  "Thistle", "D8BFD8",
  "Tomato", "FF6347",
  "Turquoise", "40E0D0",
  "Violet", "EE82EE",
  "Wheat", "F5DEB3",
  "White", "FFFFFF",
  "WhiteSmoke", "F5F5F5",
  "Yellow", "FFFF00",
  "YellowGreen", "9ACD32",
};


/******************************************************************************/

/**
 * bsearch_str - binary search an array of strings
 *
 * @param key - pointer to item being searched for
 * @param arr - pointer to first element to search
 * @param max - number of elements
 * @param dim - array dimensions (1,2,3)
 * @return id of array (-1 if not founded)
 */
static int bsearch_str(const char *key, const char **arr, int max, int dim)
{
  int cmp, mid, min=0;

  while (min < max) {
    mid = (min + max) >> 1;
    cmp = strcmp(key, arr[mid*dim]);

    if (cmp > 0) {
      min = mid + 1;
      continue;
    }
    if (cmp == 0) return mid;
    max = mid;
  }

  return -1;
}


/******************************************************************************/

/**
 * convert a hex string to uint
 *
 * @param hex - the string
 * @return unsigned int value
 */
static uint32_t hex2int(const char *hex)
{
  uint32_t ch,ret = 0;

  while (*hex) {
    ch = *hex;
    /* isdigit - isalpha_lo - isalpha_hi */
    if (ch-'0' <= ('9'-'0'))  ret += ch - '0';
    else if (ch-'a' <= ('f'-'a')) ret += (ch - 'a') + 10;
    else if (ch-'A' <= ('F'-'A')) ret += (ch - 'A') + 10;
    if (*++hex) ret <<= 4;
  }

  return ret;
}

static uint32_t swap_ui32(uint32_t val)
{
  val = ((val << 8) & 0xFF00FF00) | ((val>>8) & 0xFF00FF);
  return (val << 16) | (val >> 16);
}


/******************************************************************************/

/**
 * return a value based on css color name
 *
 * @param name - a css based color name. exemple : "Red"
 * @return -
 *
 * @note this function is case sensitive "White" != "white"
 */
uint32_t rgba32_css(const char *name)
{
  /* In my bench, the linear search is faster than bsearch */
  int id = bsearch_str(name, css_colors, 140, 2);
  if (id < 0) return 0 | (0xff<<24);
  return (swap_ui32(hex2int(css_colors[id*2+1]))>>8) | (0xff<<24);
}

uint32_t rgba16_css(const char *name)
{
  return color_RGBA32toRGBA16(rgba32_css(name));
}



/******************************************************************************/

/**
 * Linear interpolation between 2 colors
 *
 * @param col1 - RGB888 format
 * @param col2 - RGB888 format
 * @param t    - fraction [0 - 255]
 * @return -
 *
 */
int32_t rgba32_lerp(int32_t col1, int32_t col2, int32_t t)
{
  int32_t r1,g1,b1,a1, r2,g2,b2,a2;
  r1 = (col1)&0xff;
  g1 = (col1>>8)&0xff;
  b1 = (col1>>16)&0xff;
  a1 = (col1>>24)&0xff;
  r2 = (col2)&0xff;
  g2 = (col2>>8)&0xff;
  b2 = (col2>>16)&0xff;
  a2 = (col2>>24)&0xff;

  r1 += t*(r2-r1)>>8;
  g1 += t*(g2-g1)>>8;
  b1 += t*(b2-b1)>>8;
  a1 += t*(a2-a1)>>8;
  return r1 | (g1<<8) | (b1<<16) | (a1<<24);
}

int32_t rgba16_lerp(int32_t col1, int32_t col2, int32_t t)
{
  int32_t r1,g1,b1,a1, r2,g2,b2,a2;
  r1 = (col1)&0xf;
  g1 = (col1>>4)&0xf;
  b1 = (col1>>8)&0xf;
  a1 = (col1>>12)&0xf;
  r2 = (col2)&0xf;
  g2 = (col2>>4)&0xf;
  b2 = (col2>>8)&0xf;
  a2 = (col2>>12)&0xf;

  r1 += t*(r2-r1)>>4;
  g1 += t*(g2-g1)>>4;
  b1 += t*(b2-b1)>>4;
  a1 += t*(a2-a1)>>4;
  return r1 | (g1<<4) | (b1<<8) | (a1<<12);
}



uint32_t rgb555_clamp(int r, int g, int b)
{
    if ((r | g | b) & ~0x1f) {
        r = (r & ~0x1f)? ((~r)>>31)&0x1f : r;
        g = (g & ~0x1f)? ((~g)>>31)&0x1f : g;
        b = (b & ~0x1f)? ((~b)>>31)&0x1f : b;
    }
    return r | g<<5 | b<<10;
}

uint32_t rgb565_clamp(int r, int g, int b)
{
    if ((r<<1 | g | b<<1) & ~0x3f) {
        r = (r & ~0x1f)? ((~r)>>31)&0x1f : r;
        g = (g & ~0x3f)? ((~g)>>31)&0x3f : g;
        b = (b & ~0x1f)? ((~b)>>31)&0x1f : b;
    }
    return r | g<<5 | b<<11;
}

// rgba4444
uint32_t rgba16_clamp(int r, int g, int b, int a)
{
    if ((r | g | b | a) & ~0xf) {
        r = (r & ~0xf)? ((~r)>>31)&0xf : r;
        g = (g & ~0xf)? ((~g)>>31)&0xf : g;
        b = (b & ~0xf)? ((~b)>>31)&0xf : b;
        a = (a & ~0xf)? ((~a)>>31)&0xf : a;
    }
    return r | g<<4 | b<<8 | a<<12;
}

uint32_t rgb888_clamp(int r, int g, int b)
{
    if ((r | g | b) & ~0xff) {
        r = (r & ~0xff)? ((~r)>>31)&0xff : r;
        g = (g & ~0xff)? ((~g)>>31)&0xff : g;
        b = (b & ~0xff)? ((~b)>>31)&0xff : b;
    }
    return r | g<<8 | b<<16;
}

uint32_t rgba32_clamp(int r, int g, int b, int a)
{
    if ((r | g | b | a) & ~0xff) {
        r = (r & ~0xff)? ((~r)>>31)&0xff : r;
        g = (g & ~0xff)? ((~g)>>31)&0xff : g;
        b = (b & ~0xff)? ((~b)>>31)&0xff : b;
        a = (a & ~0xff)? ((~a)>>31)&0xff : a;
    }
    return r | g<<8 | b<<16 | a<<24;
}




/******************************************************************************/

/**
 * convert rgb to lab
 *
 * @param R - red component (0 to 255)
 * @param G - green component (0 to 255)
 * @param B - blue component (0 to 255)
 * @param out_lab - lab array
 * @return -
 *
 */
static void color_rgb2lab(int R, int G, int B, int *out_lab)
{
  /* http://www.brucelindbloom.com */
  float r, g, b, X, Y, Z, fx, fy, fz, xr, yr, zr;
  float Ls, as, bs;
  float eps = 216.0f / 24389.0f;
  float k = 24389.0f / 27.0f;

  float Xr = 0.964221f;  /* reference white D50 */
  float Yr = 1.0f;
  float Zr = 0.825211f;

  /* RGB to XYZ [0..255] to [0..1] */
  r = R / 255.0f;
  g = G / 255.0f;
  b = B / 255.0f;

  /* assuming sRGB (D65) */
  if (r <= 0.04045) r /= 12.92;
  else r = (float) pow((r + 0.055) / 1.055, 2.4);
  if (g <= 0.04045) g /= 12.92;
  else g = (float) pow((g + 0.055) / 1.055, 2.4);
  if (b <= 0.04045) b /= 12.92;
  else b = (float) pow((b + 0.055) / 1.055, 2.4);

  X = 0.436052025f * r + 0.385081593f * g + 0.143087414f * b;
  Y = 0.222491598f * r + 0.71688606f  * g + 0.060621486f * b;
  Z = 0.013929122f * r + 0.097097002f * g + 0.71418547f  * b;

  /* XYZ to Lab */
  xr = X / Xr;
  yr = Y / Yr;
  zr = Z / Zr;

  if (xr > eps) fx = (float) pow(xr, 1 / 3.0);
  else fx = (float) ((k * xr + 16.0) / 116.0);

  if (yr > eps) fy = (float) pow(yr, 1 / 3.0);
  else fy = (float) ((k * yr + 16.0) / 116.0);

  if (zr > eps) fz = (float) pow(zr, 1 / 3.0);
  else fz = (float) ((k * zr + 16.0) / 116.0);

  Ls = (116.0 * fy) - 16.0;
  as = 500.0 * (fx - fy);
  bs = 200.0 * (fy - fz);

  out_lab[0] = (int) (2.55 * Ls + 0.5);
  out_lab[1] = (int) (as + 0.5);
  out_lab[2] = (int) (bs + 0.5);
}


/******************************************************************************/

/**
 * Computes the difference between two RGB colors
 *
 * @param color1 - RGB888 format
 * @param color2 - RGB888 format
 * @return -
 *
 */
float color_diffLAB(uint32_t color1, uint32_t color2)
{
  uint32_t lab1[3], lab2[3];
  uint32_t r1,g1,b1, r2,g2,b2;
  r1 = (color1)&0xff;
  g1 = (color1>>8)&0xff;
  b1 = (color1>>16)&0xff;
  r2 = (color2)&0xff;
  g2 = (color2>>8)&0xff;
  b2 = (color2>>16)&0xff;
  color_rgb2lab(r1,g1,b1, lab1);
  color_rgb2lab(r2,g2,b2, lab2);

  lab2[0] -= lab1[0];
  lab2[1] -= lab1[1];
  lab2[2] -= lab1[2];
  return sqrtf(lab2[0]*lab2[0] + lab2[1]*lab2[1] + lab2[2]*lab2[2]);
}

float color_diffXYZ(uint32_t color1, uint32_t color2)
{
  float r1,g1,b1, r2,g2,b2;

  /* RGB to XYZ [0..255] to [0..1] */
  r1 = (float)((color1)&0xff) / 255.0;
  g1 = (float)((color1>>8)&0xff) / 255.0;
  b1 = (float)((color1>>16)&0xff) / 255.0;
  r2 = (float)((color2)&0xff) / 255.0;
  g2 = (float)((color2>>8)&0xff) / 255.0;
  b2 = (float)((color2>>16)&0xff) / 255.0;
  r2 -= r1;
  g2 -= g1;
  b2 -= b1;
  return sqrtf(r2*r2 + g2*g2 + b2*b2);
}


int color_rgb2yuv(int r, int g, int b)
{
  int ret;
  ret  = (int) (r *  0.299000 + g *  0.587000 + b *  0.114000);
  ret |= (int) (r * -0.168736 + g * -0.331264 + b *  0.500000 + 128) << 8;
  ret |= (int) (r *  0.500000 + g * -0.418688 + b * -0.081312 + 128) << 16;
  return ret;
}

int color_yuv2rgb(int y, int u, int v)
{
  int ret;
  ret  = (int) ( y + 1.4075 * (v - 128) );
  ret |= (int) ( y - 0.3455 * (u - 128) - (0.7169 * (v - 128)) ) << 8;
  ret |= (int) ( y + 1.7790 * (u - 128) ) << 16;
  return ret;
}
void color_split(int col,
                 int *out1, int *out2, int *out3)
{
  *out1 = (col)&0xff;
  *out2 = (col>>8)&0xff;
  *out3 = (col>>16)&0xff;
}


// YUV : PAL (standard allemand)
void color_RGB888toYUV888(int r, int g, int b,
                          float *out_y, float *out_u, float *out_v)
{
  float Y,U,V, R,G,B;

  R = (float)r;
  G = (float)g;
  B = (float)b;
 /*
 R = r / 255.0;
  G = g / 255.0;
  B = b / 255.0;

  Y = R * 0.299  + G * 0.587  + B * 0.114;
  U = R * -0.147 + G * -0.289 + B * 0.437;
  V = R * 0.615  + G * -0.515 + B * -0.100;
*/
  *out_y = R *  0.29900 + G *  0.58700 + B *  0.11400;
  *out_u = R * -0.16874 + G * -0.33126 + B *  0.50000 + 128;
  *out_v = R *  0.50000 + G * -0.41869 + B * -0.08131 + 128;

  /*Y *= 255.0;
  U *= 255.0;
  V *= 255.0;
  Y = (Y <= 0) ? 0 : (Y >= 255.0) ? 255.0 : Y;
  U = (U <= 0) ? 0 : (U >= 255.0) ? 255.0 : U;
  V = (B <= 0) ? 0 : (V >= 255.0) ? 255.0 : V;
*/
}
int color_YUV888toRGB888(float Y, float U, float V)
{
  float R,G,B;
  /*
  Y = y / 255.0;
  U = u / 255.0;
  V = v / 255.0;
  */

  /*
  R =  Y + (V * 0.140);
  G = -Y + (U * -0.394 + V * -0.581);
  B =  Y + (U * 2.028);
*/
  R = Y +                        (V - 128) *  1.40200;
  G = Y + (U - 128) * -0.34414 + (V - 128) * -0.71414;
  B = Y + (U - 128) *  1.77200;


  /*R *= 255.0;
  G *= 255.0;
  B *= 255.0;*/
  R = (R <= 0) ? 0 : (R >= 255.0) ? 255.0 : R;
  G = (G <= 0) ? 0 : (G >= 255.0) ? 255.0 : G;
  B = (B <= 0) ? 0 : (B >= 255.0) ? 255.0 : B;

  return (int)R |
         (int)G << 8 |
         (int)B << 16;
}




/******************************************************************************/

/**
 * Linear interpolation between 2 colors
 *
 * @param col1 - RGB888 format
 * @param col2 - RGB888 format
 * @param t    - fraction [0 - 255]
 * @return -
 *
 */
int32_t color_lerpRGB(int32_t col1, int32_t col2, int32_t t)
{
  int32_t r1,g1,b1, r2,g2,b2;
  r1 = (col1)&0xff;
  g1 = (col1>>8)&0xff;
  b1 = (col1>>16)&0xff;
  r2 = (col2)&0xff;
  g2 = (col2>>8)&0xff;
  b2 = (col2>>16)&0xff;
  r1 += t*(r2-r1)>>8;
  g1 += t*(g2-g1)>>8;
  b1 += t*(b2-b1)>>8;
  return r1 | (g1<<8) | (b1<<16);
}


/******************************************************************************/

/**
 * Linear interpolation between 2 colors (bad method)
 *
 * @param col1 - RGB888 format
 * @param col2 - RGB888 format
 * @param t    - fraction [0 - 1]
 * @return -
 *
 */
int32_t color_lerpRGBFl(int32_t col1, int32_t col2, float t)
{
  float r1,g1,b1, r2,g2,b2;
  r1 = (float) ((col1)&0xff);
  g1 = (float) ((col1>>8)&0xff);
  b1 = (float) ((col1>>16)&0xff);
  r2 = (float) ((col2)&0xff);
  g2 = (float) ((col2>>8)&0xff);
  b2 = (float) ((col2>>16)&0xff);

  r1 *= 0.299;
  g1 *= 0.587;
  b1 *= 0.114;

  r2 *= 0.299;
  g2 *= 0.587;
  b2 *= 0.114;

  r1 += t*(r2-r1);
  g1 += t*(g2-g1);
  b1 += t*(b2-b1);

  return (int)r1 | ((int)g1<<8) | ((int)b1<<16);
}


int32_t color_lerpYUV(int32_t col1, int32_t col2, float t)
{
  int r1,g1,b1, r2,g2,b2;
  float y1,u1,v1, y2,u2,v2;


  /* color to yuv */
  color_split(col1, &r1,&g1,&b1);
  color_RGB888toYUV888(r1,g1,b1, &y1,&u1,&v1);

  color_split(col2, &r2,&g2,&b2);
  color_RGB888toYUV888(r2,g2,b2, &y2,&u2,&v2);

  /* lerp */
  y1 += t*(y2-y1);
  u1 += t*(u2-u1);
  v1 += t*(v2-v1);

  //0.299000 + g *  0.587000 + b *  0.114000);

  return color_YUV888toRGB888(y1,u1,v1);

  //return color_lerpRGBFl(color_YUV888toRGB888(y1,u1,v1), color_YUV888toRGB888(y2,u2,v2), t);


  /* return the interpolated rgb color */
  //return color_yuv2rgb(y1,u1,v1);
}





uint32_t aCGA_COL[16] = {
/*  0x000000,
  0x800000,
  0x008000,
  0x808000,
  0x000080,
  0x800080,
  0x008080,
  0xc0c0c0,

  0x808080,
  0xff0000,
  0x00ff00,
  0xffff00,
  0x0000ff,
  0xff00ff,
  0x00ffff,
  0xffffff,*/

  RGBA32(0,0,0, 255),
  RGBA32(0,0,127, 255),
  RGBA32(0,127,0, 255),
  RGBA32(0,127,127, 255),
  RGBA32(127,0,0, 255),
  RGBA32(127,0,127, 255),
  RGBA32(127,127,0, 255),
  RGBA32(191,191,191, 255),

  RGBA32(127,127,127, 255),
  RGBA32(0,0,255, 255),
  RGBA32(0,255,0, 255),
  RGBA32(0,255,255, 255),
  RGBA32(255,0,0, 255),
  RGBA32(255,0,255, 255),
  RGBA32(255,255,0, 255),
  RGBA32(255,255,255, 255)
};



int colorArray_intensity(int col1, int col2,
                         int col1_cnt, int total_cnt)
{
  float r1,g1,b1, r2,g2,b2;
  int col2_cnt = total_cnt-col1_cnt;
  r1 = (float) ((col1)&0xff);
  g1 = (float) ((col1>>8)&0xff);
  b1 = (float) ((col1>>16)&0xff);

  r2 = (float) ((col2)&0xff);
  g2 = (float) ((col2>>8)&0xff);
  b2 = (float) ((col2>>16)&0xff);

  /* intensité totale de la couleur1 */
  r1 = (r1 * col1_cnt);
  g1 = (g1 * col1_cnt);
  b1 = (b1 * col1_cnt);

  /* intensité totale de la couleur2 */
  r2 = (r2 * col2_cnt);
  g2 = (g2 * col2_cnt);
  b2 = (b2 * col2_cnt);

  /* ajout des deux couleurs */
  r1 = (r1+r2)/**0.299*/;
  g1 = (g1+g2)/**0.587*/;
  b1 = (b1+b2)/**0.114*/;

  /* division par le total de pixels */
  r1 = r1 / total_cnt;
  g1 = g1 / total_cnt;
  b1 = b1 / total_cnt;

  /* clamp */
  r1 = (r1 <= 0) ? 0 : (r1 >= 255) ? 255 : r1;
  g1 = (g1 <= 0) ? 0 : (g1 >= 255) ? 255 : g1;
  b1 = (b1 <= 0) ? 0 : (b1 >= 255) ? 255 : b1;

  return (int)r1 | (int)g1<<8 | (int)b1<<16;
}



/**
 * http://www.benryves.com/tutorials/winconsole/2
 */
int color_toCHARINFO(uint32_t color_base, int8_t *out_ascii, uint8_t *out_color)
{
  int i,j,k;
  float tmp,diff = 1000000.0;
  int intensity=0, ret_intensity=0;
  uint32_t ascii=0,col1=0,col2=0;
  uint32_t id1=0, id2=0;

  //float (*color_diff)(uint32_t, uint32_t) = color_diffXYZ;
  float (*color_diff)(uint32_t, uint32_t) = color_diffLAB;

  /* bruteforce colors */
  for (j = 0; j < 16; ++j)
    for (i = j; i < 16; ++i) {
      /* total pixels of the first color with '\xDB' : 8x12 = 96 pixels (100%) */
      //tmp = color_diff(color_base,
      //                 color_lerpRGBFl(aCGA_COL[i], aCGA_COL[j], 0.0));
      intensity = colorArray_intensity(aCGA_COL[i], aCGA_COL[j], 96,96);
      tmp = color_diff(color_base, intensity);
      if (tmp < diff) { diff = tmp; col1 = i; col2 = j; ascii = '\xDB'; ret_intensity = intensity; }

      /* total pixels of the first color with '\xB0' : 64 pixels  (75%) */
/*
      intensity = colorArray_intensity(aCGA_COL[i], aCGA_COL[j], 64,96);
      tmp = color_diff(color_base, intensity);
      if (tmp < diff) { diff = tmp; col1 = i; col2 = j; ascii = '\xB2'; ret_intensity = intensity; }
*/

      /* total pixels of the first color with '\xB1' : 48 pixels  (50%) */
      intensity = colorArray_intensity(aCGA_COL[i], aCGA_COL[j], 48,96);
      tmp = color_diff(color_base, intensity);
      if (tmp < diff) { diff = tmp; col1 = i; col2 = j; ascii = '\xB1'; ret_intensity = intensity; }


      /* total pixels of the first color with '\xB2' : 32 pixels  (25%) */
/*
      intensity = colorArray_intensity(aCGA_COL[i], aCGA_COL[j], 24,96);
      tmp = color_diff(color_base, intensity);
      if (tmp < diff) { diff = tmp; col1 = i; col2 = j; ascii = '\xB0'; ret_intensity = intensity; }
*/

      /*  * total pixels of the first color with ' ' : 0 pixels */
      intensity = colorArray_intensity(aCGA_COL[i], aCGA_COL[j], 0,96);
      tmp = color_diff(color_base, intensity);
      if (tmp < diff) { diff = tmp; col1 = i; col2 = j; ascii = ' '; ret_intensity = intensity; }
    }

  *out_ascii = ascii;
  *out_color = col1 | col2<<4;
  return ret_intensity;
}


uint32_t color_RGB555toRGBA32(uint32_t col, uint32_t a)
{
  uint32_t r,g,b;
  r = (col&0x1f) << 3;
  g = ((col>>5)&0x1f) << 3;
  b = ((col>>10)&0x1f) << 3;
  return r | (g<<8) | (b<<16) | (a<<24);
}

uint32_t color_RGB888toRGB555(uint32_t col)
{
    uint32_t r,g,b;
    r = (col)&0xff;
    g = (col>>8)&0xff;
    b = (col>>16)&0xff;
    r = (r * 249 + 1014) >> 11;
    g = (g * 249 + 1014) >> 11;
    b = (b * 249 + 1014) >> 11;
    return r | (g<<5) | (b<<10);
}

uint32_t color_RGBA32toRGBA16(uint32_t col)
{
    uint32_t r,g,b,a;
    r = (col<< 8)&0xf000;
    g = (col>> 4)&0x0f00;
    b = (col>>16)&0x00f0;
    a = (col>>28)&0x000f;
    return r | g | b | a;
}



CHAR_INFO charinfo_fromRGB555(uint32_t col)
{
  if (col > 0x7FFF) col = 0x7FFF;
  return aRGB555toCHARINFO_LUT[col];
}

CHAR_INFO charinfo_fromRGB888(uint32_t col)
{
  col = color_RGB888toRGB555(col);
  return aRGB555toCHARINFO_LUT[col];
}
