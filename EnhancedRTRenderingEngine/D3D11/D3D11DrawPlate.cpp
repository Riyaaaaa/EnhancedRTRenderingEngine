#include "stdafx.h"
#include "D3D11DrawPlate.h"

#include "D3D11TextureProxy.h"
#include "D3D11FormatUtils.h"

#include "Structure/Structure.h"
#include "Resource/ResourceLoader.h"
#include "Constant/RenderTag.h"
#include "Common.h"

template<class VertType>
void D3D11DrawPlate<VertType>::Initialize(ComPtr<ID3D11Device> device, MeshObject<VertType>*  element, TextureType type, std::string psshader, int index) {
    inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 });

    primitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    vShader = ResourceLoader::LoadShader("HUDVertexShader");

    float _index;
    if (type == TextureType::Texture2D) {
        pShader = ResourceLoader::LoadShader(psshader);
        _index = 0.0f;
    }
    _type = type;

    if (!CreateBuffer(device, element, _index)) {
        return;
    }

    drawMesh = element;
}

template<class VertType>
bool D3D11DrawPlate<VertType>::CreateBuffer(ComPtr<ID3D11Device> device, MeshObject<VertType>* element, float index) {
    D3D11_BUFFER_DESC bufferDesc;
    D3D11_SUBRESOURCE_DATA subResource;

    subResource.pSysMem = &(element->GetMesh()->GetVertexList()[0]);
    vertexCount = element->GetMesh()->GetVertexList().size();
    bufferDesc.ByteWidth = sizeof(VertType) * vertexCount;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);

    if (FAILED(device->CreateBuffer(&bufferDesc, &subResource, vertexBuffer.ToCreator()))) {
        return false;
    }

    D3D11_SUBRESOURCE_DATA constantSubResource;
    auto mat = XMMatrixTranspose(element->GetMatrix());
    constantSubResource.pSysMem = &mat;

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);

    bufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
    if (FAILED(device->CreateBuffer(&bufferDesc, &constantSubResource, transformBuffer.ToCreator()))) {
        return false;
    }

    bufferDesc.ByteWidth = sizeof(AlignedBuffer<float>);
    AlignedBuffer<float> buf;
    buf.value = index;
    constantSubResource.pSysMem = &buf;
    if (FAILED(device->CreateBuffer(&bufferDesc, &constantSubResource, pBuffer.ToCreator()))) {
        return false;
    }

    if (element->GetMesh()->HasIndexList()) {
        vertexCount = element->GetMesh()->GetIndexList().size();
        bufferDesc.ByteWidth = sizeof(element->GetMesh()->GetIndexList()[0]) * vertexCount;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = sizeof(unsigned short);

        subResource.pSysMem = &element->GetMesh()->GetIndexList()[0];
        subResource.SysMemPitch = 0;
        subResource.SysMemSlicePitch = 0;

        if (FAILED(device->CreateBuffer(&bufferDesc, &subResource, indexBuffer.ToCreator()))) {
            return false;
        }
    }

    return true;
}

template<class VertType>
void D3D11DrawPlate<VertType>::SetShader(const std::shared_ptr<D3DX11RenderView>& view) {
    auto err = view->hpDevice->CreateInputLayout(&inElemDesc[0], inElemDesc.size(), vShader.get(), vShader.size(), hpInputLayout.ToCreator());
    if (FAILED(err)) {
        return;
    }

    view->hpDeviceContext->IASetInputLayout(hpInputLayout.Get());

    if (FAILED(view->hpDevice->CreateVertexShader(vShader.get(), vShader.size(), NULL, hpVertexShader.ToCreator()))) {
        return;
    }
    view->hpDeviceContext->VSSetShader(hpVertexShader.Get(), NULL, 0);

    if (FAILED(view->hpDevice->CreatePixelShader(pShader.get(), pShader.size(), NULL, hpPixelShader.ToCreator()))) {
        return;
    }
    view->hpDeviceContext->PSSetShader(hpPixelShader.Get(), NULL, 0);

    view->hpDeviceContext->PSSetShaderResources(0, 1, texture->GetSubResourceView().Ref());

    if (_type == TextureType::TextureCube) {
        view->hpDeviceContext->PSSetConstantBuffers(0, 0, pBuffer.Ref());
    }

    view->hpDeviceContext->PSSetSamplers(0, 1, texture->GetSampler().Ref());
}

template<class VertType>
void D3D11DrawPlate<VertType>::SetBuffer(const std::shared_ptr<D3DX11RenderView>& view) {
    UINT hStrides = sizeof(VertType);
    UINT hOffsets = 0;

    view->hpDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.Ref(), &hStrides, &hOffsets);
    view->hpDeviceContext->VSSetConstantBuffers(1, 1, transformBuffer.Ref());
    view->hpDeviceContext->IASetPrimitiveTopology(primitiveTopology);

    if (indexBuffer.Get()) {
        view->hpDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    }
}

template<class VertType>
void D3D11DrawPlate<VertType>::Draw(const std::shared_ptr<D3DX11RenderView>& view) {
    this->SetBuffer(view);
    this->SetShader(view);

    if (indexBuffer.Get()) {
        view->hpDeviceContext->DrawIndexed(drawMesh->GetMesh()->GetDrawFacesMap()[0].faceNumVerts, 0, 0);
    }
    else {
        view->hpDeviceContext->Draw(drawMesh->GetMesh()->GetDrawFacesMap()[0].faceNumVerts, 0);
    }
}

template D3D11DrawPlate<Vertex3D>;
template D3D11DrawPlate<SimpleVertex>;
template D3D11DrawPlate<PMDVertex>;
template D3D11DrawPlate<TexVertex>;

