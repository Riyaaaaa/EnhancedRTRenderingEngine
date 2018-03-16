#include "stdafx.h"
#include "TextureCube.h"

using namespace DirectX;

TextureCube::TextureCube(int size)
{
    textures.reserve(6);
    for (int i = 0; i < 6; i++) {
        textures.emplace_back(Texture2D(size, size, 4));
    }

    width = height = size;
    channels = 4;
    stride = size * 4;

    _ptr = &textures[0];
    _size = size;
}

TextureCube::TextureCube(const std::vector<Texture2D>& texResource) {
    textures = texResource;
    _size = textures[0].Width();

    width = height = _size;
    channels = 4;
    stride = _size * 4;

    _ptr = &textures[0];
}

DirectX::XMVECTOR TextureCube::lookAt[6] = {
    XMVectorSet(1.0f,  0.0f,  0.0f, 0.0f), // +X
    XMVectorSet(-1.0f,  0.0f,  0.0f, 0.0f), // -X
    XMVectorSet(0.0f,  1.0f,  0.0f, 0.0f), // +Y
    XMVectorSet(0.0f, -1.0f,  0.0f, 0.0f), // -Y
    XMVectorSet(0.0f,  0.0f,  1.0f, 0.0f), // +Z
    XMVectorSet(0.0f,  0.0f, -1.0f, 0.0f) // -Z
};

DirectX::XMVECTOR TextureCube::up[6] = {
    XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f), // +X(Up = +Y)
    XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f), // -X(Up = +Y)
    XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), // +Y(Up = -Z)
    XMVectorSet(0.0f, 0.0f,  1.0f, 0.0f), // -X(Up = +Z)
    XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f), // +Z(Up = +Y)
    XMVectorSet(0.0f, 1.0f,  0.0f, 0.0f), // -Z(Up = +Y)
};
