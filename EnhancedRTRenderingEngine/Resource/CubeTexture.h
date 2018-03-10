#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Resource/Texture2D.h"
#include "Structure/Structure.h"

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

    CubeTexture(int _size = 1000);

    const ResourceHandle<Texture2D>& operator[](CUBE_DIRECTION dir) {
        return textures[dir];
    }

    int size;
    std::vector<ResourceHandle<Texture2D>> textures;
};

