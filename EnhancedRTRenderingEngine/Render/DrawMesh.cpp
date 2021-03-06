
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

DrawMesh::DrawMesh(GIImmediateCommands* cmd) {
    BufferDesc desc;
    desc.usage = ResourceUsage::Dynamic;
    desc.byteWidth = sizeof(ObjectBuffer);
    desc.stride = sizeof(float);
    desc.accessFlag = ResourceAccessFlag::W;
    auto objectBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, nullptr));
    RegisterConstantBuffer(objectBuffer, VSRegisterSlots::BasePassObjectBuffer, ShaderType::VS);
}

void DrawMesh::ExtractDrawElements(GIImmediateCommands* cmd, 
    const std::vector<ElementDesc>& elementDescs, 
    const std::vector<Material>& materials,
    const DrawPlan& plan) {
    int index = 0;
    for (auto&& drawface : elementDescs) {
        auto& material = materials[drawface.materialIdx];
        Shader ps(material.shadingType, material.pShader);

        switch (material.shadingType) {
        case ShadingType::BasePass: {
            TextureParam param;
            param.type = material.type;
            GITextureProxy texture = GITextureProxyEntity::Create();
            if (material.type == TextureType::Texture2D) {
                param.arraySize = 1;
                texture->Initialize(cmd, param, material.texture);
            }
            else if (material.type == TextureType::TextureCube) {
                param.arraySize = 6;
                texture->Initialize(cmd, param, material.cubeTexture.textures);
            }

            MaterialBuffer mbuf;
            mbuf.baseColor = material.baseColor;
            mbuf.metallic = material.metallic;
            mbuf.roughness = material.roughness;
            if (plan.useLightMap) {
                mbuf.useLightMap = 1.0f;
            }
            if (plan.useEnviromentMap) {
                mbuf.useEnviromentMap = 1.0f;
            }
            BufferDesc desc;
            desc.byteWidth = sizeof(mbuf);
            desc.stride = sizeof(float);
            desc.usage = ResourceUsage::Dynamic;
            desc.accessFlag = ResourceAccessFlag::W;
            auto materialBuffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, desc, &mbuf));
            ps.constantBuffers[BasePassMaterialBuffer] = materialBuffer;
            ps.textureResources[BasePassMainTexture] = texture;
        }
                                    break;

        case ShadingType::Detph:
            break;
        case ShadingType::Unlit: {
            TextureParam param;
            param.type = material.type;
            GITextureProxy texture = GITextureProxyEntity::Create();
            if (material.type == TextureType::Texture2D) {
                param.arraySize = 1;
                texture->Initialize(cmd, param, material.texture);
            }
            else if (material.type == TextureType::TextureCube) {
                param.arraySize = 6;
                texture->Initialize(cmd, param, material.cubeTexture.textures);
            }
            ps.textureResources[UnlitMainTexture] = texture;
        }
            break;
        default:
            break;
        }

        DrawElement face(this, drawface.faceNumVerts, index);
        face.SetShaders(ps, Shader(ShadingType::Vertex, material.vShader), Shader(ShadingType::Geometry, material.gShader));
        _elements.push_back(face);

        index += drawface.faceNumVerts;
    }
}

void DrawMesh::UpdateObjectBuffer(GIImmediateCommands* cmd, const DirectX::XMMATRIX& matrix)
{
    auto buffer = FindSharedResource(ResourceType::VSConstantBuffer, VSRegisterSlots::BasePassObjectBuffer);
    auto mapped = cmd->MapBuffer(buffer.get(), 0, MapType::WRITE_DISCARD);

    ObjectBuffer data;
    data.World = DirectX::XMMatrixTranspose(matrix);
    data.NormalWorld = DirectX::XMMatrixInverse(nullptr, matrix);
    memcpy(mapped.pData, &data, sizeof(data));

    cmd->UnmapBuffer(buffer.get(), 0);
}

void DrawMesh::Draw(GIImmediateCommands* cmd)
{
    for (auto&& element : _elements) {
        element.Draw(cmd);
    }
}
