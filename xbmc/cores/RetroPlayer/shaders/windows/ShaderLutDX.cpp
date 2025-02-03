/*
 *  Copyright (C) 2017-2019 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "ShaderLutDX.h"

#include "ShaderUtilsDX.h"
#include "cores/RetroPlayer/rendering/RenderContext.h"
#include "cores/RetroPlayer/shaders/IShaderPreset.h"
#include "rendering/dx/RenderSystemDX.h"
#include "guilib/TextureDX.h"
#include "utils/log.h"

#include <utility>

using namespace KODI;
using namespace SHADER;

CShaderLutDX::CShaderLutDX(const std::string& id, const std::string& path) : IShaderLut(id, path)
{
}

CShaderLutDX::~CShaderLutDX() = default;

bool CShaderLutDX::Create(RETRO::CRenderContext& context, const ShaderLut& lut)
{
  std::unique_ptr<CTexture> lutTexture(CreateLUTexture(lut));
  if (!lutTexture)
  {
    CLog::LogF(LOGWARNING, "Couldn't create a texture for LUT: {}", lut.strId);
    return false;
  }

  m_texture = std::move(lutTexture);
  return true;
}

std::unique_ptr<CTexture> CShaderLutDX::CreateLUTexture(const ShaderLut& lut)
{
  std::unique_ptr<CTexture> texture = CTexture::LoadFromFile(lut.path);
  auto* textureDX = static_cast<CDXTexture*>(texture.get());

  if (textureDX == nullptr)
  {
    CLog::Log(LOGERROR, "Couldn't open LUT: {}", lut.path);
    return std::unique_ptr<CTexture>();
  }

  if (lut.mipmap)
    textureDX->SetMipmapping();

  textureDX->SetScalingMethod(lut.filter == FILTER_TYPE_LINEAR ? TEXTURE_SCALING::LINEAR
                                                               : TEXTURE_SCALING::NEAREST);
  textureDX->LoadToGPU();

  return texture;
}
