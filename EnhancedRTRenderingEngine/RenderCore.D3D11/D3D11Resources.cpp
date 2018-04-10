#include "stdafx.h"
#include "D3D11Resources.h"

#include "D3D11FormatUtils.h"

TextureParam D3D11Texture2D::GetTextureParam() {
    TextureParam param;
    D3D11_TEXTURE2D_DESC desc;
    resource->GetDesc(&desc);

    param.width = desc.Width;
    param.height = desc.Height;
    param.accessFlag = CastToGIFormat<ResourceAccessFlag>(desc.CPUAccessFlags);
    param.arraySize = desc.ArraySize;
    param.usage = CastToGIFormat<ResourceUsage>(desc.Usage);
    param.format = CastToGIFormat<TextureFormat>(desc.Format);
    param.bindFlag = CastToGIFormat<unsigned int>(desc.BindFlags);
    param.type = desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE ? TextureType::TextureCube : TextureType::Texture2D;
    param.mipLevels = desc.MipLevels;

    return param;
}