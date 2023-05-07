
// standard materrial
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// img
#include "palette.h"

// DEBUG local, debug_printf("assert error!");
#include "../file.h"




/**
 * fourcc structure verification
 */
#define PAL_MAKEFOURCC(b0,b1,b2,b3) ((b0)|((b1)<<8)|((b2)<<16)|((b3)<<24))
#define PAL_MAKETWOCC(b0,b1)        ((b0)|((b1)<<8))


/**< Errors */
enum palette_err_e {
  PAL_EUNKN     = (1<<0),  /**< Unknow error */
  PAL_ENOENT    = (1<<1),  /**< No such file or directory */
  PAL_EIO       = (1<<2),  /**< I/O error */
  PAL_ENOMEM    = (1<<3),  /**< Out of memory */
  PAL_EINVAL    = (1<<4),  /**< Invalid argument */
  PAL_EMFILE    = (1<<5),  /**< Too many open files */
  PAL_EFBIG     = (1<<6),  /**< File too large */
  PAL_ESPIPE    = (1<<7),  /**< Illegal seek */
  PAL_ENOIMG    = (1<<8),  /**< Not a pal */
  PAL_ENOSTR    = (1<<9),  /**< Not a correct structure */
  PAL_ENODATA   = (1<<10), /**< No data available */
  PAL_EOVERFLOW = (1<<11), /**< Value too large for defined data type */
  PAL_EFORM     = (1<<12)  /**< Invalid format */
};


static const char *aPAL_ERR[16] = {
  "Unknow error",
  "No such file or directory",
  "I/O error",
  "Out of memory",
  "Invalid argument",
  "Too many open files",
  "File too large",
  "Illegal seek",
  "Not a palette ",
  "Not a correct structure",
  "No data available",
  "Value too large for defined data type",
  "Invalid format",
  "Unknow error",
  "Unknow error",
  ""
};


/* prototypes */
char *ascii_strcpy(char *out, const char *in);
int ascii_strlen(const char *s);
char *ascii_strcat(char *out, const char *in);
char *ascii_strbase10toa(int val, char *out);
char *ascii_extn(const char *fname, int n);
int ascii_isEqual(const char *s1, const char *s2);

/******************************************************************************/

/**
 * Convert a palette to rgb888 format
 * @param self - a palette_t structure
 * @return
 */
 uint32_t palette_setColor(palette_t *const self, uint32_t id,
                           uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
  if (NULL == self) return -1;
  /*
  switch (self->texImg.type) {
    case PAL_TYPE_RGB888: texImg_rgb888_set(self->texImg.data,id, r,g,b); break;
    case PAL_TYPE_BGR888: texImg_rgb888_set(self->texImg.data,id, b,g,r); break;
    case PAL_TYPE_RGBA32: texImg_rgba32_set(self->texImg.data,id, r,g,b,a); break;
    case PAL_TYPE_BGRA32: texImg_rgba32_set(self->texImg.data,id, b,g,r,a); break;
    case PAL_TYPE_RGB565: texImg_rgb565_set(self->texImg.data,id, r,g,b); break;
    case PAL_TYPE_BGR565: texImg_rgb565_set(self->texImg.data,id, b,g,r); break;
    case PAL_TYPE_RGB555: texImg_rgb555_set(self->texImg.data,id, r,g,b); break;
    case PAL_TYPE_BGR555: texImg_rgb555_set(self->texImg.data,id, b,g,r); break;
    case PAL_TYPE_RGB5551: texImg_rgba5551_set(self->texImg.data,id, r,g,b,a); break;
    case PAL_TYPE_BGR5551: texImg_rgba5551_set(self->texImg.data,id, b,g,r,a); break;
    case PAL_TYPE_RGB444: texImg_rgb444_set(self->texImg.data,id, r,g,b); break;
    case PAL_TYPE_BGR444: texImg_rgb444_set(self->texImg.data,id, b,g,r); break;
    case PAL_TYPE_RGBA4444: texImg_rgba4444_set(self->texImg.data,id, r,g,b,a); break;
    case PAL_TYPE_BGRA4444: texImg_rgba4444_set(self->texImg.data,id, b,g,r,a); break;
    case PAL_TYPE_RGB332: texImg_rgb332_set(self->texImg.data,id, r,g,b); break;
    case PAL_TYPE_BGR332: texImg_rgb332_set(self->texImg.data,id, b,g,r); break;
    default: return -1;
  }*/
  return 0;
}



uint32_t palette_getColor(palette_t *const self, uint32_t id)
{
  if (NULL == self) return 0;
  /*
  switch (self->texImg.type) {
    case PAL_TYPE_RGB888:
    case PAL_TYPE_BGR888:
    case PAL_TYPE_RGBA32:
    case PAL_TYPE_BGRA32:
      return texImg_get32(self->texImg.data,id);
    case PAL_TYPE_RGB565:
    case PAL_TYPE_BGR565:
    case PAL_TYPE_RGB555:
    case PAL_TYPE_BGR555:
    case PAL_TYPE_RGB5551:
    case PAL_TYPE_BGR5551:
    case PAL_TYPE_RGB444:
    case PAL_TYPE_BGR444:
    case PAL_TYPE_RGBA4444:
    case PAL_TYPE_BGRA4444:
      return texImg_get16(self->texImg.data,id);
    case PAL_TYPE_RGB332:
    case PAL_TYPE_BGR332:
      return texImg_get8(self->texImg.data,id);
  }*/
  return 0;
}

uint32_t palette_getR(palette_t *const self, uint32_t id)
{
  if (NULL == self) return 0;
  /*
  switch (self->texImg.type) {
    case PAL_TYPE_RGB888: return texImg_rgb888_getR(self->texImg.data,id);
    case PAL_TYPE_BGR888: return texImg_rgb888_getB(self->texImg.data,id);
    case PAL_TYPE_RGBA32: return texImg_rgb888_getR(self->texImg.data,id);
    case PAL_TYPE_BGRA32: return texImg_rgb888_getB(self->texImg.data,id);
    case PAL_TYPE_RGB565: return texImg_rgb565_getR(self->texImg.data,id);
    case PAL_TYPE_BGR565: return texImg_rgb565_getB(self->texImg.data,id);
    case PAL_TYPE_RGB555: return texImg_rgb555_getR(self->texImg.data,id);
    case PAL_TYPE_BGR555: return texImg_rgb555_getB(self->texImg.data,id);
    case PAL_TYPE_RGB5551: return texImg_rgb555_getR(self->texImg.data,id);
    case PAL_TYPE_BGR5551: return texImg_rgb555_getB(self->texImg.data,id);
    case PAL_TYPE_RGB444: return texImg_rgb444_getR(self->texImg.data,id);
    case PAL_TYPE_BGR444: return texImg_rgb444_getB(self->texImg.data,id);
    case PAL_TYPE_RGBA4444: return texImg_rgb444_getR(self->texImg.data,id);
    case PAL_TYPE_BGRA4444: return texImg_rgb444_getB(self->texImg.data,id);
    case PAL_TYPE_RGB332: return texImg_rgb332_getR(self->texImg.data,id);
    case PAL_TYPE_BGR332: return texImg_rgb332_getB(self->texImg.data,id);
  }*/
  return 0;
}

uint32_t palette_getG(palette_t *const self, uint32_t id)
{
  if (NULL == self) return 0;
  /*
  switch (self->texImg.type) {
    case PAL_TYPE_RGB888:
    case PAL_TYPE_BGR888: return texImg_rgb888_getG(self->texImg.data,id);
    case PAL_TYPE_RGBA32:
    case PAL_TYPE_BGRA32: return texImg_rgb888_getG(self->texImg.data,id);
    case PAL_TYPE_RGB565:
    case PAL_TYPE_BGR565: return texImg_rgb565_getG(self->texImg.data,id);
    case PAL_TYPE_RGB555:
    case PAL_TYPE_BGR555: return texImg_rgb555_getG(self->texImg.data,id);
    case PAL_TYPE_RGB5551:
    case PAL_TYPE_BGR5551: return texImg_rgb555_getG(self->texImg.data,id);
    case PAL_TYPE_RGB444:
    case PAL_TYPE_BGR444: return texImg_rgb444_getG(self->texImg.data,id);
    case PAL_TYPE_RGBA4444:
    case PAL_TYPE_BGRA4444: return texImg_rgb444_getG(self->texImg.data,id);
    case PAL_TYPE_RGB332:
    case PAL_TYPE_BGR332: return texImg_rgb332_getG(self->texImg.data,id);
  }*/
  return 0;
}

uint32_t palette_getB(palette_t *const self, uint32_t id)
{
  if (NULL == self) return 0;
  /*
  switch (self->texImg.type) {
    case PAL_TYPE_RGB888: return texImg_rgb888_getB(self->texImg.data,id);
    case PAL_TYPE_BGR888: return texImg_rgb888_getR(self->texImg.data,id);
    case PAL_TYPE_RGBA32: return texImg_rgb888_getB(self->texImg.data,id);
    case PAL_TYPE_BGRA32: return texImg_rgb888_getR(self->texImg.data,id);
    case PAL_TYPE_RGB565: return texImg_rgb565_getB(self->texImg.data,id);
    case PAL_TYPE_BGR565: return texImg_rgb565_getR(self->texImg.data,id);
    case PAL_TYPE_RGB555: return texImg_rgb555_getB(self->texImg.data,id);
    case PAL_TYPE_BGR555: return texImg_rgb555_getR(self->texImg.data,id);
    case PAL_TYPE_RGB5551: return texImg_rgb555_getB(self->texImg.data,id);
    case PAL_TYPE_BGR5551: return texImg_rgb555_getR(self->texImg.data,id);
    case PAL_TYPE_RGB444: return texImg_rgb444_getB(self->texImg.data,id);
    case PAL_TYPE_BGR444: return texImg_rgb444_getR(self->texImg.data,id);
    case PAL_TYPE_RGBA4444: return texImg_rgb444_getB(self->texImg.data,id);
    case PAL_TYPE_BGRA4444: return texImg_rgb444_getR(self->texImg.data,id);
    case PAL_TYPE_RGB332: return texImg_rgb332_getB(self->texImg.data,id);
    case PAL_TYPE_BGR332: return texImg_rgb332_getR(self->texImg.data,id);
  }*/
  return 0;
}


/******************************************************************************/

/**
 * Convert a palette to rgb888 format
 * @param self - a palette_t structure
 * @return
 */
int palette_convertToRGB888(palette_t *const self)
{
  if (NULL == self) return -1;
  if (NULL == self->texImg.data) return -1;
/*
  switch (self->texImg.type) {
    case PAL_TYPE_RGB888: break;
    case PAL_TYPE_RGBA32: texImg_RGBA32toRGB888(self->texImg.data, self->size); break;
    case PAL_TYPE_BGR888: texImg_RGB888toBGR888(self->texImg.data, self->size); break;
    case PAL_TYPE_BGRA32: texImg_RGBA32toBGR888(self->texImg.data, self->size); break;
    case PAL_TYPE_RGB565: break;
    case PAL_TYPE_BGR565: break;
    case PAL_TYPE_RGB555: break;
    case PAL_TYPE_BGR555: break;
    case PAL_TYPE_RGB5551: break;
    case PAL_TYPE_BGR5551: break;
    case PAL_TYPE_RGB444: break;
    case PAL_TYPE_BGR444: break;
    case PAL_TYPE_RGBA4444: break;
    case PAL_TYPE_BGRA4444: break;
    case PAL_TYPE_RGB332: break;
    case PAL_TYPE_BGR332: break;
  }*/

  return 0;
}


/******************************************************************************/

/**
 * Convert a palette to bgr888 format
 * @param self - a palette_t structure
 * @return
 */
int palette_convertToBGR888(palette_t *const self)
{
  if (NULL == self) return -1;
  if (NULL == self->texImg.data) return -1;
/*
  switch (self->texImg.type) {
    case PAL_TYPE_BGR888:
    case PAL_TYPE_BGRA32: break;
    case PAL_TYPE_RGB888:
    case PAL_TYPE_RGBA32: texImg_RGBA32toBGRA32(self->texImg.data, self->texImg.xcount); break;
    case PAL_TYPE_RGB565:  break;
    case PAL_TYPE_BGR565:  break;
    case PAL_TYPE_RGB555:  break;
    case PAL_TYPE_BGR555:  break;
    case PAL_TYPE_RGB5551:  break;
    case PAL_TYPE_BGR5551:  break;
    case PAL_TYPE_RGB444:  break;
    case PAL_TYPE_BGR444:  break;
    case PAL_TYPE_RGBA4444:  break;
    case PAL_TYPE_BGRA4444:  break;
    case PAL_TYPE_RGB332:  break;
    case PAL_TYPE_BGR332:  break;
  }*/

  return 0;
}


/******************************************************************************/

/*!
 * export the pixels type
 * @return
 */
char *palette_type_toString(palette_t *const self, char *s)
{
  if (NULL == self || NULL == s) return s;
  s[0] = '\0';
/*
  if (NULL != self) {
    switch ((self->texImg.flags>>PAL_TYPE_SHIFT)&PAL_TYPE_MASK) {
      case PAL_TYPE_RGB888: ascii_strcpy(s,"RGB888"); break;
      case PAL_TYPE_BGR888: ascii_strcpy(s,"BGR888"); break;
      case PAL_TYPE_RGBA32: ascii_strcpy(s,"RGBA32"); break;
      case PAL_TYPE_BGRA32: ascii_strcpy(s,"BGRA32"); break;
      case PAL_TYPE_RGB565: ascii_strcpy(s,"RGB565"); break;
      case PAL_TYPE_BGR565: ascii_strcpy(s,"BGR565"); break;
      case PAL_TYPE_RGB555: ascii_strcpy(s,"RGB555"); break;
      case PAL_TYPE_BGR555: ascii_strcpy(s,"BGR555"); break;
      case PAL_TYPE_RGB5551: ascii_strcpy(s,"RGB5551"); break;
      case PAL_TYPE_BGR5551: ascii_strcpy(s,"BGR5551"); break;
      case PAL_TYPE_RGB444: ascii_strcpy(s,"RGB444"); break;
      case PAL_TYPE_BGR444: ascii_strcpy(s,"BGR444"); break;
      case PAL_TYPE_RGBA4444: ascii_strcpy(s,"RGBA4444"); break;
      case PAL_TYPE_BGRA4444: ascii_strcpy(s,"BGRA4444"); break;
      case PAL_TYPE_RGB332: ascii_strcpy(s,"RGB332"); break;
      case PAL_TYPE_BGR332: ascii_strcpy(s,"BGR332"); break;
    }
  }*/

  return s;
}

#if 0
/******************************************************************************/

/*!
 * Create a new palette_t struct
 * @return
 */
char *palette_color_toString(palette_t *const self,
                             char *s, char *space, uint32_t id)
{
  if (NULL == self || NULL == s) return s;
  if (id > 255) return s;
  s[0] = '\0';

/*
  if (NULL != self) {
    switch ((self->texImg.flags>>PAL_TYPE_SHIFT)&PAL_TYPE_MASK) {
      case PAL_TYPE_RGB888:
      case PAL_TYPE_BGR888:
        texImg_rgb888_toString(self->texImg.data,s,space,id);
        break;
      case PAL_TYPE_RGBA32:
      case PAL_TYPE_BGRA32:
        texImg_rgba32_toString(self->texImg.data,s,space,id);
        break;
      case PAL_TYPE_RGB565:
      case PAL_TYPE_BGR565:
        texImg_rgb565_toString(self->texImg.data,s,space,id);
        break;
      case PAL_TYPE_RGB555:
      case PAL_TYPE_BGR555:
        texImg_rgb555_toString(self->texImg.data,s,space,id);
        break;
      case PAL_TYPE_RGB5551:
      case PAL_TYPE_BGR5551:
        texImg_rgba5551_toString(self->texImg.data,s,space,id);
        break;
      case PAL_TYPE_RGB444:
      case PAL_TYPE_BGR444:
        texImg_rgb444_toString(self->texImg.data,s,space,id);
        break;
      case PAL_TYPE_RGBA4444:
      case PAL_TYPE_BGRA4444:
        texImg_rgba4444_toString(self->texImg.data,s,space,id);
        break;
      case PAL_TYPE_RGB332:
      case PAL_TYPE_BGR332:
        texImg_rgb332_toString(self->texImg.data,s,space,id);
        break;
    }
  }*/

  return s;
}
#endif

/******************************************************************************/

/*!
 * @param
 * @param
 * @return
 */
static int palette_isspace(int ch)
{
  static const int aISSPACE_RAW_BITS[8] = { 15872,1,0,0,0,0,0,0 };
  //return ch == ' ' || ch == '\t' || ch == '\n' ||
  //       ch == '\v' || ch == '\f' || ch == '\r';
  return (aISSPACE_RAW_BITS[ch>>5] >> (ch&31)) & 1;
}


/******************************************************************************/

/*!
 * @param
 * @param
 * @return number of skiped spaces
 */
static int palette_skip_whitespaces(const char **in)
{
  const char *base = *in;
  while (palette_isspace(**in)) ++(*in);
  return (int)(*in - base);
}


/******************************************************************************/

/*!
 * @param
 * @param
 * @return value of strncmp between in and str
 */
static int palette_skip_cmp(const char **in, const char *str)
{
  size_t len = strlen(str);
  int ret = strncmp(*in,str,len);
  if (ret == 0) (*in) += len;
  return ret;
}

/******************************************************************************/

/*!
 * @param
 * @param
 * @return
 */
static int palette_isdigit(int ch)
{
  static const int aISDIGIT_BITS[8] = {0,67043328,0,0,0,0,0,0};
  // return ch >= '0' && ch <= '9';
  return (aISDIGIT_BITS[ch>>5] >> (ch&31)) & 1;
}


/******************************************************************************/

/**
 *
 * @param in -    the string to parse
 * @param pos -   return the incremention number
 * @param count - len of the part
 * @param len -   max len of the entire string
 *
 */
static int palette_iswhitespace(const char *in, int len)
{
  int pos = 0;
  if ((pos += palette_skip_whitespaces(&in)) > len) return 1;
  if ('\0' != *in) return 0;
  return 1;
}


/******************************************************************************/

/**
 * is N max or less digits ?
 *
 * @param in the string to parse
 * @param pos return the incremention number
 * @param val return value of the digit (if is a  digit)
 * @param count len MAX of the part
 * @param len  max len of the entire string
 *
 */
static int palette_is_maxOrLessNdigits(const char **in, int *pos, int *val,
                                       int n, int len)
{
  int i,num=0;
  *val = 0;

  for (i=0; i<n; i++) {
    if (0 == palette_isdigit(**in)) break;
    num = num*10 + (**in)-'0';

    (*pos)++;
    if (*pos > len) return 0;
    (*in)++;
  }

  if (i == 0) return 0;
  *val = num;
  return 1;
}



/******************************************************************************/

/**
 * Read pixels
 * @return errors
 */
static int pal_read_pix(palette_t *self)
{
  int len,pos,val = 0;
  int rgba_count = 0;
  char buf[256];
  const char *pbuf;
  uint8_t *pdata = (uint8_t*)self->texImg.data;

  if (0 != stream_seek(&self->src, self->src.fbeg, SEEK_SET)) return PAL_ESPIPE;

  for (pos = 0; NULL != stream_gets(buf,sizeof(buf),&self->src); pos = 0) {
    len = strlen(buf);
    pbuf = buf;

    /* r */
    if ((pos += palette_skip_whitespaces(&pbuf)) > len) goto error_form;;
    if (0 == palette_is_maxOrLessNdigits(&pbuf,&pos,&val, 3,len)) goto error_form;
    if (!palette_isspace(*pbuf)) goto error_form;
    if (val < 0 || val > 255) goto error_form;
    pdata[0] = val;

    /* g */
    if ((pos += palette_skip_whitespaces(&pbuf)) > len) goto error_form;
    if (0 == palette_is_maxOrLessNdigits(&pbuf,&pos,&val, 3,len)) goto error_form;
    if (!palette_isspace(*pbuf)) goto error_form;
    if (val < 0 || val > 255) goto error_form;
    pdata[1] = val;

    /* b */
    if ((pos += palette_skip_whitespaces(&pbuf)) > len) goto error_form;
    if (0 == palette_is_maxOrLessNdigits(&pbuf,&pos,&val, 3,len)) goto error_form;
    if (!palette_isspace(*pbuf)) goto error_form;
    if (val < 0 || val > 255) goto error_form;
    pdata[2] = val;

    /* a */
    //pdata[3] = 255;

    //printf("%u: %u,%u,%u\n", dd++, pdata[0], pdata[1], pdata[2]);
    pdata += 3;
    /* Break the loop when we exceed the max size allocated */
    if (++rgba_count > self->texImg.xcount) break;
  }

  /* good end */
  return 0;

  /* bad end */
  error_form:
  texImg_delete(&self->texImg);
  self->src.errors |= PAL_EFORM;
  return self->src.errors;
}


/******************************************************************************/

/**
 * Read pixels
 * @return errors
 */
static int pal_read(palette_t *self)
{
  int len,pos,val = 0;
  int rgba_count = 0;
  char buf[256];
  const char *pbuf;
  uint8_t *pdata;

  /* Assert */
  if (NULL == self || NULL == self->src.file) {
    self->src.errors = PAL_ENODATA;
    return self->src.errors;
  }

  /* jasc-pal header */
  for (pos = 0; NULL != stream_gets(buf,sizeof(buf),&self->src); pos = 0) {
    len = strlen(buf);
    pbuf = buf;
    if (!palette_iswhitespace(pbuf,len)) break;
  }

  if ((pos += palette_skip_whitespaces(&pbuf)) > len) goto error_form;
  if (0 == palette_skip_cmp(&pbuf, "JASC-PAL"))
    if (!palette_iswhitespace(pbuf, len)) goto error_form;


  /* "four-cc" : 0100 */
  for (pos = 0; NULL != stream_gets(buf,sizeof(buf),&self->src); pos = 0) {
    len = strlen(buf);
    pbuf = buf;
    if (!palette_iswhitespace(pbuf,len)) break;
  }
  if (0 == palette_skip_cmp(&pbuf, "0100")) {
    if (!palette_iswhitespace(pbuf, len)) goto error_form;
  } else goto error_form;


  /* get palette size */
  for (pos = 0; NULL != stream_gets(buf,sizeof(buf),&self->src); pos = 0) {
    len = strlen(buf);
    pbuf = buf;
    if (!palette_iswhitespace(pbuf,len)) break;
  }
  if ((pos += palette_skip_whitespaces(&pbuf)) > len) goto error_form;
  if (0 == palette_is_maxOrLessNdigits(&pbuf,&pos,&val, 3,len)) goto error_form;
  if (!palette_iswhitespace(pbuf,len)) goto error_form;

  /* load pal data's */
  self->src.fbeg = stream_tell(&self->src);
  /*if (0 != stream_seek(&self->src, 0, SEEK_END)) return PAL_ESPIPE;
  self->src.size = stream_tell(&self->src) - self->src.fbeg;*/
  //if (0 != stream_seek(&self->src, self->src.fbeg, SEEK_SET)) return PAL_ESPIPE;
  //pdata = (uint8_t*)self->texImg.data;

  /* verification and allocation */
  if (val < 2 || val > 256) {
    self->src.errors = PAL_EFORM;
    return self->src.errors;
  }

  self->texImg = texImg1D_new(TIMG_TEXTURE_1D, TIMG_RGB, val, TIMG_RGB, TIMG_BYTE, NULL);
  if (texImg_isNull(&self->texImg)) {
    self->src.errors = PAL_EFORM;
    return self->src.errors;
  }

  if (0 != pal_read_pix(self)) return self->src.errors;

  /* good end */
  return 0;

  /* bad end */
  error_form:
  //texImg_delete(&self->texImg);
  self->src.errors |= PAL_EFORM;
  return PAL_EFORM;
}


/******************************************************************************/

/**
 * Write .pal (JASC) file
 * @return errors
 */
static int pal_write(palette_t *self)
{
  uint32_t i,size;
  char buf[32];

  /* Assert */
  if (NULL == self || NULL == self->src.file) {
    self->src.errors = PAL_ENODATA;
    return self->src.errors;
  }

  /* jasc-pal header */
  if (EOF == stream_puts("JASC-PAL\n", &self->src)) goto error_io;
  if (EOF == stream_puts("0100\n", &self->src)) goto error_io;

  /* size */
  ascii_strbase10toa(self->texImg.xcount,buf);
  ascii_strcat(buf,"\n");
  if (EOF == stream_puts(buf,&self->src)) goto error_io;

  for (i = 0; i < self->texImg.xcount ; ++i) {
    //palette_color_toString(self,buf," ",i);
    texImg_color_toString(self,buf," ",i);
    ascii_strcat(buf,"\n");
    if (EOF == stream_puts(buf,&self->src)) goto error_io;
  }

  /* good end */
  return 0;

  /* bad end */
  error_io:
  self->src.errors = PAL_EIO;
  return PAL_EIO;
}


/******************************************************************************/

/**
 * Write .gpl (GIMP) file
 * @return errors
 */
static int gpl_write(palette_t *self, char *name)
{
  uint32_t i,size;
  char buf[128];

  /* Assert */
  if (NULL == self || NULL == self->src.file) {
    self->src.errors = PAL_ENODATA;
    return self->src.errors;
  }

  /* gimp header */
  if (EOF == (stream_puts("GIMP Palette\n", &self->src))) goto error_io;
  if (NULL != name) {
    if (EOF == (stream_puts("Name: ", &self->src))) goto error_io;
    ascii_strcpy(buf,name);
    ascii_strcat(buf,"\n#\n");
  } else {
    ascii_strcpy(buf,"#\n");
  }
  if (EOF == (stream_puts(buf, &self->src))) goto error_io;


  for (i = 0; i < self->texImg.xcount ; ++i) {
    //palette_color_toString(self,buf," ",i);
    texImg_color_toString(self,buf," ",i);
    ascii_strcat(buf," Untitled\n");
    if (EOF == (stream_puts(buf, &self->src))) goto error_io;
  }

  /* good end */
  return 0;

  /* bad end */
  error_io:
  self->src.errors = PAL_EIO;
  return PAL_EIO;
}


/******************************************************************************/

/**
 * Open and read/write a file/buffer
 * @param iout file or buffer
 * @param img an img stucture to import or export
 * @param mode  file: "r" or "w"
 *              mem:  "rm" or "wm"
 *              file stream: "rs" or "ws"
 *              mem stream: "rms" or "wms"
 * @return errors
 */
static int palette_open(const char *inout, palette_t *pal, const char *format)
{
  palette_t tmp;
  char *ext;
  texImg_memset(&tmp,0,sizeof(tmp));

  // assert
  if (NULL == inout || NULL == format || '\0' == format[0]) {
    pal->src.errors = PAL_EINVAL;
    return pal->src.errors;
  }

  /* read mode */
  if ('r' == format[0]) {
    if (pal != &tmp) *pal = tmp;

  /* write mode */
  } else if ('w' == format[0]) {

    /**< In write mode we need a structure */
    if (pal == &tmp) {
      pal->src.errors = PAL_EINVAL | PAL_ENODATA;
      return pal->src.errors;
    }

  /* Args error */
  } else {
    pal->src.errors = PAL_EINVAL;
    return pal->src.errors;
  }


  /* File mode. */
  if ('\0' == format[1] || 's' == format[1]) {

    if (NULL == (pal->src.file = file_open(inout, format))) {
      pal->src.errors = PAL_ENOENT;
      return pal->src.errors;
    }
    pal->src.flags = STREAM_FLAG_FILE | STREAM_FLAG_TEXT;
    if ('s' != format[1]) pal->src.flags |= STREAM_FLAG_DIRECTCOPY;

  /* Buffer mode. */
  } else if ('m' == format[1]) {
    pal->src.file = (void*)inout;
    pal->src.flags = STREAM_FLAG_BUFFER | STREAM_FLAG_TEXT;
    if ('s' != format[2]) pal->src.flags |= STREAM_FLAG_DIRECTCOPY;

  /* Args error */
  } else {
    pal->src.errors = PAL_EINVAL;
    return pal->src.errors;
  }

  // Accès lecture ou écriture.
  switch (*(uint16_t*)format) {

  case PAL_MAKETWOCC('r','s'):
  case PAL_MAKETWOCC('r','m'):
  case PAL_MAKETWOCC('r','\0'):
    //pal->errors = pal_read(pal);

    ext = ascii_extn(inout,0);
    if (ascii_isEqual(ext, "pal")) {
      pal->src.errors = pal_read(pal);
    } else if (ascii_isEqual(ext, "gpl")) {
      //pal->errors = gpl_read(pal);
      pal->src.errors = PAL_ENOIMG;
    } else {
      pal->src.errors = PAL_ENOIMG;
    }

    if (0 != pal->src.errors) {
      stream_close(&pal->src);
      return pal->src.errors;
    }
    break;

    //if (in == img_pix) img->pix = img_pix;
    //else /*tga_mread(pal, img_pix, texture_flags)*/;
    //break;

  case PAL_MAKETWOCC('w','s'):
  case PAL_MAKETWOCC('w','m'):
  case PAL_MAKETWOCC('w','\0'):

    ext = ascii_extn(inout,0);
    if (ascii_isEqual(ext, "pal")) {
      pal->src.errors = pal_write(pal);
    } else if (ascii_isEqual(ext, "gpl")) {
      pal->src.errors = gpl_write(pal,NULL);
    } else {
      pal->src.errors = PAL_ENOIMG;
    }

    if (0 != pal->src.errors) {
      stream_close(&pal->src);
      return pal->src.errors;
    }
    break;

  default: // unknow flag
    stream_close(&pal->src);
    pal->src.errors = PAL_EINVAL;
    return pal->src.errors;
  }


  /* close the file */
  stream_close(&pal->src);
  return 0;
}






/******************************************************************************/

/**
 * callbacks events.
 * This event occurs when an error occurred during the loading of an img.
 *
 * @param self - current structure
 * @return .
 */
void palette_onerror_print(palette_t *const self)
{
  int i;
  if (NULL == self) return;
  if (0 == self->src.errors) return;

  printf("Palette ErrCodes:\n");
  for (i = 0; i < 15; ++i)
    if (self->src.errors & (1 << i))
      printf("- %s\n", aPAL_ERR[i]);

}


/******************************************************************************/

/**
 * Check is an error occured
 *
 * @param self - a palette_t structure
 * @return true:error.  false:good
 */
int palette_isError(palette_t *const self)
{
  if (NULL == self) return 0;
  return self->src.errors;
}


/******************************************************************************/

/*!
 * Create a new palette_t struct
 * @return
 */
palette_t palette_new(uint8_t internalFormat, uint16_t size,
                      uint8_t format, uint8_t type, void *data)
                      //void *data, uint16_t flags, uint16_t size)
{
  palette_t ret;
  texImg_memset(&ret,0,sizeof(ret));
/*
  switch (flags) {
    case PAL_TYPE_RGB888:
    case PAL_TYPE_BGR888: //ret.bpc = 24; break;
    case PAL_TYPE_RGBA32:
    case PAL_TYPE_BGRA32: ret.texImg.bpp = 32; break;
    case PAL_TYPE_RGB565:
    case PAL_TYPE_BGR565:
    case PAL_TYPE_RGB555:
    case PAL_TYPE_BGR555:
    case PAL_TYPE_RGB5551:
    case PAL_TYPE_BGR5551:
    case PAL_TYPE_RGBA4444:
    case PAL_TYPE_BGRA4444: //ret.bpc = 16; break;
    case PAL_TYPE_RGB444:
    case PAL_TYPE_BGR444: ret.texImg.bpp = 16; break;
    case PAL_TYPE_RGB332:
    case PAL_TYPE_BGR332: ret.texImg.bpp = 8; break;
    default: ret.errors = PAL_EINVAL; return ret;
  }*/

  /*if (NULL == data ) {
    if (size > 0 && NULL == (data = texImg_realloc(NULL,size*(ret.texImg.bpp/8)))) {
      ret.errors = PAL_ENOMEM;
      return ret;
    }
    ret.texImg.flags = TIMG_FLAG_ISALLOC;
  }*/

  ret.texImg = texImg1D_new(TIMG_TEXTURE_1D, internalFormat, size, format, type, data);
  if (texImg_isNull(&ret.texImg)) ret.src.errors = PAL_EFORM;

  /*
  ret.texImg.flags |= flags;
  ret.texImg.data = data;
  ret.texImg.xcount = size;
  printf("taille:%d bpp:%d\n", ret.texImg.xcount, ret.texImg.bpp);
*/
  return ret;
}


/******************************************************************************/

/*!
 * Create a new palette_t struct
 * @return
 */
palette_t palette_newFromFile(const char *str_file)
{
  palette_t ret;
  texImg_memset(&ret,0,sizeof(ret));
  palette_open(str_file, &ret, "r");
  return ret;
}


/******************************************************************************/

/*!
 * Create a new palette_t struct
 * @return
 */
palette_t palette_dup(palette_t *const self)
{
  palette_t ret;
  if (NULL == self) {
    texImg_memset(&ret,0,sizeof(ret));
    return ret;
  }
  ret = palette_new(self->texImg.iformat, self->texImg.xcount,
                    self->texImg.format, self->texImg.type, NULL);
  if (palette_isError(&ret)) {
    ret.src.errors |= PAL_ENOMEM;
    return ret;
  }
  if (NULL != self->texImg.data)
    texImg_memcpy(ret.texImg.data, self->texImg.data, self->texImg.size);
  return ret;
}


/******************************************************************************/

/*!
 * Export a palette structure into a file
 * @return
 */
int palette_toFile(palette_t *const self, const char *str_file)
{
  return palette_open(str_file, self, "w");
}


/******************************************************************************/

/*!
 * Create a new palette_t struct
 * @return
 */
void palette_free(palette_t *const self)
{
  if (NULL == self) return;
  texImg_delete(&self->texImg);
  /*
  if (self->texImg.flags & TIMG_FLAG_ISALLOC) {
    texImg_realloc(self->texImg.data,0);
    self->texImg.data = NULL;
    self->texImg.xcount = 0;
  }
  self->texImg.flags = 0;
  */
  self->src.errors = 0;
}


/******************************************************************************/

/*!
 * Create a new palette_t struct
 * @return
 */
void palette_delete(palette_t *const self)
{
  if (NULL == self) return;
  texImg_delete(&self->texImg);
  /*if (self->texImg.flags & TIMG_FLAG_ISALLOC) {
    texImg_realloc(self->texImg.data,0);
  }*/
  texImg_memset(self, 0, sizeof(*self));
  //self->errors = PAL_ENODATA;
}









