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

class TextureCube : public Texture2D
{
public:
    static DirectX::XMVECTOR lookAt[6];
    static DirectX::XMVECTOR up[6];

    TextureCube() = default;
    explicit TextureCube(int _size);
    TextureCube(const std::vector<Texture2D>& texResource);

    const Texture2D& operator[](CUBE_DIRECTION dir) {
        return textures[dir];
    }

    std::vector<Texture2D> textures;
};

