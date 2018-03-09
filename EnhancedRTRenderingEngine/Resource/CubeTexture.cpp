#include "stdafx.h"
#include "CubeTexture.h"

using namespace DirectX;

CubeTexture::CubeTexture()
{
    textures.reserve(6);
    for (int i = 0; i < 6; i++) {
        textures.emplace_back(Texture2D(1000, 1000, 4));
    }
}

DirectX::XMVECTOR CubeTexture::lookAt[6] = {
    XMVectorSet(1.0f,  0.0f,  0.0f, 0.0f), // +X
    XMVectorSet(-1.0f,  0.0f,  0.0f, 0.0f), // -X
    XMVectorSet(0.0f,  1.0f,  0.0f, 0.0f), // +Y
    XMVectorSet(0.0f, -1.0f,  0.0f, 0.0f), // -Y
    XMVectorSet(0.0f,  0.0f,  1.0f, 0.0f), // +Z
    XMVectorSet(0.0f,  0.0f, -1.0f, 0.0f) // -Z
};

DirectX::XMVECTOR CubeTexture::up[6] = {
    XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f), // +X(Up = +Y)
    XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f), // -X(Up = +Y)
    XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), // +Y(Up = -Z)
    XMVectorSet(0.0f, 0.0f,  1.0f, 0.0f), // -X(Up = +Z)
    XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f), // +Z(Up = +Y)
    XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f), // -Z(Up = +Y)
};
