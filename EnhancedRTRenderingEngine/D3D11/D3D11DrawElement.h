#pragma once

#include <d3d11.h>
#include <vector>
#include <memory>

#include "D3D11RenderView.h"
#include "D3D11TextureProxy.h"
#include "Scene/MeshObject.h"
#include "Resource/ResourceHandle.h"
#include "RenderingContext.h"
#include "Constant/RenderTag.h"
#include "GraphicsInterface/GIDrawMesh.h"

class D3D11DrawElement
{
public:
    D3D11DrawElement() {}

    bool Draw(const std::shared_ptr<D3D11RenderView>& view, const GIDrawMesh& element);

protected:

    std::vector<D3D11_INPUT_ELEMENT_DESC> inElemDesc;
    D3D_PRIMITIVE_TOPOLOGY primitiveTopology;

    std::vector<D3D11TextureProxy> textures;

    SIZE_T vertexCount;

    ComPtr<ID3D11VertexShader> hpVertexShader;
    ComPtr<ID3D11PixelShader> hpPixelShader;
    ComPtr<ID3D11InputLayout> hpInputLayout;
    ComPtr<ID3D11Buffer> transformBuffer;
    ComPtr<ID3D11Buffer> materialBuffer;
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;

    RenderingState _state;
};

