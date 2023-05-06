
// File
#include "file.h"

// byte order mark for text files
const char BOM_UTF8[4] = {0xEF,0xBB,0xBF,0};
const char BOM_UTF16BE[3] = {0xFE,0xFF,0}; // Big Endian
const char BOM_UTF16LE[3] = {0xFF,0xFE,0}; // Little Endian
const char BOM_UTF32BE[5] = {0x00,0x00,0xFE,0xFF,0}; // Big Endian
const char BOM_UTF32LE[5] = {0xFF,0xFE,0x00,0x00,0}; // Little Endian
const char BOM_UTF7[4][5] = {
  {0x2B,0x2F,0x76,0x38,0}, // 0x2D
  {0x2B,0x2F,0x76,0x39,0},
  {0x2B,0x2F,0x76,0x2B,0},
  {0x2B,0x2F,0x76,0x2F,0}
};
const char BOM_UTF1[4] = {0xF7,0x64,0x4C,0};
const char BOM_UTFEBCDIC[5] = {0xDD,0x73,0x66,0x73,0};
const char BOM_SCSU[4] = {0x0E,0xFE,0xFF,0};
const char BOM_BOCU1[4] = {0xFB,0xEE,0x28,0};
const char BOM_GB18030[5] = {0x84,0x31,0x95,0x33,0};

// copy buffer
char aBUFF_TMP[2048];

// tableau des fichiers utilisés au cours du jeu
// ce tableau est accessible par une liste chainée.
//sFILE *tFILES[FILE_SIZE_MAX];


/******************************************************************************/

/**
 * wrapper function for fgetc()
 */
int file_getc(FILE *const self)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  return fgetc(self);
#endif
}


/**
 * wrapper function for fputc()
 */
int file_putc(int c, FILE *const self)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  return fputc(c, self);
#endif
}


/**
 * wrapper function for ftell()
 */
long file_tell(FILE *const self)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  return ftell(self);
#endif
}


/**
 * wrapper function for fseek()
 */
// whence: SEEK_SET, SEEK_CUR, SEEK_END.
int file_seek(FILE *const self, long offset, int whence)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  return fseek(self, offset, whence);
#endif
}


/**
 * wrapper function for feof()
 */
int file_eof(FILE *const self)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  return feof(self);
#endif
}


/**
 * wrapper function for fgets()
 */
char *file_gets(char *s, int num, FILE *f)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  return fgets(s, num, f);
#endif
}


/**
 * wrapper function for fputs()
 */
int file_puts(const char *s, FILE *f)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  return fputs(s, f);
#endif
}


/**
 * wrapper function for fread()
 */
 size_t file_read(void *p, size_t sz, size_t cnt, FILE *f)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  return fread(p, sz, cnt, f);
#endif
}


/**
 * wrapper function for fwrite()
 */
size_t file_write(void *p, size_t sz, size_t cnt, FILE *f)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux) // PC
  return fwrite(p, sz, cnt, f);
#endif
}


/**
 * wrapper function for fopen()
 */
 FILE *file_open(const char *pfilename, const char *pflag)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
//file_warning((void*)__FUNCTION__, (void*)0);
  return fopen(pfilename, pflag);
#endif
}


/**
 * wrapper function for fclose()
 */
int file_close(FILE *const self)
{
#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  //file_warning((void*)__FUNCTION__, (void*)0);
  return fclose(self);
#endif
}


/******************************************************************************/

/**
 *
 */
long dir_exist(char *s)
{
  DIR *prep;
  if (NULL != (prep = opendir(s))) {
    closedir(prep);
    return 1;
  }
  return 0;
}


/******************************************************************************/

/**
 * remove a directory (and all subdirs and files)
 */
int dir_remove(char *name)
{
  DIR *directory;
  struct dirent *entry;
  struct stat file_stat;
  char buffer[1024];

  // On ouvre le dossier.
  directory = opendir(name);
  if (directory == NULL) return 0;

  // On boucle sur les entrées du dossier.
  while (NULL != (entry = readdir(directory))) {
    // On "saute" les répertoires "." et "..".
    if (strcmp(entry->d_name, ".")>0 || strcmp(entry->d_name, "..")>0)  continue;
    snprintf(buffer, 1024, "%s/%s", name, entry->d_name);
    //ascii_cpy(buffer,name);
    //ascii_cat(buffer,"/");
    //ascii_cat(buffer,entry->d_name);
    // On récupère des infos sur le fichier.
    stat(buffer, &file_stat);
    if (S_ISREG(file_stat.st_mode)) unlink(buffer);
    else if (S_ISDIR(file_stat.st_mode)) dir_remove(buffer);

  }

    // On ferme le dossier puis l'efface.
  closedir(directory);
  rmdir(name);

  return 1;
}


/******************************************************************************/

/**
 * create a directory
 */
int dir_create(char *name)
{
  int i = 0, j = 0;
  int res = 0;
  //char buffer[512];
  char *buffer = aBUFF_TMP;

  if (NULL == name) return res;
  if ('\0' == *name) return res;

  // Crée tous les dossiers jusqu'a celui voulu.
  for (i = 0; name[i]; ++i, ++j) {
    buffer[j] = name[i];

    if (name[i] == '/' || name[i] == '\\' || name[i + 1] == '\0') {
      buffer[j + 1] = '\0';
      if (!dir_exist(buffer)) res = !mkdir(buffer);
      else res = 1;
    }
  }

  return res;
}


/******************************************************************************/

/**
 * types
 */
short file_getshort(FILE *const self)
{
  int c = file_getc(self);
  c |= file_getc(self) << 8;
  return c;
}
int file_getlong(FILE *const self)
{
  int c = file_getc(self);
  c |= file_getc(self) << 8;
  c |= file_getc(self) << 16;
  c |= file_getc(self) << 24;
  return c;
}

#if 0
unsigned long long file_getlonglong(FILE *const self)
{
  unsigned long long c;
//file_read(&c, sizeof(unsigned long long), 1, self); // faster ?
  c  = file_getc(self);
  c |= file_getc(self) << 8;
  c |= file_getc(self) << 16;
  c |= file_getc(self) << 24;
  c |= file_getc(self) << 32;
  c |= file_getc(self) << 40;
  c |= file_getc(self) << 48;
  c |= file_getc(self) << 56;
  return c;
}
#endif


/******************************************************************************/

/**
 *
 */
void file_putshort(FILE *const self, unsigned long twocc)
{
  char *p = (char*)&twocc;
  file_putc(p[0],self);
  file_putc(p[1],self);
}
void file_putlong(FILE *const self, unsigned long fourcc)
{
  char *p = (char*)&fourcc;
  file_putc(p[0],self);
  file_putc(p[1],self);
  file_putc(p[2],self);
  file_putc(p[3],self);
}
#if 0
void file_putlonglong(FILE *const self, unsigned long long heightcc)
{
  char *p = (char*)&heightcc;
//file_write(&heightcc, sizeof(unsigned long long), 1, self); // faster ?
  file_putc(*(p++),self);
  file_putc(*(p++),self);
  file_putc(*(p++),self);
  file_putc(*(p++),self);
  file_putc(*(p++),self);
  file_putc(*(p++),self);
  file_putc(*(p++),self);
  file_putc(*(p  ),self);
}
#endif


/******************************************************************************/

/**
 *
 */
int file_copy(char *in, char *out, char *pflags)
{
  FILE *src, *dst;
  //static char buffer[512];
  char *buffer = aBUFF_TMP;
  int num;

  if (NULL == (src = file_open(in, "rb"))) return 1;
  if (NULL == (dst = file_open(out, pflags))) {
    fclose(src);
    return -1;
  }
  // Par blocs de 512.
  while (0 != (num = file_read(buffer, 1, sizeof(aBUFF_TMP), src)))
    file_write(buffer, 1, num, dst);

  file_close(dst);
  file_close(src);
  return 0;
}


/******************************************************************************/

/**
 *
 */
long file_exist(char *s)
{
  FILE *f;
  if (NULL == s || '\0' == *s) return 0;
  if (NULL != (f = file_open(s, "r"))) {
    file_close(f);
    return 1;
  }
  return 0;
}


/******************************************************************************/

/**
 * size of a file
 */
long file_size(char *s)
{
  FILE *f;
  long size = 0;
  if (NULL != (f = file_open(s, "r"))) {
    file_seek(f, 0, SEEK_END);
    size = file_tell(f);
    file_close(f);
  }
  return size;
}


/******************************************************************************/

/**
 * hide a file
 */
void file_hide(char *name)
{
  int attr;
  attr = GetFileAttributes(name);
  if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0)
    SetFileAttributes(name, attr | FILE_ATTRIBUTE_HIDDEN);
}


/******************************************************************************/

/**
 * unhide a file
 */
void file_unhide(char *name)
{
  int attr;
  //char buffer[1024];
  if (!file_exist(name) && !dir_exist(name)) return;
  attr = GetFileAttributes(name);
  if ((attr & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN) SetFileAttributes(name, attr & ~FILE_ATTRIBUTE_HIDDEN);
}


/******************************************************************************/

/**
 *
 */
int file_string_fromFile(char *buf, FILE *file, int buf_size_max)
{
  long fpos, fsize;
  if (NULL == buf || NULL == file) {
    //printf("nexiste pas\n");
    return -1;
  }
  // Récupératon taille.
  fpos = file_tell(file);
  file_seek(file, 0, SEEK_END);
  fsize = file_tell(file);
  // Remet à la position d'origine
  file_seek(file, fpos, SEEK_SET);

  if (fsize + 1 >= buf_size_max) {
    //printf("taille:%d taille max:%d\n");
    return -1;
  }

  // copie puis remet à la position d'origine
  //ret = file_read(buf, 1, fsize, file);
  file_seek(file, fpos, SEEK_SET);

  //if (ret != fsize) {
  //printf("taille mauvaise: %d %d\n", ret, fsize);
  //    return 0;
  //}

  buf[fsize] = '\0';
  return 1;
}


/******************************************************************************/

/**
 *
 */
int file_fromString(FILE *const self, char *buf)
{
  long fsize;
  if (NULL == buf || NULL == self) return 0;
  fsize = strlen(buf);
  if (fsize <= 0) return 0;
  file_write(buf, 1, fsize, self);
  return 1;
}

/******************************************************************************/

/**
 * flags: "w+", "a+"...
 */
int file_write_string(char *pname, char *buf, char *flags)
{
  FILE *f;
  long ret;

  if (NULL != (f = file_open(pname, flags))) {
    ret = file_fromString(f, buf);
    if (ret) file_putc('\n', f);
    file_close(f);
    return ret;
  }
  return 0;
}


/******************************************************************************/

/**
 *
 */
int file_read_string(char *pname, char *buf, int buf_size_max)
{
  FILE *f;
  if (NULL == (f = file_open(pname, "r"))) return 0;

  //ret = file_string_fromFile(buf, f, buf_size_max);
  while (NULL != file_gets(buf, buf_size_max, f)) {

  }
  file_close(f);
  return 1;
}


