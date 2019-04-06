
#include "DrawMesh.h"

#include "GraphicsInterface/GIImmediateCommands.h"

void DrawElement::Draw(GIImmediateCommands* cmd) {
    bool useIndexList = false;

    for (auto&& sharedResource : _parentMesh->GetShaderResources()) {
        auto& buffer = sharedResource.first;
        switch (buffer->type) {
        case ResourceType::VertexList: {
            UINT hOffsets = 0;
            cmd->IASetVertexBuffer(buffer.get(), buffer->structureByteStride, hOffsets);
            break;
        }
        case ResourceType::LightVertexList: {
            UINT hOffsets = 0;
            cmd->IASetVertexBuffer(buffer.get(), buffer->structureByteStride, hOffsets);
            break;
        }
        case ResourceType::IndexList:
            cmd->IASetIndexBuffer(buffer.get(), DXGI_FORMAT_R16_UINT);
            useIndexList = true;
            break;
        case ResourceType::VSConstantBuffer:
            cmd->VSSetConstantBuffers(sharedResource.second, buffer.get());
            break;
        case ResourceType::PSConstantBuffer:
            cmd->PSSetConstantBuffers(sharedResource.second, buffer.get());
            break;
        case ResourceType::GSConstantBuffer:
            //cmd->GSSetConstantBuffers(psResource.second, buffer.get());
            break;
        }
    }

    for (auto&& texture : _parentMesh->GetTextureResources()) {
        cmd->PSSetShaderResources(texture.second, texture.first->GetSubResourceView().get());
        cmd->PSSetSamplers(texture.second, texture.first->GetSampler().get());
    }

    cmd->IASetPrimitiveTopology(_parentMesh->GetPrimitiveType());

    auto vshader = MakeRef(cmd->CreateVertexShader(VS()));
    cmd->VSSetShader(vshader.get());

    for (auto && cbuffer : VS().constantBuffers) {
        cmd->VSSetConstantBuffers(cbuffer.first, cbuffer.second.get());
    }

    if (PS().isValid()) {
        auto pshader = MakeRef(cmd->CreatePixelShader(PS()));
        cmd->PSSetShader(pshader.get());

        for (auto&& texRes : PS().textureResources) {
            if (texRes.second->IsAvalable()) {
                cmd->PSSetShaderResources(texRes.first, texRes.second->GetSubResourceView().get());
                cmd->PSSetSamplers(texRes.first, texRes.second->GetSampler().get());
            }
        }

        for (auto && cbuffer : PS().constantBuffers) {
            cmd->PSSetConstantBuffers(cbuffer.first, cbuffer.second.get());
        }
    }

    if (GS().isValid()) {
        auto gshader = MakeRef(cmd->CreateGeometryShader(GS()));
        cmd->GSSetShader(gshader.get());
    }

    auto inputLayout = MakeRef(cmd->CreateInputLayout(_parentMesh->GetVertexLayout(), vshader.get()));
    cmd->IASetInputLayout(inputLayout.get());

    if (useIndexList) {
        cmd->DrawIndexed(_faceNumVerts, _startIndex, 0);
    }
    else {
        cmd->Draw(_faceNumVerts, _startIndex);
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
std::vector<VertexLayout> DrawMesh::GenerateVertexLayout<LineVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "COLOR", VertexProperty::FloatRGBA, 0, 0, 12 }, { "THICKNESS", VertexProperty::FloatR, 0, 0, 28 } };
}

template<>
std::vector<VertexLayout> DrawMesh::GenerateVertexLayout<MainVertex>() {
    std::vector<VertexLayout> layout;
    unsigned int offset = 0;
    layout.emplace_back("POSITION", VertexProperty::FloatRGB, 0, 0, static_cast<unsigned int>(offsetof(MainVertex, pos)));
    layout.emplace_back("COLOR", VertexProperty::FloatRGBA, 0, 0, static_cast<unsigned int>(offsetof(MainVertex, col)));
    layout.emplace_back("TEXCOORD", VertexProperty::FloatRG, 0, 0, static_cast<unsigned int>(offsetof(MainVertex, tex)));
    layout.emplace_back("NORMAL", VertexProperty::FloatRGB, 0, 0, static_cast<unsigned int>(offsetof(MainVertex, normal)));
    layout.emplace_back("TEXCOORD", VertexProperty::FloatRG, 1, 0, static_cast<unsigned int>(offsetof(MainVertex, lightUV)));
    return layout;
}

DrawMesh::DrawMesh(GIImmediateCommands* cmd, const StaticLightBuildData* staticLightBuildData) {
    typedef std::remove_cv_t<std::remove_reference_t<decltype(staticLightBuildData->lightVertices[0])>> VertType;
    _vertexLayout = GenerateVertexLayout<VertType>();

    BufferDesc bufferDesc;
    bufferDesc.byteWidth = static_cast<unsigned int>(sizeof(VertType) * staticLightBuildData->lightVertices.size());
    bufferDesc.usage = ResourceUsage::Default;
    bufferDesc.accessFlag = ResourceAccessFlag::None;
    bufferDesc.stride = sizeof(VertType);

    auto buffer = MakeRef(cmd->CreateBuffer(ResourceType::VertexList, bufferDesc, (void*)&staticLightBuildData->lightVertices[0]));
    buffer->type = ResourceType::LightVertexList;
    meshSharedResource.emplace_back(buffer, -1);

    _primitiveType = VertexPrimitiveType::TRIANGLELIST;
}

