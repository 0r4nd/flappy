


// Standard library
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

// File
#include "wav.h"



//##################################################################################################
//## compare 2 chaines et retourne 1 si elles sont egal (memes caracteres && taille)
//##################################################################################################
static int ascii_equal(char *s0, char *s1)
{
  do {
    if (*(s0++)!=*(s1++)) return 0;
  } while (*(s0-1) != 0);

  return 1;
}


//##################################################################################################
//## lecture fichier crf.
//##################################################################################################
/*long stream_read_CHUNK_DATA(sound_t *psnd, sFILE *f){
long size,sample=0;
short *pdata; //long *pdata;

size = 65535*5; // taille des données.
pdata = psnd->data;


if (size>psnd->stream.size) size = psnd->stream.size;

//file_seek(f, psnd->stream.fbeg, SEEK_SET);
fseek(f,psnd->stream.fbeg , SEEK_SET);
//file_debug("position: %i, position ftell: %i", psnd->stream.fbeg, file_tell(f));

// mix ce sample dans le tableau destination.
while (size-->0){
//if (size%5)
psnd->stream.fpos += sizeof(short);
sample = file_getshort(f);
sample = sample*psnd->vol>>12;

*pdata = clamp16((sample + *pdata), -32768,32767); //(sample + *pdata);
pdata++;
}

}*/


//##################################################################################################
//## lecture fichier crf.
//##################################################################################################
int wav_read(sample_t *const self, wav_t *f)
{
  format_fmt_ext_t format_fmt_ext;
  uint32_t magic, chunk_size, audioFormat;
  int i,flag_fmt = 0;
  stream_t *pstream = &self->stream;
  self->stream.file = (void*)f->file;
  //stream_set_FILE(&psound->stream);

  // fichier wav correct ?.
  if (stream_getlong(pstream) != WAV_RIFF_MAGIC) {
    // printf("Unknow type file");
    return 0;
  }
  chunk_size = stream_getlong(pstream);
  if (stream_getlong(pstream) != WAV_WAVE_MAGIC) {
    // printf("Unknow type file");
    return 0;
  }



  while (!stream_eof(pstream)) { // pas top.
    magic = (uint32_t)stream_getlong(pstream);

    switch(magic) {

    // Block fmt.
    case WAVE_CHUNK_FMT:
      // Taille de ce chunk.
      chunk_size = stream_getlong(pstream);
      audioFormat = stream_getshort(pstream);  // PCM = 1 (i.e. Linear quantization)  Values other than 1 indicate some form of compression.

      if (!stream_read(&f->header, sizeof(wav_header_t), 1, pstream)) {
        // printf("Can't read FMT block");
        return 0;
      }

      // conserve quelques données.
      self->format = audioFormat;
      self->numChannels = f->header.format.numChannels; // nombre de canaux.
      self->sampleRate = f->header.format.samplesRate; // échantillons par seconde: 11025, 22050, 44100...
      self->blockAlign = f->header.format.blockAlign; // Taille d’un échantillon (canaux cumulés) en octets.(par exemple pour un son stéréo 16bits, ce champ est égal à 4).
      //printf ("blockAlign: %d\n" , self->blockAlign);

      for (i = 0; i < 2; ++i) {
        self->vol[i]  = 1.0f;
        self->dB_amplification[i] = 1.0f;
      }

      // Selon le format on va lire des données supplémentaires ou pas.
      switch (self->format) {
      case WAVE_FORMAT_PCM:
        //file_warning((void*)__FUNCTION__, "PCM");
        break;

      case WAVE_FORMAT_IMA_ADPCM:
        stream_read(&format_fmt_ext, sizeof(format_fmt_ext_t), 1, pstream);
        //file_warning((void*)__FUNCTION__, "ADPCM");
        break;

      // Beaucoup de  formats ne sont pas gérés.
      // On saute à la fin du chunk 16 est la taille du chunk de base (on quitte la fonction ici)
      default:
        stream_seek(pstream, chunk_size - 16, SEEK_CUR);
        return 0;
      }


      //file_debug("taille haudioFormateader: %i,format: %i, numChannels: %i, sampleRate: %i, blockAlign: %i",sizeof(wav_header_t), psound->format, psound->numChannels, psound->sampleRate, psound->blockAlign);
      flag_fmt++;
      break;

    // Block data. (ATTENTION IL PEUT Y AVOIR PLUSIEURS BLOCK CHUNK)
    case WAVE_CHUNK_DATA:
      // début des données à streamer dans le fichier.
      self->stream.size = stream_getlong(pstream); // taille.
      self->stream.fpos = self->stream.fbeg;
      self->stream.fbeg = stream_tell(pstream); // début des données.

      //file_debug("durée en secondes: %i, buffer fifo taille: %i", psound->stream.size/(f->header.format.avgBytesPerSec), (44100*4)/(f->header.format.avgBytesPerSec));
      //file_debug("durée en secondes: %i", f->header.format.avgBytesPerSec);
      //stream_read_CHUNK_DATA(psnd,f->file);

      // Ne sort que si le block FMT à été lu.
      //if (flag_fmt>0) return 1;//
      //break; // à modifier
      return 1;

    default:
      break;
    }

  }

  return 0;
}


//##################################################################################################
//## ouvre et charge un fichier.
//##################################################################################################
sample_t wav_open(const char *pfilename, char *pflag)
{
  wav_t wav;
  sample_t ret;

  assert(pfilename);
  assert(pflag);

  sample_memset(&ret,0,sizeof(ret));

  /* mode fichier. */
  if (pflag[1] == 'b') {
    //stream_set_FILE(&ret->stream);
    ret.stream.flags = STREAM_FLAG_FILE;

    if (!(wav.file = (void*)file_open(pfilename, pflag))) {
      /* printf("N'a pas pu ouvrir le fichier"); */
      return ret;
    }

  /* mode buffer mémoire. */
  } else if (pflag[1] == 'm') {
    wav.file = (void*)pfilename;
    //stream_set_BUFF(&ret.stream);
    ret.stream.flags = STREAM_FLAG_BUFFER;
  }

  /* accès lecture ou écriture. */
  wav.name = pfilename;
  switch (*(unsigned short*)pflag) {
  case WAV_MAKETWOCC('r','\n'):
  case WAV_MAKETWOCC('r', 'm'): /** @TODO: memory mode */
  case WAV_MAKETWOCC('r', 'b'):
    wav_read(&ret, &wav);
    break;
  case WAV_MAKETWOCC('w', 'b'): /* wav_write(&ret, &wav); */
    break;
  default:
    /* printf("flag inconnu"); */
    if (pflag[1] == 'b') file_close(&wav.file);
    return ret;
  }

  /* ferme le fichier. */
  if (wav.file) file_close(&wav.file);
  return ret;
}


//##################################################################################################
//## Ouvre tous les fichiers wav d'un repertoire.
//##################################################################################################
long wav_close(wav_t *const self)
{
  assert(self);
  file_close(self->file);
}

//##################################################################################################
//## Ouvre tous les fichiers wav d'un repertoire.
//##################################################################################################
void wav_open_dir(char *pfilepath, sample_t *psample, int count_max)
{
  char path_name[512];
  struct dirent *pentry;
  struct stat st;
  DIR *prep;

  // Chemin dossier.
  strcpy(path_name, pfilepath);

  // Obligatoire.
  strcat(path_name, "/.");
  prep = opendir(path_name);

  if (NULL == prep) return;

  while ((pentry = readdir(prep)) != NULL) {
    // Saute si on trouve un ou deux points seuls.
    if (ascii_equal(".", pentry->d_name) || ascii_equal("..", pentry->d_name)) continue;

    strcpy(path_name, pfilepath);
    strcat(path_name, pentry->d_name);

    stat(path_name, &st);
    if (S_ISDIR(st.st_mode)) {
      // printf("%s (dossier)", pentry->d_name);
    } else {
      // printf("%s (fichier)", pentry->d_name);
      //wav_open(pfilepath, pentry->d_name, "rb");
      if (count_max == 0) break;
      count_max--;
      *psample++ = wav_open(path_name, "rb");
    }

  }

  closedir(prep);
}







