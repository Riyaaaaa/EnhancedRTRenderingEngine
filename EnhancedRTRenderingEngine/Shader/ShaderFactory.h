#pragma once

#include "Resource/RawBinary.h"
#include "Resource/ResourceLoader.h"

#include "Constant/GraphicConstants.h"

struct Shader : public RawBinary {
    Shader() = default;
    Shader(ShadingType t, RawBinary data) : 
        RawBinary(data),
        type(t){}

    ShadingType type = ShadingType::None;
};

namespace ShaderFactory
{
    inline Shader DepthOnlyVertexShader() {
        return Shader(ShadingType::Vertex, ResourceLoader::LoadShader("DepthVertexShader"));
    }

    inline Shader RenderShadowMapShader() {
        return Shader(ShadingType::Detph, ResourceLoader::LoadShader("RenderShadowMapShader"));
    }

    inline Shader HUDVertexShader() {
        return Shader(ShadingType::Vertex, ResourceLoader::LoadShader("HUDVertexShader"));
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
        return Shader(ShadingType::Unlit, ResourceLoader::LoadShader("LineVertexShader"));
    }

    inline Shader LineGeometryShader() {
        return Shader(ShadingType::Geometry, ResourceLoader::LoadShader("LineGeometryShader"));
    }
};

