#pragma once
#include <d3d11.h>
#include <vector>
#include <memory>

#include "D3DX11RenderView.h"
#include "D3D11Texture.h"
#include "Scene/MeshObject.h"
#include "Resource/ResourceHandle.h"
#include "RenderingContext.h"
#include "Constant/RenderTag.h"

template<class VertType>
class D3D11DrawPlate
{
public:
    D3D11DrawPlate() {}

    void Initialize(ComPtr<ID3D11Device> device, MeshObject<VertType>* mesh, TextureType type);

    virtual void Draw(const std::shared_ptr<D3DX11RenderView>& view);

    void SetTexture(const D3D11Texture& tex) {
        texture = tex;
    }

protected:
    virtual bool CreateBuffer(ComPtr<ID3D11Device> device, MeshObject<VertType>* element);

    virtual void SetBuffer(const std::shared_ptr<D3DX11RenderView>& view);
    virtual void SetShader(const std::shared_ptr<D3DX11RenderView>& view);

    MeshObject<VertType>* drawMesh;

    std::vector<D3D11_INPUT_ELEMENT_DESC> inElemDesc;
    D3D_PRIMITIVE_TOPOLOGY primitiveTopology;

    D3D11Texture texture;

    SIZE_T vertexCount;

    ResourceHandle<> vShader, pShader;

    ComPtr<ID3D11VertexShader> hpVertexShader;
    ComPtr<ID3D11PixelShader> hpPixelShader;
    ComPtr<ID3D11InputLayout> hpInputLayout;
    ComPtr<ID3D11Buffer> transformBuffer;
    ComPtr<ID3D11Buffer> materialBuffer;
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;
};