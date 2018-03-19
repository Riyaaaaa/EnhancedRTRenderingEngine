#pragma once

#include <unordered_map>

#include "Constant/GraphicConstants.h"

#include "Scene/MeshObject.h"

#include "Material/Material.h"

#include "Resource/Texture2D.h"
#include "Resource/TextureCube.h"

#include "Shader/ShaderFactory.h"

#include "GIRawResource.h"

class GIDrawFace
{
public:
    GIDrawFace(Shader ps, Shader vs) : 
        _shadingType(ps.type),
        _pshader(ps),
        _vshader(vs)
    {}

    GIDrawFace(const Material& material) :
        _shadingType(material.shadingType),
        _pshader(material.shadingType, material.pShader),
        _vshader(ShadingType::Vertex, material.vShader)
    {}

    template<class BufferType>
    void RegisterConstantBuffer(BufferType* ptr, unsigned int regsiterId) {
        RegisterShaderResource(GIRawResource(RawBinary(ptr, sizeof(BufferType)), ResourceType::ConstantBuffer, sizeof(float)), regsiterId);
    }

    void RegisterShaderResource(const Texture2D& tex, unsigned int regsiterId);
    void RegisterShaderResource(const TextureCube& tex, unsigned int regsiterId);

    ShadingType GetShadingType() const {
        return _shadingType;
    }

    const std::vector<std::pair<GITextureResource, unsigned int>>& GetTextureResources() const {
        return textureResources;
    }

    const std::vector<std::pair<GIRawResource, unsigned int>>& GetRawResources() const {
        return rawResources;
    }

    unsigned int faceNumVerts;
    unsigned int startIndex;

    Shader PS() const {
        return _pshader;
    }

    Shader VS() const {
        return _vshader;
    }

protected:
    void RegisterShaderResource(GIRawResource res, unsigned int registerId);

    // Shader Resources
    std::vector<std::pair<GITextureResource, unsigned int>> textureResources;
    std::vector<std::pair<GIRawResource, unsigned int>> rawResources;

    Shader _pshader;
    Shader _vshader;

    ShadingType _shadingType;
};

class GIDrawElement {
public:
    template<class VertType>
    GIDrawElement(MeshObject<VertType>* element) {
        _vertexLayout = GenerateVertexLayout<VertType>();
        auto& vertexList = element->GetMesh()->GetVertexList();
        shaderResources.emplace_back(GIRawResource(
            RawBinary((void*)&(vertexList[0]), sizeof(VertType) * vertexList.size()),
            ResourceType::VertexList, sizeof(VertType)), -1);

        if (element->GetMesh()->HasIndexList()) {
            auto& indexList = element->GetMesh()->GetIndexList();
            shaderResources.emplace_back(
                GIRawResource(RawBinary((void*)&(indexList[0]), sizeof(uint16_t) * indexList.size()),
                ResourceType::IndexList, sizeof(uint16_t)), -1);
        }
        
        ObjectBuffer* pBuffer = new ObjectBuffer;
        std::shared_ptr<void> buffer(pBuffer);

        pBuffer->World = XMMatrixTranspose(element->GetMatrix());
        pBuffer->NormalWorld = XMMatrixInverse(nullptr, element->GetMatrix());

        shaderResources.emplace_back(GIRawResource(RawBinary(pBuffer, sizeof(ObjectBuffer)),
            ResourceType::ConstantBuffer, -1), 1);

        _dataHandles.push_back(buffer);

        _primitiveType = VertexPrimitiveType::TRIANGLELIST;
    }

    VertexPrimitiveType GetPrimitiveType() const { 
        return _primitiveType; 
    }

    const std::vector<VertexLayout>& GetVertexLayout() const {
        return _vertexLayout;
    }

    const std::vector<std::pair<GIRawResource, unsigned int>>& GetShaderResources() const {
        return shaderResources;
    }

    void AddDrawFace(const GIDrawFace& face) {
        _drawLinks.insert(std::make_pair(face.GetShadingType(), face));
    }

    const std::unordered_multimap<ShadingType, GIDrawFace>& GetDrawLinks() const {
        return _drawLinks;
    }

private:
    template<class VertType>
    std::vector<VertexLayout> GenerateVertexLayout();

    VertexPrimitiveType _primitiveType;

    std::vector<VertexLayout> _vertexLayout;
    std::vector<std::shared_ptr<void>> _dataHandles;
    std::vector<std::pair<GIRawResource, unsigned int>> shaderResources;
    std::unordered_multimap<ShadingType, GIDrawFace> _drawLinks;
};



