
#ifndef WAV_H_
#define WAV_H_


// Standard library.
#include <stdio.h>
#include <stdint.h>

// sample
#include "sample.h"


//##################################################################################################
//## Structure son.
//##################################################################################################

#pragma pack(push,1)
typedef struct wav_format_fmt_s {
  int16_t numChannels;  // nombre de canaux : 1=mono, 2=stéréo.
  int32_t samplesRate;  // Nombre d’échantillons par seconde: 11025, 22050, 44100, éventuellement 48000 et 96000.
  int32_t byteRate;     // SampleRate * NumChannels * BitsPerSample/8
  int16_t blockAlign;   // Taille d’un échantillon (canaux cumulés) en octets.(par exemple pour un son stéréo 16bits, ce champ est égal à 4).
  int16_t bitPerSample; // NumChannels * BitsPerSample/8
} wav_format_fmt_t;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct wav_header_s {
  wav_format_fmt_t format;
//int16_t bitsPerSample; // Taille en bits d’un échantillon sur un seul canal. 8, 16, 24.
} wav_header_t;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct format_fmt_ext_s {
  char subchunk2ID[4]; // "data"  string
  int32_t subchunk2Size;  // Sampled data length
} format_fmt_ext_t;
#pragma pack(pop)


typedef struct wav_s {
  FILE *file;
  char *name;
  wav_header_t header;
} wav_t;


/*
typedef struct {
sFILE *file;
  long lastSample;
  short lastIndex;
} sFILE_ADPCM;
typedef struct {
  short iCoef1;
  short iCoef2;
} sADPCMCOEFSET;
*/


//##################################################################################################
//## flag.
//##################################################################################################
#define WAV_MAKEFOURCC(b0,b1,b2,b3) ((b0)|((b1)<<8)|((b2)<<16)|((b3)<<24))
#define WAV_MAKETWOCC(b0,b1)        ((b0)|((b1)<<8))

#define WAV_MONO         1
#define WAV_STEREO       2

// type.
#define WAV_RIFF_MAGIC   WAV_MAKEFOURCC('R','I','F','F')
#define WAV_WAVE_MAGIC   WAV_MAKEFOURCC('W','A','V','E')

// différents chunk.
#define WAVE_CHUNK_FMT    WAV_MAKEFOURCC('f','m','t',' ')
#define WAVE_CHUNK_DATA   WAV_MAKEFOURCC('d','a','t','a')
#define WAVE_CHUNK_FACT   WAV_MAKEFOURCC('f','a','c','t')
#define WAVE_CHUNK_SLNT   WAV_MAKEFOURCC('s','l','n','t')
#define WAVE_CHUNK_CUE    WAV_MAKEFOURCC('c','u','e',' ')
#define WAVE_CHUNK_PLST   WAV_MAKEFOURCC('p','l','s','t')
#define WAVE_CHUNK_LIST   WAV_MAKEFOURCC('l','i','s','t')
#define WAVE_CHUNK_LABL   WAV_MAKEFOURCC('l','a','b','l')
#define WAVE_CHUNK_NOTE   WAV_MAKEFOURCC('n','o','t','e')
#define WAVE_CHUNK_LTXT   WAV_MAKEFOURCC('m','t','x','t')
#define WAVE_CHUNK_SMPL   WAV_MAKEFOURCC('s','m','p','l')
#define WAVE_CHUNK_INST   WAV_MAKEFOURCC('i','n','s','t')



// type de compression.
#define  WAVE_FORMAT_UNKNOWN                0x0000  // Microsoft Corporation.
#ifndef  WAVE_FORMAT_PCM
    #define  WAVE_FORMAT_PCM                0x0001  // Microsoft Corporation.
#endif
#define  WAVE_FORMAT_ADPCM                  0x0002  // Microsoft Corporation.
#define  WAVE_FORMAT_IEEE_FLOAT             0x0003  // Microsoft Corporation IEEE754: range (+1,-1] 32-bit/64-bit format as defined by MSVC++ float/double type.
#define  WAVE_FORMAT_IBM_CVSD               0x0005  // IBM Corporation.
#define  WAVE_FORMAT_ALAW                   0x0006  // Microsoft Corporation.
#define  WAVE_FORMAT_MULAW                  0x0007  // Microsoft Corporation.
#define  WAVE_FORMAT_OKI_ADPCM              0x0010  // OKI.
#define  WAVE_FORMAT_DVI_ADPCM              0x0011  // Intel Corporation.
#define  WAVE_FORMAT_IMA_ADPCM              WAVE_FORMAT_DVI_ADPCM // Intel Corporation.
#define  WAVE_FORMAT_MEDIASPACE_ADPCM       0x0012  // Videologic.
#define  WAVE_FORMAT_SIERRA_ADPCM           0x0013  // Sierra Semiconductor Corp.
#define  WAVE_FORMAT_G723_ADPCM             0x0014  // Antex Electronics Corporation.
#define  WAVE_FORMAT_DIGISTD                0x0015  // DSP Solutions, Inc.
#define  WAVE_FORMAT_DIGIFIX                0x0016  // DSP Solutions, Inc.
#define  WAVE_FORMAT_DIALOGIC_OKI_ADPCM     0x0017  // Dialogic Corporation.
#define  WAVE_FORMAT_MEDIAVISION_ADPCM      0x0018  // Media Vision, Inc.
#define  WAVE_FORMAT_YAMAHA_ADPCM           0x0020  // Yamaha Corporation of America.
#define  WAVE_FORMAT_SONARC                 0x0021  // Speech Compression.
#define  WAVE_FORMAT_DSPGROUP_TRUESPEECH    0x0022  // DSP Group, Inc.
#define  WAVE_FORMAT_ECHOSC1                0x0023  // Echo Speech Corporation.
#define  WAVE_FORMAT_AUDIOFILE_AF36         0x0024  //
#define  WAVE_FORMAT_APTX                   0x0025  // Audio Processing Technology.
#define  WAVE_FORMAT_AUDIOFILE_AF10         0x0026  //
#define  WAVE_FORMAT_DOLBY_AC2              0x0030  // Dolby Laboratories.
#define  WAVE_FORMAT_GSM610                 0x0031  // Microsoft Corporation.
#define  WAVE_FORMAT_MSNAUDIO               0x0032  // Microsoft Corporation.
#define  WAVE_FORMAT_ANTEX_ADPCME           0x0033  // Antex Electronics Corporation.
#define  WAVE_FORMAT_CONTROL_RES_VQLPC      0x0034  // Control Resources Limited.
#define  WAVE_FORMAT_DIGIREAL               0x0035  // DSP Solutions, Inc.
#define  WAVE_FORMAT_DIGIADPCM              0x0036  // DSP Solutions, Inc.
#define  WAVE_FORMAT_CONTROL_RES_CR10       0x0037  // Control Resources Limited.
#define  WAVE_FORMAT_NMS_VBXADPCM           0x0038  // Natural MicroSystems.
#define  WAVE_FORMAT_CS_IMAADPCM            0x0039 // Crystal Semiconductor IMA ADPCM.
#define  WAVE_FORMAT_ECHOSC3                0x003A // Echo Speech Corporation.
#define  WAVE_FORMAT_ROCKWELL_ADPCM         0x003B  // Rockwell International.
#define  WAVE_FORMAT_ROCKWELL_DIGITALK      0x003C  // Rockwell International.
#define  WAVE_FORMAT_XEBEC                  0x003D  // Xebec Multimedia Solutions Limited.
#define  WAVE_FORMAT_G721_ADPCM             0x0040  // Antex Electronics Corporation.
#define  WAVE_FORMAT_G728_CELP              0x0041  // Antex Electronics Corporation.
#define  WAVE_FORMAT_MPEG                   0x0050  // Microsoft Corporation.
#define  WAVE_FORMAT_MPEGLAYER3             0x0055  // ISO/MPEG Layer3 Format Tag.
#define  WAVE_FORMAT_CIRRUS                 0x0060  // Cirrus Logic.
#define  WAVE_FORMAT_ESPCM                  0x0061  // ESS Technology.
#define  WAVE_FORMAT_VOXWARE                0x0062  // Voxware Inc.
#define  WAVE_FORMAT_CANOPUS_ATRAC          0x0063  // Canopus, co., Ltd.
#define  WAVE_FORMAT_G726_ADPCM             0x0064  // APICOM.
#define  WAVE_FORMAT_G722_ADPCM             0x0065  // APICOM.
#define  WAVE_FORMAT_DSAT                   0x0066  // Microsoft Corporation.
#define  WAVE_FORMAT_DSAT_DISPLAY           0x0067  // Microsoft Corporation.
#define  WAVE_FORMAT_SOFTSOUND              0x0080  // Softsound, Ltd.
#define  WAVE_FORMAT_RHETOREX_ADPCM         0x0100  // Rhetorex Inc.
#define  WAVE_FORMAT_CREATIVE_ADPCM         0x0200  // Creative Labs, Inc.
#define  WAVE_FORMAT_CREATIVE_FASTSPEECH8   0x0202  // Creative Labs, Inc.
#define  WAVE_FORMAT_CREATIVE_FASTSPEECH10  0x0203  // Creative Labs, Inc.
#define  WAVE_FORMAT_QUARTERDECK            0x0220  // Quarterdeck Corporation.
#define  WAVE_FORMAT_FM_TOWNS_SND           0x0300  // Fujitsu Corp.
#define  WAVE_FORMAT_BTV_DIGITAL            0x0400  // Brooktree Corporation.
#define  WAVE_FORMAT_OLIGSM                 0x1000  // Ing C. Olivetti & C., S.p.A.
#define  WAVE_FORMAT_OLIADPCM               0x1001  // Ing C. Olivetti & C., S.p.A.
#define  WAVE_FORMAT_OLICELP                0x1002  // Ing C. Olivetti & C., S.p.A.
#define  WAVE_FORMAT_OLISBC                 0x1003  // Ing C. Olivetti & C., S.p.A.
#define  WAVE_FORMAT_OLIOPR                 0x1004  // Ing C. Olivetti & C., S.p.A.
#define  WAVE_FORMAT_LH_CODEC               0x1100  // Lernout & Hauspie.
#define  WAVE_FORMAT_NORRIS                 0x1400  // Norris Communications, Inc.
#define  WAVE_FORMAT_DEVELOPMENT            0xFFFF  // Experimental.





//##################################################################################################
//## prototype
//##################################################################################################
int wav_read(sample_t *psample, wav_t *f);
sample_t wav_open(const char *pfilename, char *pflag);
long wav_close(wav_t *f);
void wav_open_dir(char *pfilepath, sample_t *psample, int count_max);



#endif

