#include "stdafx.h"
#include "D3D11DrawElement.h"

#include "D3D11Texture.h"
#include "D3D11FormatUtils.h"

#include "Structure/Structure.h"
#include "Resource/ResourceLoader.h"
#include "Constant/RenderTag.h"
#include "Common.h"

template<class VertType>
void D3D11DrawElement<VertType>::Initialize(ComPtr<ID3D11Device> device, MeshObject<VertType>* element, RenderTag::OpaqueRender) {
    auto& context = element->GetContext();
    _state = RenderingState::NONE;

    UINT memoryOffset = 0;
    for (auto&& layout : context.layouts) {
        inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ layout.name, 0, CastToD3D11Formart<DXGI_FORMAT>(layout.vProperty), 0, memoryOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        memoryOffset += GetMemoryBlockSize(layout.vProperty);
    }

    primitiveTopology = CastToD3D11Formart<D3D_PRIMITIVE_TOPOLOGY>(context.pType);

    if (!CreateBuffer(device, element)) {
        _state = RenderingState::FAILED;
        return;
    }

    drawMesh = element;

    textures.resize(drawMesh->GetMesh()->GetDrawTargetNum());
    for (int i = 0; i < drawMesh->GetMesh()->GetDrawTargetNum(); i++) {
        auto& material = drawMesh->GetMaterials()[i];
        if (material.texture().isValid()) {
            textures[i].Initialize(device, material.texture);
        }
    }

    _state = RenderingState::RENDER_READIED;
}


template<class VertType>
void D3D11DrawElement<VertType>::Initialize(ComPtr<ID3D11Device> device, MeshObject<VertType>* element, RenderTag::DepthRender) {
    auto& context = element->GetContext();
    _state = RenderingState::NONE;

    UINT memoryOffset = 0;
    for (auto&& layout : context.layouts) {
        inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ layout.name, 0, CastToD3D11Formart<DXGI_FORMAT>(layout.vProperty), 0, memoryOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        memoryOffset += GetMemoryBlockSize(layout.vProperty);
    }

    primitiveTopology = CastToD3D11Formart<D3D_PRIMITIVE_TOPOLOGY>(context.pType);

    if (!CreateBuffer(device, element)) {
        _state = RenderingState::FAILED;
        return;
    }

    _state = RenderingState::WRITE_DEPTH;
}

template<class VertType>
bool D3D11DrawElement<VertType>::CreateBuffer(ComPtr<ID3D11Device> device, MeshObject<VertType>* element) {
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
    //D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);
    if (FAILED(device->CreateBuffer(&bufferDesc, &constantSubResource, transformBuffer.ToCreator()))) {
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
void D3D11DrawElement<VertType>::SetShader(const std::shared_ptr<D3DX11RenderView>& view, int drawIndex) {
   

    ResourceHandle<> vShader, pShader;
    
    if (_state == RenderingState::WRITE_DEPTH){
        vShader = ResourceLoader::LoadShader("DepthVertexShader");
    }
    else {
        auto material = drawMesh->GetMaterials()[drawIndex];
        vShader = material.vShader;
        pShader = material.pShader;
    }
    
    auto err = view->hpDevice->CreateInputLayout(&inElemDesc[0], inElemDesc.size(), vShader().get(), vShader().size(), hpInputLayout.ToCreator());
    if (FAILED(err)) {
        return;
    }

    view->hpDeviceContext->IASetInputLayout(hpInputLayout.Get());

    if (FAILED(view->hpDevice->CreateVertexShader(vShader().get(), vShader().size(), NULL, hpVertexShader.ToCreator()))) {
        return;
    }
    view->hpDeviceContext->VSSetShader(hpVertexShader.Get(), NULL, 0);

    if (pShader.HasResource()) {
        if (FAILED(view->hpDevice->CreatePixelShader(pShader().get(), pShader().size(), NULL, hpPixelShader.ToCreator()))) {
            return;
        }
        view->hpDeviceContext->PSSetShader(hpPixelShader.Get(), NULL, 0);
    }
    else {
        view->hpDeviceContext->PSSetShader(nullptr, NULL, 0);
    }

    if (textures[drawIndex].IsAvalable()) {
        view->hpDeviceContext->PSSetShaderResources(0, 1, textures[drawIndex].GetSubResourceView().Ref());
    }
    view->hpDeviceContext->PSSetSamplers(0, 1, textures[drawIndex].GetSampler().Ref());
}

template<class VertType>
void D3D11DrawElement<VertType>::SetBuffer(const std::shared_ptr<D3DX11RenderView>& view) {
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
void D3D11DrawElement<VertType>::Draw(const std::shared_ptr<D3DX11RenderView>& view) {
    this->SetBuffer(view);

    for (int i = 0; i < drawMesh->GetMesh()->GetDrawTargetNum(); i++) {
        int index = 0;
        this->SetShader(view, i);
        if (indexBuffer.Get()) {
            view->hpDeviceContext->DrawIndexed(drawMesh->GetMesh()->GetDrawTargetIndexes()[i], index, 0);
        }
        else {
            view->hpDeviceContext->Draw(drawMesh->GetMesh()->GetDrawTargetIndexes()[i], index);
        }

        index += drawMesh->GetMesh()->GetDrawTargetIndexes()[i];
        // DEBUG:
        // Check index < vertexCount;
    }
}

template D3D11DrawElement<Vertex3D>;
template D3D11DrawElement<SimpleVertex>;
template D3D11DrawElement<PMDVertex>;
