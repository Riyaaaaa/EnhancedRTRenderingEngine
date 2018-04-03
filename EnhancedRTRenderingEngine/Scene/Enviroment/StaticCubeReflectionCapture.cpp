#include "stdafx.h"
#include "StaticCubeReflectionCapture.h"
#include "GraphicsInterface/GITextureProxy.h"

StaticCubeReflectionCapture::StaticCubeReflectionCapture(const TextureCube& tex) :
    reflectionSource(tex)
{
    box.w = box.h = box.d = reflectionSource.Size();
}

void StaticCubeReflectionCapture::SetupTexture(GIImmediateCommands* cmd, GITextureProxy tex) {
    TextureParam param;
    param.arraySize = 6;
    param.width = reflectionSource.Size();
    param.height = reflectionSource.Size();
    param.bindFlag = TextureBindTarget::SHADER_RESOURCE;
    param.type = TextureType::TextureCube;
    param.format = TextureFormat::RGBA8_UNORM;
    tex->Initialize(cmd, param, reflectionSource.textures);
}