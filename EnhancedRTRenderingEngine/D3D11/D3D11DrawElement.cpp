#include "stdafx.h"
#include "D3D11DrawElement.h"

#include "D3D11TextureProxy.h"
#include "D3D11FormatUtils.h"

#include "Structure/Structure.h"
#include "Resource/ResourceLoader.h"
#include "Resource/RawBinary.h"
#include "Constant/RenderTag.h"
#include "Common.h"

bool D3D11DrawElement::Draw(const std::shared_ptr<D3DX11RenderView>& view, const GIDrawElement& element) {
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
            view->hpDeviceContext->IASetInputLayout(hpInputLayout.Get());

            TextureParam param;
            param.format = TextureFormat::RGBA8_UNORM;
            param.bindFlag = TextureBindTarget::SHADER_RESOURCE;

            for (auto&& texRes : shader.GetTextureResources()) {
                auto nativeTex = std::static_pointer_cast<D3D11TextureProxyEntity>(texRes.first);
                view->hpDeviceContext->PSSetShaderResources(texRes.second, 1, nativeTex->GetSubResourceView().Ref());
                view->hpDeviceContext->PSSetSamplers(texRes.second, 1, nativeTex->GetSampler().Ref());
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
