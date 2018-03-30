#include "stdafx.h"
#include "D3D11UnlitRenderer.h"
#include "D3D11FormatUtils.h"
#include "D3D11DrawElement.h"
#include "D3D11TextureEffects.h"

#include "GraphicsInterface/GIDrawMesh.h"
#include "D3D11DrawElement.h"
#include "D3D11ConstantBufferBuilder.h"
#include "Constant/RenderTag.h"

#include "Common.h"

using namespace DirectX;

template<class VertType>
void D3D11UnlitRenderer::render(GIImmediateCommands* cmd, const CameraObject& camera, std::vector<MeshObject<VertType>>& meshes) {
    if (!_view) {
        return;
    }

    cmd->SetViewPortSize(_view->GetRenderSize());

    cmd->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), _view->hpDepthStencilView.Get());
    cmd->ClearDepthStencilView(_view->hpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    TransformBufferParam hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(camera.GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(camera.GetPerspectiveProjection());

    ComPtr<ID3D11Buffer> hpConstantBuffer(nullptr);

    hpConstantBuffer = D3D11ConstantBufferBuilder::BuildConstantBuffer<TransformBufferParam>(_view->hpDevice, &hConstantBuffer);

    cmd->VSSetConstantBuffers(0, 1, hpConstantBuffer.Ref());

    for (auto && object : meshes) {
        GIDrawMesh element(&object);
        GIDrawElement face(ShaderFactory::MinPixelColor(), ShaderFactory::MinVertexColor());

        face.startIndex = 0;
        if (object.GetMesh()->HasIndexList()) {
            face.faceNumVerts = object.GetMesh()->GetIndexList().size();
        }
        else {
            face.faceNumVerts = object.GetMesh()->GetVertexList().size();
        }
        element.AddDrawElement(face);
        D3D11DrawElement draw;
        draw.Draw(_view, element);
    }
}

template void D3D11UnlitRenderer::render<Vertex3D>(GIImmediateCommands* cmd, const CameraObject& camera, std::vector<MeshObject<Vertex3D>>& meshes);
