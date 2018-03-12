#include "stdafx.h"
#include "D3D11ConstantBufferBuilder.h"

#include "Scene/Scene.h"

using namespace DirectX;

ConstantBuffer D3D11ConstantBufferBuilder::CreateBasePassConstantBuffer(Scene* scene) {
    ConstantBuffer hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(scene->GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());
    hConstantBuffer.Eye = scene->GetEyePoint();

    // Only support one light.
    hConstantBuffer.numDirecitonalLights = scene->GetDirectionalLights().size();
    for (int i = 0; i < LIGHT_MAX; i++) {
        if (i >= hConstantBuffer.numDirecitonalLights) {
            break;
        }
        auto& dLight = scene->GetDirectionalLights()[i];
        hConstantBuffer.DirectionalLightView[i] = XMMatrixTranspose(dLight.GetViewProjection());
        hConstantBuffer.DirectionalLightProjection[i] = XMMatrixTranspose(dLight.GetPerspectiveProjection());
        hConstantBuffer.DirectionalLight[i] = dLight.GetDirection();
    }

    hConstantBuffer.numPointLights = scene->GetPointLightParams().size();
    for (int i = 0; i < LIGHT_MAX; i++) {
        if (i >= hConstantBuffer.numPointLights) {
            break;
        }
        auto& pLight = scene->GetPointLights()[i];
        hConstantBuffer.PointLightProjection[i] = pLight.GetShadowPerspectiveMatrix();
        memcpy(hConstantBuffer.PointLightView[i], pLight.GetViewMatrixes(), sizeof(XMMATRIX) * 6);
        hConstantBuffer.PointLight[i] = scene->GetPointLightParams()[i];
    }

    return hConstantBuffer;
}