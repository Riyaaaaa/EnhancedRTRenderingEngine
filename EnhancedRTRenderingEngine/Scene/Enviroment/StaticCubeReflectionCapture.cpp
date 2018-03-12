#include "stdafx.h"
#include "StaticCubeReflectionCapture.h"
#include "GraphicsInterface/GITexture.h"

StaticCubeReflectionCapture::StaticCubeReflectionCapture(const CubeTexture& tex) :
    reflectionSource(tex)
{
    box.w = box.h = box.d = reflectionSource.size;
}

void StaticCubeReflectionCapture::SetupTexture(GITexture* tex) {
    TextureParam param;
    param.arraySize = 6;
    param.width = reflectionSource.size;
    param.height = reflectionSource.size;
    param.bindFlag = TextureBindTarget::SHADER_RESOURCE;
    param.type = TextureType::TextureCube;
    param.format = TextureFormat::RGBA8_UNORM;
    tex->Initialize(param, reflectionSource.textures);
}