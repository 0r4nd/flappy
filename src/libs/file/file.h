
#ifndef FILE_H_
#define FILE_H_

// File
#include "file_type.h"


#ifdef __cplusplus
extern "C" {
#endif


/* file/buffer type */
#define STREAM_FLAG_FILE        (1<<0)  // depuis un fichier.
#define STREAM_FLAG_BUFFER      (1<<1)  // depuis un buffer.
#define STREAM_FLAG_TEXT        (1<<2)  // text format
#define STREAM_FLAG_DIRECTCOPY  (1<<3)

#define stream_isFile(s) ((s)->flags & STREAM_FLAG_FILE)
#define stream_isBuffer(s) ((s)->flags & STREAM_FLAG_BUFFER)



/******************************************************************************/

/**
 * some callbacks for systems
 */
char *fname_extn(const char *fname, int n);

/**
 *
 */
int file_getc(FILE *const self);
int file_putc(int c, FILE *f);
long file_tell(FILE *const self);
int file_seek(FILE *const self, long offset, int whence);
int file_eof(FILE *const self);
char *file_gets(char *s, int num, FILE *f);
int file_puts(const char *s, FILE *f);
size_t file_read(void *p, size_t sz, size_t cnt, FILE *f);
size_t file_write(void *p, size_t sz, size_t cnt, FILE *f);
FILE *file_open(const char *pfilename, const char *pflag);
int file_close(FILE *const self);

long dir_exist(char *s);
int dir_remove(char *name);
int dir_create(char *name);

short file_getshort(FILE *const self);
int file_getlong(FILE *const self);
void file_putshort(FILE *const self, unsigned long twocc);
void file_putlong(FILE *const self, unsigned long fourcc);
int file_copy(char *in, char *out, char *pflags);
long file_exist(char *s);
long file_size(char *s);
void file_hide(char *name);
void file_unhide(char *name);
int file_string_fromFile(char *buf, FILE *file, int buf_size_max);
int file_fromString(FILE *const self, char *buf);
int file_write_string(char *pname, char *buf, char *flags);
int file_read_string(char *pname, char *buf, int buf_size_max);


// file_debug.c
void file_debug(char *form, ...);
void file_warning(char *function, char *form, ...);
void file_exception(char *function, char *form, ...);

// file_stream.c
int stream_getc(stream_t *const self);
int stream_getshort(stream_t *const self);
long stream_getlong(stream_t *const self);

char *stream_gets(char *str, int size, stream_t *const self);
int stream_puts(const char *str, stream_t *const self);

size_t stream_read(void *p, long size, long count, stream_t *const self);
size_t stream_write(void *p, long size, long count, stream_t *const self);

int stream_seek(stream_t *const self, long offset, int whence);
long stream_tell(stream_t *const self);

int stream_eof(stream_t *const self);
void stream_close(stream_t *const self);



#ifdef __cplusplus
}
#endif


#endif

