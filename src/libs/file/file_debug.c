
// File
#include "file.h"



//##################################################################################################
//## messages de debug
//##################################################################################################
void file_debug(char *form, ...)
{
#define STR(s)  #s
#define XSTR(s) STR(s)

#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  static char init = 1;
  FILE *f;
  va_list arg;
  va_start(arg, form);

  if (!dir_exist("debug")) dir_create("debug");

  // initialisation lors du premier appel
  if (init) {
    init = 0;

    f = fopen("debug/debug.txt", "w+");
    if (NULL == f) {
      va_end(arg);
      return;
    }

    fprintf(f, "%s", BOM_UTF8);
    fprintf(f, "/******************************************************************************/n");
    fprintf(f, STR(## DEBUG FILE)STR([)XSTR(__DATE__)STR(])STR([)XSTR(__TIME__)STR(]\n));
    fprintf(f, "/******************************************************************************/\n");
    fprintf(f, "\n");
  } else {
    f = fopen("debug/debug.txt", "a+");
    if (NULL == f) {
      va_end(arg);
      return;
    }
  }

  _vsnprintf(aBUFF_TMP, sizeof(aBUFF_TMP), form, arg);
  fprintf(f, "%s", aBUFF_TMP);
  fclose(f);
  va_end(arg);
#endif


#undef STR
#undef XSTR
}


//##################################################################################################
//## messages de debug    ex: file_warning((void*)__FUNCTION__, "");
//##################################################################################################
void file_warning(char *function, char *form, ...)
{
  // ex: file_warning((void*)__FUNCTION__, "");
#define STR(s)  #s
#define XSTR(s) STR(s)

#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  char sfunction[512];
  FILE *f;
  va_list arg;
  va_start(arg, form);

  strcpy(sfunction, "debug/warning/");
  strcat(sfunction, function);
  strcat(sfunction, ".txt");

  if (!dir_exist("debug")) dir_create("debug");
  if (!dir_exist("debug/warning")) dir_create("debug/warning");

  // initialisation lors du premier appel
  if (!file_exist(sfunction)) {
    f = file_open(sfunction, "w+");
    if (NULL == f) {
      file_debug("n'a pas pu créer");
      va_end(arg);
      return;
    }

    fprintf(f, "%s", BOM_UTF8);
    fprintf(f, "/******************************************************************************/\n");
    fprintf(f, STR(## DEBUG FILE)STR([)XSTR(__DATE__)STR(])STR([)XSTR(__TIME__)STR(]\n));
    fprintf(f, "/******************************************************************************/\n");
    fprintf(f, "\n");
  } else {
    f = fopen(sfunction, "a+");
    if (NULL == f) {
      file_debug("n'a pas pu créer...");
      va_end(arg);
      return;
    }
  }

  _vsnprintf(aBUFF_TMP, sizeof(aBUFF_TMP), form, arg);
  fprintf(f, "%s\n", aBUFF_TMP);
  fclose(f);
  va_end(arg);
#endif

//file_debug("FIN");


#undef STR
#undef XSTR
}

//##################################################################################################
//## messages de debug    ex: file_exception((void*)__FUNCTION__, "");
//##################################################################################################
void file_exception(char *function, char *form, ...)
{
#define STR(s)  #s
#define XSTR(s) STR(s)

#if defined(WIN32) || defined(_WIN32) || defined(Linux)
  char sfunction[512];
  FILE *f;
  va_list arg;
  va_start(arg, form);

  strcpy(sfunction, "debug/exception/");
  strcat(sfunction, function);
  strcat(sfunction, ".txt");

  if (!dir_exist("debug")) dir_create("debug");
  if (!dir_exist("debug/exception")) dir_create("debug/exception");

// initialisation lors du premier appel
  if (!file_exist(sfunction)) {
    f = fopen(sfunction, "w+");
    if (NULL == f) {
      va_end(arg);
      return;
    }
    fprintf(f, "%s", BOM_UTF8);
    fprintf(f, "/******************************************************************************/\n");
    fprintf(f, STR(## DEBUG FILE)STR([)XSTR(__DATE__)STR(])STR([)XSTR(__TIME__)STR(]\n));
    fprintf(f, "/******************************************************************************/\n");
    fprintf(f, "\n");
  } else {
    f = fopen(sfunction, "a+");
    if (NULL == f) {
      va_end(arg);
      return;
    }
  }

  _vsnprintf(aBUFF_TMP, sizeof(aBUFF_TMP), form, arg);
  fprintf(f, "%s\n", aBUFF_TMP);
  fclose(f);
  va_end(arg);
#endif


#undef STR
#undef XSTR
}


/* dump uint32_t as hex digits */
void file_uint32_array_to_c_hex(const char *name, const uint32_t *a, size_t sz)
{
  size_t i;

  file_debug("// extern const uint32_t %s[%d];\n", name, (int)sz);
  file_debug("const uint32_t %s[%d] = {\n  ", name, (int)sz);
  for (i=0; i<sz; ++i) {
    file_debug("0x%08x%s", a[i], Inter(i,10,sz));
  }
  file_debug("};\n");
}


/**
 * prints char array as a c program snippet
 */
void file_char_array_to_c(const char *name, const char *a, size_t sz)
{
  uint8_t tmp;
  size_t i;

  file_debug("// extern const uint8_t %s[%d];\n", name, (int)sz);
  file_debug("const uint8_t %s[%d] = {\n  ", name, (int)sz);

  for (i=0; i<sz; ++i) {
    if (a[i] == 0) {
      file_debug("'\\0'%s", Inter(i,10,sz));
    } else if (a[i] == '\n') {
      file_debug("'\\n'%s", Inter(i,10,sz));
    } else if (a[i] == '\t') {
      file_debug("'\\t'%s", Inter(i,10,sz));
    } else if (a[i] == '\r') {
      file_debug("'\\r'%s", Inter(i,10,sz));
    } else if (a[i] == '\'') {
      file_debug("'\\''%s", Inter(i,10,sz));
    } else if (a[i] == '\\') {
      file_debug("'\\\\'%s", Inter(i,10,sz));
    } else if (a[i] < 32 || a[i] > 126) {
      tmp = (uint8_t)a[i];
      file_debug("0x%02x%s", tmp, Inter(i,10,sz));
    } else {
      file_debug(" '%c'%s", (char)a[i], Inter(i,10,sz));
    }

  }
  file_debug("\n};\n\n");
}


/**
 * prints an uint array as a c program snippet
 */
void file_uint32_array_to_c(const char* name, const uint32_t *a, size_t sz)
{
  size_t i;
  file_debug("// extern const uint32_t %s[%d];\n", name, (int)sz);
  file_debug("const uint32_t %s[%d] = {\n  ", name, (int)sz);

  for (i=0; i<sz; ++i) {
    file_debug("%3u%s", a[i],Inter(i,12,sz));
  }
  file_debug("};\n\n");
}

