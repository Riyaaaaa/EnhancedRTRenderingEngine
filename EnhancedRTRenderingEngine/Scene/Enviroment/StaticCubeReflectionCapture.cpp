
#include "StaticCubeReflectionCapture.h"
#include "GraphicsInterface/GITextureProxy.h"

#include "Utility/TextureUtils.h"

StaticCubeReflectionCapture::StaticCubeReflectionCapture(const TextureCube& tex) :
    reflectionSource(tex)
{
    box.w = box.h = box.d = reflectionSource.Size();
}

void StaticCubeReflectionCapture::SetupTexture(GIImmediateCommands* cmd, GITextureProxy tex) {
    TextureParam param;
    param.mipLevels = static_cast<unsigned int>(std::floor(std::log2(reflectionSource.Size())) + 1);
    param.arraySize = 6;
    param.width = reflectionSource.Size();
    param.height = reflectionSource.Size();
    param.bindFlag = TextureBindTarget::SHADER_RESOURCE;
    param.type = TextureType::TextureCube;
    param.format = TextureFormat::RGBA8_UNORM;
    
    auto mipmaps = TextureUtils::CreateMipmaps(reflectionSource, 0);
    tex->Initialize(cmd, param, mipmaps);
}