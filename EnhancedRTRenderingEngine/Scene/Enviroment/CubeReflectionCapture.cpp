
#include "CubeReflectionCapture.h" 

#include "GraphicsInterface/GITextureProxy.h"


CubeReflectionCapture::CubeReflectionCapture(unsigned int size) {
    box.h = box.w = box.d = size;
    dirty = true;
}

void CubeReflectionCapture::SetupTexture(GIImmediateCommands* cmd, GITextureProxy tex) {
    TextureParam param;
    param.arraySize = 6;
    param.width = box.w;
    param.height = box.h;
    param.bindFlag = TextureBindTarget::SHADER_RESOURCE;
    param.type = TextureType::Texture2DArray;
    param.format = TextureFormat::RGBA8_UNORM;
    tex->Initialize(cmd, param);

    dirty = false;
}

bool CubeReflectionCapture::Contains(Vector3D pos) {
    auto& mpos = _transform.location;
    return pos.x >= mpos.x - box.w / 2 && pos.x <= mpos.x + box.w / 2 &&
        pos.y >= mpos.y - box.h / 2 && pos.y <= mpos.y + box.h / 2 &&
        pos.z >= mpos.z - box.d / 2 && pos.z <= mpos.z + box.d / 2;
           
}