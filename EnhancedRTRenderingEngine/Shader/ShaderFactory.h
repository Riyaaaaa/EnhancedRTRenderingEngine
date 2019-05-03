#pragma once

#include <unordered_map>

#include "Resource/RawBinary.h"
#include "Resource/ResourceLoader.h"

#include "Constant/GraphicConstants.h"

#include "GraphicsInterface/GITextureProxy.h"

struct Shader : public RawBinary {
    Shader() = default;
    Shader(ShadingType t, RawBinary data) : 
        RawBinary(data),
        type(t){}

    ShadingType type = ShadingType::None;

    std::unordered_map<unsigned int, GITextureProxy> textureResources;
    std::unordered_map<unsigned int, std::shared_ptr<GIBuffer>> constantBuffers;
};

namespace ShaderFactory
{
    inline Shader DepthOnlyVertexShader() {
        return Shader(ShadingType::Vertex, ResourceLoader::LoadShader("DepthVertexShader"));
    }

    inline Shader RenderShadowMapShader() {
        return Shader(ShadingType::Detph, ResourceLoader::LoadShader("RenderShadowMapShader"));
    }

    inline Shader TextureVertexShader() {
        return Shader(ShadingType::Vertex, ResourceLoader::LoadShader("TextureVertexShader"));
    }

    inline Shader MinTextureColor() {
        return Shader(ShadingType::Unlit, ResourceLoader::LoadShader("MinTextureColor"));
    }

    inline Shader MinVertexColor() {
        return Shader(ShadingType::Vertex, ResourceLoader::LoadShader("VertexShader"));
    }

    inline Shader MinPixelShader() {
        return Shader(ShadingType::Unlit, ResourceLoader::LoadShader("MinPixelShader"));
    }

    inline Shader MinPixelColor() {
        return Shader(ShadingType::Unlit, ResourceLoader::LoadShader("PixelShader"));
    }

    inline Shader LineVertexShader() {
        return Shader(ShadingType::Vertex, ResourceLoader::LoadShader("LineVertexShader"));
    }

    inline Shader LineGeometryShader() {
        return Shader(ShadingType::Geometry, ResourceLoader::LoadShader("LineGeometryShader"));
    }

    inline Shader HUDVertexShader() {
        return Shader(ShadingType::Vertex, ResourceLoader::LoadShader("HUDVertexShader"));
    }

    inline Shader HUDPixelShader() {
        return Shader(ShadingType::Unlit, ResourceLoader::LoadShader("HUDPixelShader"));
    }
};

