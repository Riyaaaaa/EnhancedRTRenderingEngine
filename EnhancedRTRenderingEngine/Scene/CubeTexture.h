#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Resource/Texture2D.h"

enum CUBE_DIRECTION {
    PX = 0,
    NX,
    PY,
    NY,
    PZ,
    NZ
};

class CubeTexture
{
public:
    static DirectX::XMVECTOR lookAt[6];
    static DirectX::XMVECTOR up[6];

    CubeTexture();

    const ResourceHandle<Texture2D>& operator[](CUBE_DIRECTION dir) {
        return textures[dir];
    }

    std::vector<ResourceHandle<Texture2D>> textures;
};

