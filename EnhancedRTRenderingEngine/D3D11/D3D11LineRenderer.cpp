#include "stdafx.h"
#include "D3D11LineRenderer.h"
#include "D3D11FormatUtils.h"
#include "D3D11DrawElement.h"
#include "D3D11TextureEffects.h"

#include "GraphicsInterface/GIDrawMesh.h"
#include "D3D11DrawElement.h"
#include "D3D11ConstantBufferBuilder.h"
#include "Constant/RenderTag.h"

#include "Common.h"

using namespace DirectX;

bool D3D11LineRenderer::Initialize(const std::shared_ptr<D3D11RenderView>& view) {
    _view = view;
    return true;
}

void D3D11LineRenderer::render(const CameraObject& camera, const std::vector<Line>& lines) {
    if (!_view) {
        return;
    }

    _view->SetViewPortSize(_view->GetRenderSize());
    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), _view->hpDepthStencilView.Get());
    _view->hpDeviceContext->ClearDepthStencilView(_view->hpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    TransformBufferParam hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(camera.GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(camera.GetPerspectiveProjection());

    ComPtr<ID3D11Buffer> hpConstantBuffer(nullptr);

    hpConstantBuffer = D3D11ConstantBufferBuilder::BuildConstantBuffer<TransformBufferParam>(_view->hpDevice, &hConstantBuffer);

    _view->hpDeviceContext->VSSetConstantBuffers(0, 1, hpConstantBuffer.Ref());

    auto lineMesh = std::make_shared<MeshBase<Vertex3D>>();
    
    for (auto && line : lines) {
        Vector4D color = Vector4D(line.color.r, line.color.g, line.color.b);
        Vertex3D bvert{ line.bpos, color, Vector2D(0, 0) };
        Vertex3D evert{ line.epos, color, Vector2D(0, 0) };

        lineMesh->AddVertex(bvert);
        lineMesh->AddVertex(evert);
    }

    lineMesh->SetPrimitiveType(VertexPrimitiveType::LINELIST);

    MeshObject<Vertex3D> mesh(lineMesh);
    GIDrawMesh element(&mesh);
    GIDrawElement face(ShaderFactory::MinPixelColor(), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;
    element.AddDrawElement(face);
    D3D11DrawElement drawer;
    drawer.Draw(_view, element);
}
