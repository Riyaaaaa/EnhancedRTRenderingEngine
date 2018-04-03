#include "stdafx.h"

#include "DrawElement.h"
#include "GraphicsInterface/GIImmediateCommands.h"
#include "Structure/Structure.h"
#include "Resource/ResourceLoader.h"
#include "Resource/RawBinary.h"
#include "Constant/RenderTag.h"
#include "Common.h"

bool D3D11DrawElement::Draw(GIImmediateCommands* cmd, const GIDrawMesh& element) {
    bool useIndexList = false;

    for (auto&& psResource : element.GetShaderResources()) {
        D3D11_BUFFER_DESC bufferDesc;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = psResource.first._resource.get();
        bufferDesc.ByteWidth = psResource.first._resource.size();
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        bufferDesc.StructureByteStride = psResource.first._structureByteStride;

        auto buffer = MakeRef(cmd->CreateBuffer(psResource.first._type, psResource.first._structureByteStride, (void*)psResource.first._resource.get(), psResource.first._resource.size()));

        switch (psResource.first._type) {
        case ResourceType::VertexList: {
            UINT hOffsets = 0;
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            cmd->IASetVertexBuffer(buffer.get(), psResource.first._structureByteStride, hOffsets);
            break;
        }
        case ResourceType::IndexList:
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            cmd->IASetIndexBuffer(buffer.get(), DXGI_FORMAT_R16_UINT);
            useIndexList = true;
            break;
        case ResourceType::VSConstantBuffer:
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cmd->VSSetConstantBuffers(psResource.second, buffer.get());
            break;
        case ResourceType::PSConstantBuffer:
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cmd->PSSetConstantBuffers(psResource.second, buffer.get());
            break;
        case ResourceType::GSConstantBuffer:
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            //cmd->GSSetConstantBuffers(psResource.second, buffer.get());
            break;
        }
    }

    cmd->IASetPrimitiveTopology(element.GetPrimitiveType());

    auto& drawLinks = element.GetDrawLinks();
    for (ShadingType type : { ShadingType::BasePass, ShadingType::Detph, ShadingType::Unlit } ) {
        auto range = drawLinks.equal_range(type);

        if (range.first == range.second) {
            continue;
        }
        
        for (auto it = range.first; it != range.second; it++) {
            auto& shader = it->second;
            auto vshader = MakeRef(cmd->CreateVertexShader(shader.VS()));
            cmd->VSSetShader(vshader.get());

            if (shader.PS().isValid()) {
                auto pshader = MakeRef(cmd->CreatePixelShader(shader.PS()));
                cmd->PSSetShader(pshader.get());
            }

            auto inputLayout = MakeRef(cmd->CreateInputLayout(element.GetVertexLayout(), vshader.get()));
            cmd->IASetInputLayout(inputLayout.get());

            TextureParam param;
            param.format = TextureFormat::RGBA8_UNORM;
            param.bindFlag = TextureBindTarget::SHADER_RESOURCE;

            for (auto&& texRes : shader.GetTextureResources()) {
                cmd->PSSetShaderResources(texRes.second, texRes.first.get());
                cmd->PSSetSamplers(texRes.second, texRes.first->GetSampler().get());
            }

            for (auto && rawRes : shader.GetRawResources()) {
                D3D11_BUFFER_DESC bufferDesc;
                D3D11_SUBRESOURCE_DATA subResource;
                subResource.pSysMem = rawRes.first._resource.get();
                bufferDesc.ByteWidth = rawRes.first._resource.size();
                bufferDesc.Usage = D3D11_USAGE_DEFAULT;
                bufferDesc.CPUAccessFlags = 0;
                bufferDesc.MiscFlags = 0;

                bufferDesc.StructureByteStride = rawRes.first._structureByteStride;
                bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                auto buffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, rawRes.first._structureByteStride, (void*)rawRes.first._resource.get(), rawRes.first._resource.size()));
                cmd->PSSetConstantBuffers(rawRes.second, buffer.get());
            }

            if (useIndexList) {
                cmd->DrawIndexed(shader.faceNumVerts, shader.startIndex, 0);
            }
            else {
                cmd->Draw(shader.faceNumVerts, shader.startIndex);
            }
        }
    }

    return true;
}
