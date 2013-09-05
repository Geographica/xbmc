/*
 *      Copyright (C) 2012-2013 Team XBMC
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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */
#pragma once

#include "RetroPlayerVideo.h"
#include "RetroPlayerAudio.h"
#include "RetroPlayerInput.h"
#include "cores/IPlayer.h"
#include "FileItem.h"
#include "games/GameClient.h"
#include "games/libretro/LibretroCallbacks.h"
#include "threads/Thread.h"

#include <stdint.h>
#include <string>
#include <vector>

class CRetroPlayer : public IPlayer, public GAMES::ILibretroCallbacksAV, public CThread
{
public:
  CRetroPlayer(IPlayerCallback& callback);
  virtual ~CRetroPlayer();

  virtual bool OpenFile(const CFileItem& file, const CPlayerOptions& options);
  virtual bool CloseFile();

  virtual bool OnAction(const CAction &action);

  // Upon successful open, m_file is set to the opened file
  virtual bool IsPlaying() const { return !m_bStop && !m_file.GetPath().empty(); }
  virtual void Pause();
  virtual bool IsPaused() const { return m_playSpeed == 0; }

  const CStdString &GetFilePath() { return m_file.GetPath(); }
  CStdString GetGameClient() { return m_gameClient ? m_gameClient->ID() : ""; }

  virtual bool HasVideo() const { return true; }
  virtual bool HasAudio() const { return true; }

  CRetroPlayerInput &GetInput() { return m_input; }

  virtual void GetAudioInfo(CStdString& strAudioInfo) { strAudioInfo = "CRetroPlayer:GetAudioInfo"; }
  virtual void GetVideoInfo(CStdString& strVideoInfo) { strVideoInfo = "CRetroPlayer:GetVideoInfo"; }
  virtual void GetGeneralInfo(CStdString& strGeneralInfo) { strGeneralInfo = "CRetroPlayer:GetGeneralInfo"; }

  //virtual CStdString GetAudioCodecName() { return ""; }
  //virtual CStdString GetVideoCodecName() { return ""; }
  //virtual int  GetAudioBitrate() { return 0; }
  //virtual int  GetChannels() { return 0; }
  //virtual int  GetBitsPerSample() { return 0; }
  //virtual int  GetSampleRate() { return 0; }

  //virtual void GetVideoRect(CRect& SrcRect, CRect& DestRect) { }
  //virtual void GetVideoAspectRatio(float& fAR) { fAR = 1.0f; }
  //virtual int  GetPictureWidth() { return 0; }
  //virtual int  GetPictureHeight() { return 0; }
  //virtual bool GetStreamDetails(CStreamDetails &details) { return false; }

  //virtual int  GetAudioStreamCount() { return 0; }
  //virtual int  GetAudioStream() { return -1; }
  //virtual void GetAudioStreamName(int iStream, CStdString &strStreamName) { }
  //virtual void SetAudioStream(int iStream) { }
  //virtual void GetAudioStreamLanguage(int iStream, CStdString &strLanguage) { }

  //virtual void  SetAVDelay(float fValue = 0.0f) { return; }
  //virtual float GetAVDelay() { return 0.0f;};

  virtual void ToFFRW(int iSpeed = 0);

  // A "back buffer" is used to store game history to enable rewinding Braid-style.
  // The time is computed from the number of frames avaiable in the buffer, and
  // allows seeking and rewinding over this time range. Fast-forwarding will
  // simply play the game at a faster speed, and will cause this buffer to fill
  // faster. When the buffer is full, the progress bar will reach 100% (60s by
  // default), which will look like a track being finished, but instead of
  // exiting the game will continue to play.
  virtual bool CanSeek() { return true; }
  virtual void Seek(bool bPlus = true, bool bLargeStep = false);
  virtual void SeekPercentage(float fPercent = 0);
  virtual float GetPercentage();
  virtual void SeekTime(int64_t iTime = 0);
  virtual int64_t GetTime();
  virtual int64_t GetTotalTime();

  bool Save(unsigned int slot) { return m_gameClient && m_gameClient->Save(slot); }
  bool Save(const CStdString &label) { return m_gameClient && m_gameClient->Save(label); }
  bool Load(const CStdString &saveStatePath) { return m_gameClient && m_gameClient->Load(saveStatePath); }

  /*
   * Inherited from ILibretroCallbacksAV. Used to send and receive data from
   * the game clients.
   */
  virtual void VideoFrame(const void *data, unsigned width, unsigned height, size_t pitch);
  virtual void AudioSample(int16_t left, int16_t right);
  virtual size_t AudioSampleBatch(const int16_t *data, size_t frames);
  virtual int16_t GetInputState(unsigned port, unsigned device, unsigned index, unsigned id);
  virtual void SetPixelFormat(LIBRETRO::retro_pixel_format pixelFormat);
  virtual void SetKeyboardCallback(LIBRETRO::retro_keyboard_event_t callback);

protected:
  virtual void Process();

private:
  /**
   * Dump game information (if any) to the debug log.
   */
  void PrintGameInfo(const CFileItem &file) const;

  LIBRETRO::retro_keyboard_event_t m_keyboardCallback; // TODO

  CRetroPlayerVideo    m_video;
  CRetroPlayerAudio    m_audio;
  CRetroPlayerInput    m_input;
  GAMES::GameClientPtr m_gameClient;

  CFileItem            m_file;
  CPlayerOptions       m_PlayerOptions;
  int                  m_playSpeed; // Normal play speed is PLAYSPEED_NORMAL (1000)
  CEvent               m_pauseEvent;
};
