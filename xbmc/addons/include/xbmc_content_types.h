#pragma once
/*
 *      Copyright (C) 2013 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#ifndef XBMC_CONTENT_TYPES_H_
#define XBMC_CONTENT_TYPES_H_

/* current content API version */
#define XBMC_CONTENT_API_VERSION "0.1.0"

/* min. content API version */
#define XBMC_CONTENT_MIN_API_VERSION "0.1.0"

#ifdef _WIN32
#include <windows.h>
#else
#ifndef __cdecl
#define __cdecl
#endif
#ifndef __declspec
#define __declspec(X)
#endif
#endif

#undef ATTRIBUTE_PACKED
#undef PRAGMA_PACK_BEGIN
#undef PRAGMA_PACK_END

#if defined(__GNUC__)
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#define ATTRIBUTE_PACKED __attribute__ ((packed))
#define PRAGMA_PACK 0
#endif
#endif

#if !defined(ATTRIBUTE_PACKED)
#define ATTRIBUTE_PACKED
#define PRAGMA_PACK 1
#endif

#define CONTENT_ADDON_NAME_STRING_LENGTH         1024
#define CONTENT_ADDON_FILENAME_STRING_LENGTH     1024
#define CONTENT_ADDON_TEXT_LENGTH                4096

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct CONTENT_ADDON_PLAYLIST
  {
    char strPath[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strName[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strThumb[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strFanart[CONTENT_ADDON_FILENAME_STRING_LENGTH];
  } ATTRIBUTE_PACKED CONTENT_ADDON_PLAYLIST;

  typedef struct CONTENT_ADDON_DIRECTORY
  {
    char strPath[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strName[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strThumb[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strFanart[CONTENT_ADDON_FILENAME_STRING_LENGTH];
  } ATTRIBUTE_PACKED CONTENT_ADDON_DIRECTORY;

  typedef struct CONTENT_ADDON_SONG
  {
    char strFilename[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strThumb[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strFanart[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strTitle[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strArtist[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strAlbum[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strAlbumArtist[CONTENT_ADDON_NAME_STRING_LENGTH];
    int  iTrack;
    int  iDuration;
    int  iYear;
    int  iRating;
  } ATTRIBUTE_PACKED CONTENT_ADDON_SONG;

  typedef struct CONTENT_ADDON_ARTIST
  {
    char strPath[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strArtist[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strGenres[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strBiography[CONTENT_ADDON_TEXT_LENGTH];
    char strStyles[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strMoods[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strInstruments[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strBorn[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strFormed[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strDied[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strDisbanded[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strYearsActive[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strThumb[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strFanart[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strDiscography[CONTENT_ADDON_NAME_STRING_LENGTH];
  } ATTRIBUTE_PACKED CONTENT_ADDON_ARTIST;

  typedef struct CONTENT_ADDON_ALBUM
  {
    char strPath[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strAlbum[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strArtists[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strGenres[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strStyles[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strMoods[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strThemes[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strLabel[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strType[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strDateOfRelease[CONTENT_ADDON_NAME_STRING_LENGTH];
    char strReview[CONTENT_ADDON_TEXT_LENGTH];
    char strThumb[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    char strFanart[CONTENT_ADDON_FILENAME_STRING_LENGTH];
    int  bCompilation;
    int  iYear;
    int  iRating;
  } ATTRIBUTE_PACKED CONTENT_ADDON_ALBUM;

  typedef enum CONTENT_ADDON_TYPE
  {
    CONTENT_ADDON_TYPE_SONG,
    CONTENT_ADDON_TYPE_ARTIST,
    CONTENT_ADDON_TYPE_DIRECTORY,
    CONTENT_ADDON_TYPE_PLAYLIST,
    CONTENT_ADDON_TYPE_ALBUM
  } CONTENT_ADDON_TYPE;

  typedef struct CONTENT_ADDON_FILEITEM
  {
    CONTENT_ADDON_TYPE type;
    union
    {
      CONTENT_ADDON_PLAYLIST  playlist;
      CONTENT_ADDON_DIRECTORY directory;
      CONTENT_ADDON_ARTIST    artist;
      CONTENT_ADDON_SONG      song;
      CONTENT_ADDON_ALBUM     album;
    };
  } ATTRIBUTE_PACKED CONTENT_ADDON_FILEITEM;

  typedef struct CONTENT_ADDON_FILELIST
  {
    unsigned int iSize;
    CONTENT_ADDON_FILEITEM* items;
  } ATTRIBUTE_PACKED CONTENT_ADDON_FILELIST;

  typedef enum CONTENT_ADDON_PLAYSTATE
  {
    CONTENT_ADDON_PLAYSTATE_PLAY,
    CONTENT_ADDON_PLAYSTATE_PAUSE,
    CONTENT_ADDON_PLAYSTATE_STOP
  } CONTENT_ADDON_PLAYSTATE;

  typedef enum CONTENT_ADDON_AUDIO_FORMAT // TODO !
  {
    CONTENT_FMT_INVALID = -1,

    CONTENT_FMT_U8,
    CONTENT_FMT_S8,

    CONTENT_FMT_S16BE,
    CONTENT_FMT_S16LE,
    CONTENT_FMT_S16NE,

    CONTENT_FMT_S32BE,
    CONTENT_FMT_S32LE,
    CONTENT_FMT_S32NE,

    CONTENT_FMT_S24BE4,
    CONTENT_FMT_S24LE4,
    CONTENT_FMT_S24NE4, /* S24 in 4 bytes */

    CONTENT_FMT_S24BE3,
    CONTENT_FMT_S24LE3,
    CONTENT_FMT_S24NE3, /* S24 in 3 bytes */

    CONTENT_FMT_DOUBLE,
    CONTENT_FMT_FLOAT,

    /* Bitstream formats */
    CONTENT_FMT_AAC,
    CONTENT_FMT_AC3,
    CONTENT_FMT_DTS,
    CONTENT_FMT_EAC3,
    CONTENT_FMT_TRUEHD,
    CONTENT_FMT_DTSHD,
    CONTENT_FMT_LPCM,

    CONTENT_FMT_MAX
  } CONTENT_ADDON_AUDIO_FORMAT;

  typedef struct CONTENT_ADDON_CODEC_INFO
  {
    char                       strCodecName[32];
    CONTENT_ADDON_AUDIO_FORMAT format;
    int                        iSampleRate;
    int                        iChannels;
    int                        iBitsPerSample;
    int                        iBitrate;
  } CONTENT_ADDON_CODEC_INFO;

  /*!
   * @brief content add-on error codes
   */
  typedef enum CONTENT_ERROR
  {
    CONTENT_ERROR_NO_ERROR           = 0,  /*!< @brief no error occurred */
    CONTENT_ERROR_UNKNOWN            = -1, /*!< @brief an unknown error occurred */
    CONTENT_ERROR_NOT_IMPLEMENTED    = -2, /*!< @brief the method that XBMC called is not implemented by the add-on */
    CONTENT_ERROR_REJECTED           = -3, /*!< @brief the command was rejected by the backend */
    CONTENT_ERROR_INVALID_PARAMETERS = -4, /*!< @brief the parameters of the method that was called are invalid for this operation */
    CONTENT_ERROR_FAILED             = -5, /*!< @brief the command failed */
  } CONTENT_ERROR;

  /*!
   * @brief Properties passed to the Create() method of an add-on.
   */
  typedef struct CONTENT_PROPERTIES
  {
    const char* strUserPath;           /*!< @brief path to the user profile */
    const char* strClientPath;         /*!< @brief path to this add-on */
  } CONTENT_PROPERTIES;

  typedef enum CONTENT_TOP100_TYPE
  {
    CONTENT_TOP100_TYPE_ALBUMS,
    CONTENT_TOP100_TYPE_ARTISTS,
    CONTENT_TOP100_TYPE_SONGS
  } CONTENT_TOP100_TYPE;

  /*!
   * @brief Structure to transfer the methods from xbmc_content_dll.h to XBMC
   */
  typedef struct ContentAddon
  {
    const char*      (__cdecl* GetContentAPIVersion)(void);
    const char*      (__cdecl* GetMininumContentAPIVersion)(void);
    const char*      (__cdecl* GetServerName)(void);
    void             (__cdecl* FreeFileList)(CONTENT_ADDON_FILELIST*);
    bool             (__cdecl* SupportsFile)(const char* strFilename);

    /// @name Music files
    ///{
    CONTENT_ERROR    (__cdecl* MusicGetPlaylists)(CONTENT_ADDON_FILELIST**);
    CONTENT_ERROR    (__cdecl* MusicGetPlaylist)(CONTENT_ADDON_FILELIST**, const char*);
    CONTENT_ERROR    (__cdecl* MusicGetArtists)(CONTENT_ADDON_FILELIST**);
    CONTENT_ERROR    (__cdecl* MusicGetAlbums)(CONTENT_ADDON_FILELIST**, const char*);
    CONTENT_ERROR    (__cdecl* MusicGetSongs)(CONTENT_ADDON_FILELIST**, const char*, const char*);
    CONTENT_ERROR    (__cdecl* MusicGetTop100)(CONTENT_ADDON_FILELIST**, CONTENT_TOP100_TYPE);
    CONTENT_ERROR    (__cdecl* MusicGetOverviewItems)(CONTENT_ADDON_FILELIST**);
    CONTENT_ERROR    (__cdecl* MusicSearch)(CONTENT_ADDON_FILELIST**, const char*);
    ///}

    /// @name Music codec
    ///{
    CONTENT_ERROR    (__cdecl* MusicOpenFile)(const char*);
    CONTENT_ERROR    (__cdecl* MusicPreloadFile)(const char*);
    void             (__cdecl* MusicCloseFile)(void);
    CONTENT_ERROR    (__cdecl* MusicPause)(int);
    CONTENT_ERROR    (__cdecl* MusicGetCodecInfo)(const char*, CONTENT_ADDON_CODEC_INFO*);
    int64_t          (__cdecl* MusicSeek)(int64_t);
    int              (__cdecl* MusicReadPCM)(unsigned char*, int, int*);
    ///}
  } ContentClient;

#ifdef __cplusplus
}
#endif

#endif /* XBMC_CONTENT_TYPES_H_ */
