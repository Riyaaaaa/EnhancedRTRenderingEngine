#include "stdafx.h"
#include "D3D11DrawElement.h"

#include "D3D11TextureProxy.h"
#include "D3D11FormatUtils.h"

#include "Structure/Structure.h"
#include "Resource/ResourceLoader.h"
#include "Resource/RawBinary.h"
#include "Constant/RenderTag.h"
#include "Common.h"

template<class VertType>
void D3D11DrawElement<VertType>::Initialize(ComPtr<ID3D11Device> device, MeshObject<VertType>* element, RenderTag::OpaqueRender) {
    auto& context = element->GetContext();
    _state = RenderingState::NONE;

    UINT memoryOffset = 0;
    for (auto&& layout : context.layouts) {
        inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ layout.name, 0, CastToD3D11Format<DXGI_FORMAT>(layout.vProperty), 0, memoryOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        memoryOffset += GetMemoryBlockSize(layout.vProperty);
    }

    primitiveTopology = CastToD3D11Format<D3D_PRIMITIVE_TOPOLOGY>(context.pType);

    if (!CreateBuffer(device, element)) {
        _state = RenderingState::FAILED;
        return;
    }

    drawMesh = element;

    TextureParam param;
    param.format = TextureFormat::RGBA8_UNORM;
    param.bindFlag = TextureBindTarget::SHADER_RESOURCE;

    textures.resize(drawMesh->GetMesh()->GetMaterialNum(), D3D11TextureProxy(device));
    for (int i = 0; i < drawMesh->GetMesh()->GetMaterialNum(); i++) {
        auto& material = drawMesh->GetMaterials()[i];
        param.type = material.type;
        if (material.type == TextureType::Texture2D) {
            textures[i].Initialize(param, material.texture);
        }
        else if (material.type == TextureType::TextureCube) {
            textures[i].Initialize(param, material.cubeTexture);
        }
    }

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);
    bufferDesc.ByteWidth = sizeof(MaterialBuffer);
    if(FAILED(device->CreateBuffer(&bufferDesc, nullptr, materialBuffer.ToCreator()))) {
        _state = RenderingState::FAILED;
        return;
    }

    _state = RenderingState::RENDER_READIED;
}


template<class VertType>
void D3D11DrawElement<VertType>::Initialize(ComPtr<ID3D11Device> device, MeshObject<VertType>* element, RenderTag::DepthRender) {
    auto& context = element->GetContext();
    _state = RenderingState::NONE;

    UINT memoryOffset = 0;
    for (auto&& layout : context.layouts) {
        inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ layout.name, 0, CastToD3D11Format<DXGI_FORMAT>(layout.vProperty), 0, memoryOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        memoryOffset += GetMemoryBlockSize(layout.vProperty);
    }

    primitiveTopology = CastToD3D11Format<D3D_PRIMITIVE_TOPOLOGY>(context.pType);

    if (!CreateBuffer(device, element)) {
        _state = RenderingState::FAILED;
        return;
    }

    drawMesh = element;

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
    ObjectBuffer buffer;
    buffer.World = XMMatrixTranspose(element->GetMatrix());
    buffer.NormalWorld = XMMatrixInverse(nullptr, element->GetMatrix());
    constantSubResource.pSysMem = &buffer;

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);

    bufferDesc.ByteWidth = sizeof(ObjectBuffer);
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
    RawBinary vShader, pShader;
    MaterialBuffer materialParams;

    if (_state == RenderingState::WRITE_DEPTH){
        vShader = ResourceLoader::LoadShader("DepthVertexShader");
        pShader = ResourceLoader::LoadShader("RenderShadowMapShader");
    }
    else {
        auto& material = drawMesh->GetMaterials()[drawIndex];
        vShader = material.vShader;
        pShader = material.pShader;
        materialParams.metallic = material.metallic;
        materialParams.roughness = material.roughness;
    }
    
    auto err = view->hpDevice->CreateInputLayout(&inElemDesc[0], inElemDesc.size(), vShader.get(), vShader.size(), hpInputLayout.ToCreator());
    if (FAILED(err)) {
        return;
    }

    view->hpDeviceContext->IASetInputLayout(hpInputLayout.Get());

    if (FAILED(view->hpDevice->CreateVertexShader(vShader.get(), vShader.size(), NULL, hpVertexShader.ToCreator()))) {
        return;
    }
    view->hpDeviceContext->VSSetShader(hpVertexShader.Get(), NULL, 0);

    if (pShader.isValid()) {
        if (FAILED(view->hpDevice->CreatePixelShader(pShader.get(), pShader.size(), NULL, hpPixelShader.ToCreator()))) {
            return;
        }
        view->hpDeviceContext->PSSetShader(hpPixelShader.Get(), NULL, 0);

        if (_state == RenderingState::WRITE_HUD) {
            if (textures[drawIndex].IsAvalable()) {
                view->hpDeviceContext->PSSetShaderResources(0, 1, textures[drawIndex].GetSubResourceView().Ref());
                view->hpDeviceContext->PSSetSamplers(0, 1, textures[drawIndex].GetSampler().Ref());
            }
        }
        else if (_state == RenderingState::RENDER_READIED){
            view->hpDeviceContext->UpdateSubresource(materialBuffer.Get(), 0, NULL, &materialParams, 0, 0);
            view->hpDeviceContext->PSSetConstantBuffers(1, 1, materialBuffer.Ref());

            if (textures[drawIndex].IsAvalable()) {
                view->hpDeviceContext->PSSetShaderResources(10, 1, textures[drawIndex].GetSubResourceView().Ref());
                view->hpDeviceContext->PSSetSamplers(10, 1, textures[drawIndex].GetSampler().Ref());
            }
        }
    }
    else {
        view->hpDeviceContext->PSSetShader(nullptr, NULL, 0);
    }
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
    int index = 0, matIdx = -1;
    for (std::size_t i = 0; i < drawMesh->GetMesh()->GetDrawTargetNum(); i++) {
        if (matIdx != drawMesh->GetMesh()->GetDrawFacesMap()[i].materialIdx) {
            matIdx = drawMesh->GetMesh()->GetDrawFacesMap()[i].materialIdx;
            this->SetShader(view, matIdx);
        }
        
        if (indexBuffer.Get()) {
            view->hpDeviceContext->DrawIndexed(drawMesh->GetMesh()->GetDrawFacesMap()[i].faceNumVerts, index, 0);
        }
        else {
            view->hpDeviceContext->Draw(drawMesh->GetMesh()->GetDrawFacesMap()[i].faceNumVerts, index);
        }

        index += drawMesh->GetMesh()->GetDrawFacesMap()[i].faceNumVerts;
        // DEBUG:
        // Check index < vertexCount;
    }
}

template<class VertType>
bool D3D11DrawElement<VertType>::_Draw(const std::shared_ptr<D3DX11RenderView>& view, const GIDrawElement& element) {
    auto& layouts = element.GetVertexLayout();

    for (auto&& layout : layouts) {
        inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ 
            layout.name, layout.semanticsIndex,
            CastToD3D11Format<DXGI_FORMAT>(layout.vProperty), 
            layout.slot, 
            layout.memoryOffset, 
            D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }

    primitiveTopology = CastToD3D11Format<D3D_PRIMITIVE_TOPOLOGY>(element.GetPrimitiveType());

    std::unordered_map<unsigned int, ComPtr<ID3D11Buffer>> _bufferMap;

    for (auto&& psResource : element.GetShaderResources()) {
        D3D11_BUFFER_DESC bufferDesc;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = psResource.first._resource.get();
        bufferDesc.ByteWidth = psResource.first._resource.size();
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        bufferDesc.StructureByteStride = psResource.first._structureByteStride;

        switch (psResource.first._type) {
        case ResourceType::VertexList: {
            UINT hOffsets = 0;
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            if (FAILED(view->hpDevice->CreateBuffer(&bufferDesc, &subResource, vertexBuffer.ToCreator()))) {
                return false;
            }
            view->hpDeviceContext->IASetVertexBuffers(0, 1, vertexBuffer.Ref(), &psResource.first._structureByteStride, &hOffsets);
            break;
        }
        case ResourceType::IndexList:
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            if (FAILED(view->hpDevice->CreateBuffer(&bufferDesc, &subResource, indexBuffer.ToCreator()))) {
                return false;
            }
            view->hpDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
            break;
        case ResourceType::ConstantBuffer:
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            ComPtr<ID3D11Buffer> buffer;
            if (FAILED(view->hpDevice->CreateBuffer(&bufferDesc, &subResource, buffer.ToCreator()))) {
                return false;
            }
            _bufferMap.insert(std::make_pair(psResource.second, buffer));
            break;
        }
    }

    for (auto&& sharedRes : _bufferMap) {
        view->hpDeviceContext->VSSetConstantBuffers(sharedRes.first, 1, sharedRes.second.Ref());
    }
    view->hpDeviceContext->IASetPrimitiveTopology(primitiveTopology);

    auto& drawLinks = element.GetDrawLinks();
    for (ShadingType type : { ShadingType::BasePass, ShadingType::Detph, ShadingType::Unlit } ) {
        auto range = drawLinks.equal_range(type);

        if (range.first == range.second) {
            continue;
        }
        
        for (auto it = range.first; it != range.second; it++) {
            auto& shader = it->second;

            if (FAILED(view->hpDevice->CreateVertexShader(shader.VS().get(), shader.VS().size(), NULL, hpVertexShader.ToCreator()))) {
                return false;
            }
            view->hpDeviceContext->VSSetShader(hpVertexShader.Get(), NULL, 0);

            if (shader.PS().isValid()) {
                if (FAILED(view->hpDevice->CreatePixelShader(shader.PS().get(), shader.PS().size(), NULL, hpPixelShader.ToCreator()))) {
                    return false;
                }
                view->hpDeviceContext->PSSetShader(hpPixelShader.Get(), NULL, 0);
            }
            else {
                view->hpDeviceContext->PSSetShader(nullptr, NULL, 0);
            }
            
            auto err = view->hpDevice->CreateInputLayout(&inElemDesc[0], inElemDesc.size(), shader.VS().get(), shader.VS().size(), hpInputLayout.ToCreator());
            if (FAILED(err)) {
                return false;
            }

            TextureParam param;
            param.format = TextureFormat::RGBA8_UNORM;
            param.bindFlag = TextureBindTarget::SHADER_RESOURCE;

            for (auto&& texRes : shader.GetTextureResources()) {
                D3D11TextureProxy texture;
                switch (texRes.first._type) {
                case ResourceType::Texture2D:
                    param.type = TextureType::Texture2D;
                    break;
                case ResourceType::TextureCube:
                    param.type = TextureType::TextureCube;
                    break;
                }
                texture.Initialize(param, texRes.first._resource);
                view->hpDeviceContext->PSSetShaderResources(texRes.second, 1, texture.GetSubResourceView().Ref());
                view->hpDeviceContext->PSSetSamplers(texRes.second, 1, texture.GetSampler().Ref());
            }

            for (auto && rawRes : shader.GetRawResources()) {
                D3D11_BUFFER_DESC bufferDesc;
                D3D11_SUBRESOURCE_DATA subResource;
                subResource.pSysMem = rawRes.first._resource.get();
                bufferDesc.ByteWidth = rawRes.first._resource.size();
                bufferDesc.Usage = D3D11_USAGE_DEFAULT;
                bufferDesc.CPUAccessFlags = 0;
                bufferDesc.MiscFlags = 0;

                bufferDesc.StructureByteStride = rawRes.first._structureByteStride;
                bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                ComPtr<ID3D11Buffer> buffer;
                if (FAILED(view->hpDevice->CreateBuffer(&bufferDesc, &subResource, buffer.ToCreator()))) {
                    return false;
                }
                view->hpDeviceContext->PSSetConstantBuffers(rawRes.second, 1, buffer.Ref());
            }

            if (indexBuffer.Get()) {
                view->hpDeviceContext->DrawIndexed(shader.faceNumVerts, shader.startIndex, 0);
            }
            else {
                view->hpDeviceContext->Draw(shader.faceNumVerts, shader.startIndex);
            }
        }
    }

    return true;
}

template D3D11DrawElement<Vertex3D>;
template D3D11DrawElement<SimpleVertex>;
template D3D11DrawElement<PMDVertex>;
template D3D11DrawElement<TexVertex>;
