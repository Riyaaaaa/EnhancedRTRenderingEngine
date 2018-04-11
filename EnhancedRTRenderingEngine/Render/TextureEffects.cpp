#include "stdafx.h"

#include "TextureEffects.h"

#include "TextureEffectRenderer.h"

std::shared_ptr<GITexture2D> GaussianFilter(GIImmediateCommands* cmd, std::shared_ptr<GITexture2D> src) {
    struct GaussianCBuffer {
        float weight[8];
        Size2D texsize;
    };
    AlignedBuffer<GaussianCBuffer> buf;

    float total = 0;
    constexpr float disperision = 10.0f;
    for (int i = 0; i < 8; i++) {
        float pos = 1.0f + 2.0f * (float)i;
        buf.weight[i] = std::expf(-0.5f * pos * pos / disperision);
        if (i == 0) {
            total += buf.weight[i];
        }
        else {
            total += 2.0f * buf.weight[i];
        }
    }

    for (int i = 0; i < 8; i++) {
        buf.weight[i] /= total;
    }

    BufferDesc bufferDesc;
    bufferDesc.stride = sizeof(float);
    bufferDesc.byteWidth = sizeof(buf);
    auto constantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, bufferDesc, &buf));
    cmd->PSSetConstantBuffers(0, constantBuffer.get());

    SamplerParam p;
    p.addressMode = TextureAddressMode::CLAMP;
    GITextureProxy proxy = MakeRef(cmd->CreateTextureProxy(src, p));

    auto XFiltered = TextureEffectRenderer::Apply(cmd, proxy, "Gaussian5x5FilterX");
    auto XFilteredProxy = MakeRef(cmd->CreateTextureProxy(XFiltered, p));
    auto Final = TextureEffectRenderer::Apply(cmd, XFilteredProxy, "Gaussian5x5FilterY");

    return Final;
}