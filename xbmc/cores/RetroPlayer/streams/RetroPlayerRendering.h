/*
 *      Copyright (C) 2012-2017 Team Kodi
 *      http://kodi.tv
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
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "IRetroPlayerStream.h"
#include "RetroPlayerStreamTypes.h"

#include <stdint.h>

extern "C"
{
#include <libavutil/pixfmt.h>
}

namespace KODI
{
namespace RETRO
{
class CRPProcessInfo;
class CRPRenderManager;

struct HwFramebufferBuffer : public StreamBuffer
{
  HwFramebufferBuffer() = default;

  HwFramebufferBuffer(uintptr_t framebuffer,
                      uintptr_t texture)
    : framebuffer(framebuffer), texture(texture)
  {
  }

  uintptr_t framebuffer;
  uintptr_t texture;
};

struct HwFramebufferPacket : public StreamPacket
{
  HwFramebufferPacket(uintptr_t framebuffer,
                      uintptr_t texture)
    : framebuffer(framebuffer),
      texture(texture)
  {
  }

  uintptr_t framebuffer;
  uintptr_t texture;
};

struct RenderingStreamProperties : public StreamProperties
{
  RenderingStreamProperties(AVPixelFormat pixfmt,
                            unsigned int nominalWidth,
                            unsigned int nominalHeight,
                            unsigned int maxWidth,
                            unsigned int maxHeight,
                            float pixelAspectRatio)
    : pixfmt(pixfmt),
      nominalWidth(nominalWidth),
      nominalHeight(nominalHeight),
      maxWidth(maxWidth),
      maxHeight(maxHeight),
      pixelAspectRatio(pixelAspectRatio)
  {
  }

  AVPixelFormat pixfmt;
  unsigned int nominalWidth;
  unsigned int nominalHeight;
  unsigned int maxWidth;
  unsigned int maxHeight;
  float pixelAspectRatio;
};

class CRetroPlayerRendering : public IRetroPlayerStream
{
public:
  CRetroPlayerRendering(CRPRenderManager& m_renderManager, CRPProcessInfo& m_processInfo);

  ~CRetroPlayerRendering() override;

  // implementation of IRetroPlayerStream
  bool OpenStream(const StreamProperties& properties) override;
  bool GetStreamBuffer(unsigned int width, unsigned int height, StreamBuffer& buffer) override;
  void AddStreamData(const StreamPacket& packet) override;
  void CloseStream() override;

private:
  // Construction parameters
  CRPRenderManager& m_renderManager;
  CRPProcessInfo& m_processInfo;
};
} // namespace RETRO
} // namespace KODI
