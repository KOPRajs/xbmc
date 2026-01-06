/*
 *  Copyright (C) 2019-2025 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "cores/RetroPlayer/shaders/IShaderTexture.h"

#include <memory>

#include "system_gl.h"

namespace KODI::RETRO
{
class IRenderBuffer;
} // namespace KODI::RETRO

namespace KODI::SHADER
{

class CShaderTextureGL : public IShaderTexture
{
public:
  CShaderTextureGL(std::shared_ptr<RETRO::IRenderBuffer> texture, bool sRgbFramebuffer);
  ~CShaderTextureGL() override;

  // Implementation of IShaderTexture
  float GetWidth() const override;
  float GetHeight() const override;

  // OpenGL interface
  RETRO::IRenderBuffer& GetTexture() { return *m_texture; }
  const RETRO::IRenderBuffer& GetTexture() const { return *m_texture; }
  bool IsSRGBFramebuffer() const { return m_sRgbFramebuffer; }

  // Frame buffer interface
  bool CreateFBO();
  bool BindFBO();
  void UnbindFBO() const;

private:
  std::shared_ptr<RETRO::IRenderBuffer> m_texture;
  bool m_sRgbFramebuffer{false};
  GLuint FBO{0};
};

} // namespace KODI::SHADER
