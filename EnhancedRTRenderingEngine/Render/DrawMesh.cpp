#include "stdafx.h"
#include "DrawMesh.h"

#include "GraphicsInterface/GIImmediateCommands.h"

void DrawElement::RegisterShaderResource(const GITextureProxy& tex, unsigned int registerId) {
    textureResources.push_back(std::make_pair(tex, registerId));
}

void DrawElement::RegisterShaderResource(GIRawResource res, unsigned int registerId) {
    rawResources.push_back(std::make_pair(res, registerId));
}

void DrawMesh::Draw(GIImmediateCommands* cmd) {
    bool useIndexList = false;

    for (auto&& psResource : GetShaderResources()) {
        BufferDesc bufferDesc;

        bufferDesc.byteWidth = psResource.first._resource.size();
        bufferDesc.usage = ResourceUsage::Default;
        bufferDesc.accessFlag = ResourceAccessFlag::None;
        bufferDesc.stride = psResource.first._structureByteStride;

        auto buffer = MakeRef(cmd->CreateBuffer(psResource.first._type, bufferDesc, (void*)psResource.first._resource.get()));

        switch (psResource.first._type) {
        case ResourceType::VertexList: {
            UINT hOffsets = 0;
            cmd->IASetVertexBuffer(buffer.get(), psResource.first._structureByteStride, hOffsets);
            break;
        }
        case ResourceType::IndexList:
            cmd->IASetIndexBuffer(buffer.get(), DXGI_FORMAT_R16_UINT);
            useIndexList = true;
            break;
        case ResourceType::VSConstantBuffer:
            cmd->VSSetConstantBuffers(psResource.second, buffer.get());
            break;
        case ResourceType::PSConstantBuffer:
            cmd->PSSetConstantBuffers(psResource.second, buffer.get());
            break;
        case ResourceType::GSConstantBuffer:
            //cmd->GSSetConstantBuffers(psResource.second, buffer.get());
            break;
        }
    }

    cmd->IASetPrimitiveTopology(GetPrimitiveType());

    auto& drawLinks = GetDrawLinks();
    for (ShadingType type : { ShadingType::BasePass, ShadingType::Detph, ShadingType::Unlit }) {
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

            if (shader.GS().isValid()) {
                auto gshader = MakeRef(cmd->CreateGeometryShader(shader.GS()));
                cmd->GSSetShader(gshader.get());
            }

            auto inputLayout = MakeRef(cmd->CreateInputLayout(GetVertexLayout(), vshader.get()));
            cmd->IASetInputLayout(inputLayout.get());

            TextureParam param;
            param.format = TextureFormat::RGBA8_UNORM;
            param.bindFlag = TextureBindTarget::SHADER_RESOURCE;

            for (auto&& texRes : shader.GetTextureResources()) {
                if (texRes.first->IsAvalable()) {
                    cmd->PSSetShaderResources(texRes.second, texRes.first->GetSubResourceView().get());
                    cmd->PSSetSamplers(texRes.second, texRes.first->GetSampler().get());
                }
            }

            for (auto && rawRes : shader.GetRawResources()) {
                BufferDesc bufferDesc;
                bufferDesc.byteWidth = rawRes.first._resource.size();
                bufferDesc.usage = ResourceUsage::Default;
                bufferDesc.accessFlag = ResourceAccessFlag::None;
                bufferDesc.stride = rawRes.first._structureByteStride;

                auto buffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, bufferDesc, (void*)rawRes.first._resource.get()));
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
}

template<>
std::vector<VertexLayout> DrawMesh::GenerateVertexLayout<Vertex3D>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "COLOR", VertexProperty::FloatRGBA, 0, 0, 12 }, { "TEXCOORD", VertexProperty::FloatRG,0,0,28 } };
}

template<>
std::vector<VertexLayout> DrawMesh::GenerateVertexLayout<SimpleVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "COLOR", VertexProperty::FloatRGBA, 0, 0, 12 } };
}

template<>
std::vector<VertexLayout> DrawMesh::GenerateVertexLayout<TexVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0 }, { "TEXCOORD", VertexProperty::FloatRG, 0, 0, 12 } };
}

template<>
std::vector<VertexLayout> DrawMesh::GenerateVertexLayout<PMDVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "TEXCOORD", VertexProperty::FloatRG, 0, 0, 12 }, { "NORMAL", VertexProperty::FloatRGB, 0,0,20 }, { "COLOR", VertexProperty::FloatRGBA,0,0,32 } };
}

template<>
std::vector<VertexLayout> DrawMesh::GenerateVertexLayout<LineVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "COLOR", VertexProperty::FloatRGBA, 0, 0, 12 }, { "THICKNESS", VertexProperty::FloatR, 0, 0, 28 } };
}

