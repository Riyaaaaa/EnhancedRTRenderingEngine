#pragma once

#include <unordered_map>

#include "Constant/GICommandParameter.h"
#include "Constant/GraphicConstants.h"

#include "Scene/MeshObject.h"

#include "Mesh/StaticLightBuildData.h"
#include "Material/Material.h"

#include "Resource/Texture2D.h"
#include "Resource/TextureCube.h"

#include "Shader/ShaderFactory.h"

#include "Structure/Structure.h"

#include "GraphicsInterface/GIRawResource.h"
#include "GraphicsInterface/GIImmediateCommands.h"

class DrawMesh;

class DrawElement
{
public:
    DrawElement(DrawMesh* parent, unsigned int faceNumVerts, unsigned int startIndex) :
        _parentMesh(parent),
        _faceNumVerts(faceNumVerts),
        _startIndex(startIndex) {}

    void SetShaders(const Shader& ps, const Shader& vs, const Shader& gs = Shader()) {
        _pshader = ps;
        _vshader = vs;
        _gshader = gs;
    }

    ShadingType GetShadingType() const {
        return _shadingType;
    }

    Shader PS() const {
        return _pshader;
    }

    Shader VS() const {
        return _vshader;
    }

    Shader GS() const {
        return _gshader;
    }

    DrawMesh* ParentMesh() const {
        return _parentMesh;
    }

    void Draw(GIImmediateCommands* cmd);

protected:
    unsigned int _faceNumVerts;
    unsigned int _startIndex;

    Shader _pshader;
    Shader _vshader;
    Shader _gshader;

    ShadingType _shadingType;

    DrawMesh* _parentMesh;
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
            buffer->type = ResourceType::LightVertexList;
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

    DrawMesh(GIImmediateCommands* cmd, const StaticLightBuildData* staticLightBuildData);

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
        default:
            return;
        }

        buffer->type = resType;
        meshSharedResource.emplace_back(buffer, regsiterId);
    }

    void RegisterTexture(const GITextureProxy& texture, unsigned int registerId) {
        meshSharedTexture.emplace_back(texture, registerId);
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

    const std::vector<std::pair<GITextureProxy, unsigned int>>& GetTextureResources() const {
        return meshSharedTexture;
    }

private:
    template<class VertType>
    std::vector<VertexLayout> GenerateVertexLayout();

    VertexPrimitiveType _primitiveType;

    std::vector<VertexLayout> _vertexLayout;
    std::vector<std::pair<std::shared_ptr<GIBuffer>, unsigned int>> meshSharedResource;
    std::vector<std::pair<GITextureProxy, unsigned int>> meshSharedTexture;
};

