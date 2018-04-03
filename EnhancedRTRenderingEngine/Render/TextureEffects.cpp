#include "stdafx.h"

#include "TextureEffects.h"

#include "TextureEffectRenderer.h"

std::shared_ptr<GITexture2D> GaussianFilter(GIImmediateCommands* cmd, std::shared_ptr<GITexture2D> src) {
    struct GaussianCBuffer {
        float weight[8];
        Size texsize;
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

    D3D11_TEXTURE2D_DESC desc;
    auto param = src->GetTextureParam();
    buf.texsize.w = param.width;
    buf.texsize.h = param.height;

    TextureEffectRenderer renderer;
    auto constantBuffer = cmd->CreateBuffer(ResourceType::PSConstantBuffer, sizeof(float), &buf, sizeof(buf));

    SamplerParam p;
    p.addressMode = TextureAddressMode::CLAMP;
    GITextureProxy proxy = MakeRef(cmd->CreateTextureProxy(src, p));

    auto XFiltered = renderer.Apply(cmd, proxy, "Gaussian5x5FilterX");
    auto XFilteredProxy = MakeRef(cmd->CreateTextureProxy(XFiltered, p));
    auto Final = renderer.Apply(cmd, XFilteredProxy, "Gaussian5x5FilterY");

    return Final;
}