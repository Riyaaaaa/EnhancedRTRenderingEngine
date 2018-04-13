#pragma once

#include <unordered_map>

#include "Constant/GraphicConstants.h"

#include "Scene/MeshObject.h"

#include "Material/Material.h"

#include "Resource/Texture2D.h"
#include "Resource/TextureCube.h"

#include "Shader/ShaderFactory.h"

#include "GraphicsInterface/GIRawResource.h"

class DrawElement
{
public:
    DrawElement(Shader ps, Shader vs, Shader gs = Shader(ShadingType::Geometry, RawBinary())) :
        _shadingType(ps.type),
        _pshader(ps),
        _vshader(vs),
        _gshader(gs)
    {}

    ShadingType GetShadingType() const {
        return _shadingType;
    }

    unsigned int faceNumVerts;
    unsigned int startIndex;

    Shader PS() const {
        return _pshader;
    }

    Shader VS() const {
        return _vshader;
    }

    Shader GS() const {
        return _gshader;
    }

protected:
    Shader _pshader;
    Shader _vshader;
    Shader _gshader;

    ShadingType _shadingType;
};

class DrawMesh {
public:
    DrawMesh() = default;
    template<class VertType>
    DrawMesh(GIImmediateCommands* cmd, MeshObject<VertType>* element) {
        _vertexLayout = GenerateVertexLayout<VertType>();

        {
            auto& vertexList = element->GetMesh()->GetVertexList();
            BufferDesc bufferDesc;
            bufferDesc.byteWidth = static_cast<unsigned int>(sizeof(VertType) * vertexList.size());
            bufferDesc.usage = ResourceUsage::Default;
            bufferDesc.accessFlag = ResourceAccessFlag::None;
            bufferDesc.stride = sizeof(VertType);

            auto buffer = MakeRef(cmd->CreateBuffer(ResourceType::VertexList, bufferDesc, (void*)&vertexList[0]));
            meshSharedResource.emplace_back(buffer, -1);
        }

        if (element->GetMesh()->HasIndexList()) {
            auto& indexList = element->GetMesh()->GetIndexList();
            BufferDesc bufferDesc;
            bufferDesc.byteWidth = static_cast<unsigned int>(sizeof(uint16_t) * indexList.size());
            bufferDesc.usage = ResourceUsage::Default;
            bufferDesc.accessFlag = ResourceAccessFlag::None;
            bufferDesc.stride = sizeof(uint16_t);

            auto buffer = MakeRef(cmd->CreateBuffer(ResourceType::IndexList, bufferDesc, (void*)&indexList[0]));
            meshSharedResource.emplace_back(buffer, -1);
        }

        _primitiveType = element->GetMesh()->GetPrimitiveType();
    }

    void RegisterConstantBuffer(const std::shared_ptr<GIBuffer>& buffer, unsigned int regsiterId, ShaderType shaderType) {
        ResourceType resType;

        switch (shaderType) {
        case ShaderType::PS:
            resType = ResourceType::PSConstantBuffer;
            break;
        case ShaderType::VS:
            resType = ResourceType::VSConstantBuffer;
            break;
        case ShaderType::GS:
            resType = ResourceType::GSConstantBuffer;
            break;
        }

        buffer->type = resType;
        meshSharedResource.emplace_back(buffer, regsiterId);
    }

    VertexPrimitiveType GetPrimitiveType() const { 
        return _primitiveType; 
    }

    const std::vector<VertexLayout>& GetVertexLayout() const {
        return _vertexLayout;
    }

    const std::vector<std::pair<std::shared_ptr<GIBuffer>, unsigned int>>& GetShaderResources() const {
        return meshSharedResource;
    }

    void ClearDrawElement() {
        _drawLinks.clear();
    }

    void AddDrawElement(const DrawElement& face) {
        _drawLinks.insert(std::make_pair(face.GetShadingType(), face));
    }

    const std::unordered_multimap<ShadingType, DrawElement>& GetDrawLinks() const {
        return _drawLinks;
    }

    void Draw(GIImmediateCommands* cmd);

private:
    template<class VertType>
    std::vector<VertexLayout> GenerateVertexLayout();

    VertexPrimitiveType _primitiveType;

    std::vector<VertexLayout> _vertexLayout;
    std::vector<std::pair<std::shared_ptr<GIBuffer>, unsigned int>> meshSharedResource;

    std::unordered_multimap<ShadingType, DrawElement> _drawLinks;
};



