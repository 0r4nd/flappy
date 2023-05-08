#ifndef SAMPLE_H_
#define SAMPLE_H_


#include <stdio.h>   /* printf */
#include <stdlib.h>  /* realloc */
#include <string.h>  /* memcpy, memset */
#include <stdint.h>  /* int32_t, int64_t */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */
#include <windows.h> /* console functions */
#include "../file.h"


#ifdef __cplusplus
extern "C" {
#endif


/* uncomment for debug */
//#define SPRITECONSOLE_DEBUG


/******************************************************************************/

/**
 * User wrapped functions and options.
 */
#define sample_memset(out,val,sz) memset(out,val,sz)
#define sample_realloc(base,size) realloc(base, size)

/******************************************************************************/

/**
 * structs
 */
/*typedef struct sample_s {
  char *data;
  HINSTANCE HInstance;
} sample_t;
*/

typedef struct sample_s {
  float vol[2];     // Volume de tous les canaux.
  float dB_amplification[2]; // amplification à appliquer sur la source pour normaliser le signal.
  long type;
  long group;       // Le son appartient à quel groupe.
  long channelMix;  // Sur quel canal(interne) est joué le son.

  /* interne, ne pas toucher. */
  long format;      /* pcm,adpcm... */
  long numChannels; /* Mono=1, Stereo=2, etc. */
  long sampleRate;  /* 8000, 44100, etc. */
  long blockAlign;  /* Taille d’un échantillon (canaux cumulés) en octets.(par exemple pour un son stéréo 16bits, ce champ est égal à 4). */
  stream_t stream;
  short *data[4];   /* normal - lowPass - midPass - higPass */
} sample_t;


/******************************************************************************/

/**
 * prototypes
 */
sample_t sample_new(const char *filename);
void sample_delete(sample_t *const self);
void sample_play(sample_t *const self, int async);


/*
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <conio.h>


class Wave {

public:
    Wave(char * filename);
    ~Wave();
    void play(bool async=true);
    bool isok();

private:
    char * buffer;
    bool ok;
    HINSTANCE HInstance;
};

Wave::Wave(char * filename)
{
    ok = false;
    buffer = 0;
    HInstance = GetModuleHandle(0);

    ifstream infile(filename, ios::binary);

    if (!infile)
    {
         std::cout << "Wave::file error: "<< filename << std::endl;
        return;
    }

    infile.seekg (0, ios::end);   // get length of file
    int length = infile.tellg();
    buffer = new char[length];    // allocate memory
    infile.seekg (0, ios::beg);   // position to start of file
    infile.read (buffer,length);  // read entire file

    infile.close();
    ok = true;
}

Wave::~Wave()
{
    PlaySound(NULL, 0, 0); // STOP ANY PLAYING SOUND
    delete [] buffer;      // before deleting buffer.
}
void Wave::play(bool async)
{
    if (!ok)
        return;

    if (async)
        PlaySound(buffer, HInstance, SND_MEMORY | SND_ASYNC);
    else
        PlaySound(buffer, HInstance, SND_MEMORY);
}

bool Wave::isok()
{
    return ok;
}

int main(int argc, char *argv[]) {

    std::cout << "Trying to play sound ...\n";

    Wave one("sound1.WAV");
    Wave two("sound2.WAV");
    Wave three("sound3.WAV");

    one.play(0);
    two.play(0);
    three.play(0);

    std::cout << "press key to exit";
    getch();

    return 0;
}

https://www.youtube.com/watch?v=jUL-0IZqcYQ
*/



#ifdef __cplusplus
}
#endif


#endif /* SPRITECONSOLE_H_ */


