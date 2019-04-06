
#include "BasePassRenderer.h"

#include "DrawMesh.h"

#include "Constant/RenderTag.h"
#include "Utility/SceneUtils.h"

#include "GraphicsInterface/GICommandUtils.h"

#include "UserData/UserConfig.h"

#include "WindowsApp.h"
#include "Common/Common.h"

using namespace DirectX;

D3D11BasePassRenderer::D3D11BasePassRenderer(GIImmediateCommands* cmd) {
    BufferDesc desc;
    desc.stride = sizeof(float);
    desc.byteWidth = sizeof(MaterialBuffer);
    materialBuffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, desc));
    desc.byteWidth = sizeof(ObjectBuffer);
    objectBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc));
    desc.byteWidth = sizeof(ConstantBuffer);
    desc.usage = ResourceUsage::Dynamic;
    desc.accessFlag = ResourceAccessFlag::W;
    constantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc));
}

void D3D11BasePassRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* renderScene) {
    Scene* scene = renderScene->GetSourceScene();

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, view->GetOMResource()->depthStencilView);
    cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get(), 1.0f, 0);

    view->SetRasterizerState(cmd, UserConfig::getInstance()->RasterizerType());
    
    ConstantBuffer hConstantBuffer = SceneUtils::CreateBasePassConstantBuffer(scene);

    auto mapped = cmd->MapBuffer(constantBuffer.get(), 0, MapType::WRITE_DISCARD);
    memcpy(mapped.pData, &hConstantBuffer, sizeof(hConstantBuffer));
    cmd->UnmapBuffer(constantBuffer.get(), 0);

    cmd->VSSetConstantBuffers(0, constantBuffer.get());
    cmd->PSSetConstantBuffers(0, constantBuffer.get());

    // todo: support multi lights
    if (hConstantBuffer.numDirecitonalLights > 0) {
        cmd->PSSetShaderResources(0, renderScene->GetDirectionalShadow(0)->GetSubResourceView().get());
        cmd->PSSetSamplers(0, renderScene->GetDirectionalShadow(0)->GetSampler().get());
    }
    
    if (hConstantBuffer.numPointLights > 0) {
        cmd->PSSetShaderResources(1, renderScene->GetPointShadow(0)->GetSubResourceView().get());
        cmd->PSSetSamplers(1, renderScene->GetPointShadow(0)->GetSampler().get());
    }

    for (auto && drawface : renderScene->GetDrawList()) {
        drawface.Draw(cmd);
    }
    
    cmd->PSSetSamplers(0, nullptr);
    cmd->PSSetShaderResources(0, nullptr);
    cmd->PSSetSamplers(1, nullptr);
    cmd->PSSetShaderResources(1, nullptr);
    cmd->ResetPS();
}
