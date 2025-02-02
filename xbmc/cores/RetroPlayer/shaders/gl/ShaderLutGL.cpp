/*
 *  Copyright (C) 2019 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "ShaderLutGL.h"

#include "ShaderUtilsGL.h"
#include "cores/RetroPlayer/rendering/RenderContext.h"
#include "cores/RetroPlayer/shaders/IShaderPreset.h"
#include "rendering/gl/RenderSystemGL.h"
#include "guilib/Texture.h"
#ifndef HAS_GLES
#include "guilib/TextureGL.h"
#else
#include "guilib/TextureGLES.h"
#endif
#include "utils/log.h"

#include <utility>

using namespace KODI;
using namespace SHADER;

CShaderLutGL::CShaderLutGL(const std::string& id, const std::string& path) : IShaderLut(id, path)
{
}

CShaderLutGL::~CShaderLutGL() = default;

bool CShaderLutGL::Create(RETRO::CRenderContext& context, const ShaderLut& lut)
{
  std::unique_ptr<CTexture> lutTexture(CreateLUTTexture(context, lut));
  if (!lutTexture)
  {
    CLog::Log(LOGWARNING, "{} - Couldn't create a LUT texture for LUT {}", __FUNCTION__, lut.strId);
    return false;
  }

  m_texture = std::move(lutTexture);
  return true;
}

std::unique_ptr<CTexture> CShaderLutGL::CreateLUTTexture(RETRO::CRenderContext& context,
                                                               const ShaderLut& lut)
{
  std::unique_ptr<CTexture> texture = CTexture::LoadFromFile(lut.path);
#ifndef HAS_GLES
  auto* textureGL = static_cast<CGLTexture*>(texture.get());
#else
  auto* textureGL = static_cast<CGLESTexture*>(texture.get());
#endif

  if (textureGL == nullptr)
  {
    CLog::Log(LOGERROR, "Couldn't open LUT {}", lut.path);
    return std::unique_ptr<CTexture>();
  }

  if (lut.mipmap)
    textureGL->SetMipmapping();

  textureGL->SetScalingMethod(lut.filter == FILTER_TYPE_LINEAR ? TEXTURE_SCALING::LINEAR
                                                               : TEXTURE_SCALING::NEAREST);
  textureGL->LoadToGPU();

  auto wrapType = CShaderUtilsGL::TranslateWrapType(lut.wrap);

  glBindTexture(GL_TEXTURE_2D, textureGL->getMTexture());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);

#ifndef HAS_GLES
  GLfloat blackBorder[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, blackBorder);
#endif

#ifndef HAS_GLES
  return texture;
#else
  return texture;
#endif
}
