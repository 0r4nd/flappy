
// data
#include "file.h"



/******************************************************************************/

/**
 *
 */
void stream_zero(stream_t *const self)
{
  memset(self, 0, sizeof(*self));
}


/******************************************************************************/

/**
 * STREAM_TYPE_FILE
 */
static int stream_file_getc(stream_t *const self)
{
  self->fpos += sizeof(char);
  return file_getc((FILE*)self->file);
}

static int stream_file_getshort(stream_t *const self)
{
  self->fpos += sizeof(short);
  return file_getshort((FILE*)self->file);
}

static long stream_file_getlong(stream_t *const self)
{
  self->fpos += sizeof(long);
  return file_getlong((FILE*)self->file);
}

static char *stream_file_gets(char *str, int size, stream_t *const self)
{
  int c;
  char *pstmp = str;
  while (--size > 0 && (c = stream_file_getc(self)) != EOF) {
    if ((*pstmp++ = c) == '\n') break;
  }
  *pstmp = '\0';
  return (c == EOF && pstmp == str) ? NULL : str;
}

static int stream_file_puts(const char *str, stream_t *const self)
{
  if (NULL == str) return EOF;
  self->fpos += strlen(str);
  if (self->fpos > self->size) self->size = self->fpos;
  return file_puts(str, (FILE*)self->file);
}

static size_t stream_file_read(void *p, long size, long count, stream_t *const self)
{
  self->fpos += count * size;
  return file_read(p, size, count, (FILE*)self->file);
}

static size_t stream_file_write(void *p, long size, long count, stream_t *const self)
{
  size_t ret = file_write(p, size, count, (FILE*)self->file);
  self->fpos += ret;
  return ret;
}

static int stream_file_seek(stream_t *const self, long offset, int whence)
{
/*  if (whence == SEEK_SET)      self->fpos = self->fbeg + offset;
  else if (whence == SEEK_CUR) self->fpos += offset;
  else if (whence == SEEK_END) self->fpos = self->fbeg + self->size + offset;

  if (self->fpos < self->fbeg) {
    self->fpos = self->fbeg;
    return -1;
  }
  if (self->fpos > self->fbeg + self->size) {
    self->fpos = self->fbeg + self->size;
    return -1;
  }*/
  return file_seek((FILE*)self->file, offset, whence);
}

static long stream_file_tell(stream_t *const self)
{
  return file_tell((FILE*)self->file);
}

static int stream_file_eof(stream_t *const self)
{
  //return pstream->fpos >= (self->fbeg+self->size);
  return file_eof((FILE*)self->file);
}

static void stream_file_close(stream_t *const self)
{
  file_close((FILE*)self->file);
  stream_zero(self);
}


/******************************************************************************/

/**
 * STREAM_TYPE_BUFFER
 */
static int stream_buffer_getc(stream_t *const self)
{
  int data = *(char*)(self->file + self->fpos);
  self->fpos += sizeof(char);
  return (self->fpos <= self->size)? data : EOF;
}

static int stream_buffer_getshort(stream_t *const self)
{
  int data = *(short*)(self->file + self->fpos);
  self->fpos += sizeof(short);
  return (self->fpos <= self->size)? data : EOF;
}

static long stream_buffer_getlong(stream_t *const self)
{
  long data = *(long*)(self->file + self->fpos);
  self->fpos += sizeof(long);
  return (self->fpos <= self->size)? data : EOF;
}

static char *stream_buffer_gets(char *str, int size, stream_t *const self)
{
  int c;
  char *pstmp = str;
  while (--size > 0 && (c = stream_buffer_getc(self)) != EOF) {
    if ((*pstmp++ = c) == '\n') break;
  }
  *pstmp = '\0';
  return (c == EOF && pstmp == str) ? NULL : str;
}

static int stream_buffer_puts(const char *str, stream_t *const self)
{
  if (NULL == str) return EOF;
  while (*str) {
    self->file[self->fpos++] = *str++;
  }
  /* appends a newline character ('\n') */
  self->file[self->fpos++] = '\n';

  if (self->fpos > self->size) self->size = self->fpos;
  return 0;
}


static size_t stream_buffer_read(void *p, long size, long count, stream_t *const self)
{
  char *pdst, *psrc;
  pdst = p;
  psrc = self->file + self->fpos;
  size *= count;
  self->fpos += size;
  while (size-- > 0) *(pdst++) = *(psrc++);
  return 1;
}

static size_t stream_buffer_write(void *p, long size, long count, stream_t *const self)
{
  size = size * count;
  memcpy(self->file, p, size);
  self->fpos += size;
  if (self->fpos > self->size) self->size = self->fpos;
  return size;
}

static int stream_buffer_seek(stream_t *const self, long offset, int whence)
{
  if (whence == SEEK_SET) {
    self->fpos = self->fbeg + offset;
  } else if (whence == SEEK_CUR) {
    self->fpos = self->fpos + offset;
  } else if (whence == SEEK_END) {
    self->fpos = self->fbeg + self->size;
  }
  if (self->fpos < self->fbeg) {
    self->fpos = self->fbeg;
    return -1;
  }
  if (self->fpos > self->fbeg + self->size) {
    self->fpos = self->fbeg + self->size;
    return -1;
  }

  return 0;
}

static long stream_buffer_tell(stream_t *const self)
{
  return self->fpos;
}

static int stream_buffer_eof(stream_t *const self)
{
  return self->fpos >= (self->fbeg+self->size);
  //char *pbuff = (char*)self->file ;
  //return ( pbuff[self->fpos] == -1 );
}

static void stream_buffer_close(stream_t *const self)
{
  stream_zero(self);
}


/******************************************************************************/

/**
 *
 */
int stream_getc(stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_getc(self);
  else if (stream_isBuffer(self)) return stream_buffer_getc(self);
  return 1;
}

int stream_getshort(stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_getshort(self);
  else if (stream_isBuffer(self)) return stream_buffer_getshort(self);
  return 1;
}

long stream_getlong(stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_getlong(self);
  else if (stream_isBuffer(self)) return stream_buffer_getlong(self);
  return 1;
}

char *stream_gets(char *str, int size, stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_gets(str, size, self);
  else if (stream_isBuffer(self)) return stream_buffer_gets(str, size, self);
  return NULL;
}
int stream_puts(const char *str, stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_puts(str, self);
  else if (stream_isBuffer(self)) return stream_buffer_puts(str, self);
  return 1;
}

size_t stream_read(void *p, long size, long count, stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_read(p, size, count, self);
  else if (stream_isBuffer(self)) return stream_buffer_read(p, size, count, self);
  return 1;
}

size_t stream_write(void *p, long size, long count, stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_write(p, size, count, self);
  else if (stream_isBuffer(self)) return stream_buffer_write(p, size, count, self);
  return 0;
}

int stream_seek(stream_t *const self, long offset, int whence)
{
  if (stream_isFile(self)) return stream_file_seek(self, offset, whence);
  else if (stream_isBuffer(self)) return stream_buffer_seek(self, offset, whence);
  return 0;
}

long stream_tell(stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_tell(self);
  else if (stream_isBuffer(self)) return stream_buffer_tell(self);
  return 0;
}

int stream_eof(stream_t *const self)
{
  if (stream_isFile(self)) return stream_file_eof(self);
  else if (stream_isBuffer(self)) return stream_buffer_eof(self);
  return 1;
}

void stream_close(stream_t *const self)
{
  if (stream_isFile(self)) stream_file_close(self);
  else if (stream_isBuffer(self)) stream_buffer_close(self);
}
























//##################################################################################################
//## modifie le type de stream.
//##################################################################################################
/*
// stream à partir d'un ficher.
void stream_set_FILE(stream_t *pstream)
{
  pstream->flags = STREAM_FLAG_FILE;
  pstream->fbeg = 0;
  pstream->fpos = 0;
  pstream->size = 0;
  pstream->fn_close    = (void*)stream_close_file;
  pstream->fn_getc     = (void*)stream_getc_file;
  pstream->fn_getshort = (void*)stream_getshort_file;
  pstream->fn_getlong  = (void*)stream_getlong_file;
  pstream->fn_gets     = (void*)stream_gets_file;
  pstream->fn_read     = (void*)stream_read_file;
  pstream->fn_seek     = (void*)stream_seek_file;
  pstream->fn_tell     = (void*)stream_tell_file;
  pstream->fn_eof      = (void*)stream_eof_file;
}

// stream à partir d'un buffer en mémoire.
void stream_set_BUFF(stream_t *pstream)
{
  pstream->flags = STREAM_FLAG_BUFFER;
  pstream->fbeg = 0;
  pstream->fpos = 0;
  pstream->size = 0;
  pstream->fn_close    = (void*)stream_close_buff;
  pstream->fn_getc     = (void*)stream_getc_buff;
  pstream->fn_getshort = (void*)stream_getshort_buff;
  pstream->fn_getlong  = (void*)stream_getlong_buff;
  pstream->fn_gets     = (void*)stream_gets_buff;
  pstream->fn_read     = (void*)stream_read_buff;
  pstream->fn_seek     = (void*)stream_seek_buff;
  pstream->fn_tell     = (void*)stream_tell_buff;
  pstream->fn_eof      = (void*)stream_eof_buff;
}
*/








