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

class TextureCube
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

    unsigned int Size() const {
        return _size;
    }

    std::vector<Texture2D> textures;

protected:
    unsigned int _size;
};

