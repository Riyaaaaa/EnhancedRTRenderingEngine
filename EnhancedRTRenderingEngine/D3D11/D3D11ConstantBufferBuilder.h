#pragma once

#include <d3d11.h>

#include "ComPtr.h"
#include "Structure/Structure.h"

#include "GraphicsInterface/GIImmediateCommands.h"

class Scene;

namespace D3D11ConstantBufferBuilder
{
    template <class BufferType>
    ComPtr<ID3D11Buffer> BuildConstantBuffer(GIImmediateCommands* cmd, BufferType* buffer) {
        D3D11_BUFFER_DESC bufferDesc;
        ComPtr<ID3D11Buffer> hpConstantBuffer(nullptr);

        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = sizeof(float);
        bufferDesc.ByteWidth = sizeof(BufferType);

        D3D11_SUBRESOURCE_DATA srd;
        D3D11_SUBRESOURCE_DATA* initPtr = nullptr;

        if (buffer) {
            srd.pSysMem = buffer;
            initPtr = &srd;
        }
       
        if (FAILED(cimd->CreateBuffer(&bufferDesc, initPtr, hpConstantBuffer.ToCreator()))) {
            return ComPtr<ID3D11Buffer>(nullptr);
        }

        return hpConstantBuffer;
    }
};

