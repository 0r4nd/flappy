
#ifndef FILE_TYPE_H_
#define FILE_TYPE_H_


/* Standard library */
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <dirent.h>   /* POSIX (pour lister les fichiers d'un repertoire) */
#include <string.h>

/*#ifndef WIN32*/
#include <sys/types.h> /* pour POSIX sous windows (pour certain types) */
#include <sys/stat.h>
/*#endif*/

/* La gestion du fichier (stream ou simplement lecture en une fois) */
#define FILE_DATA_NORMAL  (1<<0)
#define FILE_DATA_STREAM  (1<<1)

/* for (i=0; i<size; ++i)  printf("%d%s", i*10, Inter(i,12,size)); */
#define Inter(i,nb,max) ( (!(((i)+1)%nb))? (((i)+1)==max)? "\n": ",\n  " : (((i)+1)==max)? "": "," )

#define FILE_SIZE_MAX  1024

/* byte order mark for text files */
extern const char BOM_UTF8[4];
extern const char BOM_UTF16BE[3]; /* Big Endian */
extern const char BOM_UTF16LE[3]; /* Little Endian */
extern const char BOM_UTF32BE[5]; /* Big Endian */
extern const char BOM_UTF32LE[5]; /* Little Endian */
extern const char BOM_UTF7[4][5];
extern const char BOM_UTF1[4];
extern const char BOM_UTFEBCDIC[5];
extern const char BOM_SCSU[4];
extern const char BOM_BOCU1[4];
extern const char BOM_GB18030[5];


extern char aBUFF_TMP[2048];


/******************************************************************************/

/**
 * cc flags
 */
#define FILE_MAKEFOURCC(b0,b1,b2,b3) ((b0)|((b1)<<8)|((b2)<<16)|((b3)<<24))
#define FILE_MAKETWOCC(b0,b1) ((b0)|((b1)<<8))



typedef struct stream_s stream_t;
struct stream_s {
  uint16_t flags;  /* type de stream (FILE ou BUFFER) */
  uint16_t errors; /* errors i/o */
  long fbeg; /* dans le fichier, index du début des données */
  long fpos; /* position actuelle depuis le début de l'echantillon */
  long size; /* taille de l'echantillon entier */
  char *file; /* fichier ou buffer concerné */
};












#endif

