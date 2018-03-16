#pragma once

#include <d3d11.h>
#include <vector>
#include <memory>

#include "D3DX11RenderView.h"
#include "D3D11TextureProxy.h"
#include "Scene/MeshObject.h"
#include "Resource/ResourceHandle.h"
#include "RenderingContext.h"
#include "Constant/RenderTag.h"
#include "GraphicsInterface/GIShader.h"

template<class VertType>
class D3D11DrawElement
{
public:
    D3D11DrawElement() {}

    void Initialize(ComPtr<ID3D11Device> device, MeshObject<VertType>* element, RenderTag::OpaqueRender);
    void Initialize(ComPtr<ID3D11Device> device, MeshObject<VertType>* element, RenderTag::DepthRender);

    virtual void Draw(const std::shared_ptr<D3DX11RenderView>& view);

    void SetTexture(const D3D11TextureProxy& tex, std::size_t index) {
        textures[index] = tex;
    }

    bool _Draw(const std::shared_ptr<D3DX11RenderView>& view, const GIDrawElement& element);

protected:
    virtual bool CreateBuffer(ComPtr<ID3D11Device> device, MeshObject<VertType>* element);
    
    virtual void SetBuffer(const std::shared_ptr<D3DX11RenderView>& view);
    virtual void SetShader(const std::shared_ptr<D3DX11RenderView>& view, int drawIndex);

    std::vector<D3D11_INPUT_ELEMENT_DESC> inElemDesc;
    D3D_PRIMITIVE_TOPOLOGY primitiveTopology;

    std::vector<D3D11TextureProxy> textures;

    // TODO: Support smart pointer
    MeshObject<VertType>* drawMesh;

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

