#include "stdafx.h"

#include "D3D11GaussianFilter.h"

#include "D3D11TextureEffectRenderer.h"

D3D11Texture D3D11GaussianFilter(const std::shared_ptr<D3DX11RenderView>& view, D3D11Texture& src) {
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
    src.GetTexture()->GetDesc(&desc);
    buf.texsize.w = desc.Width;
    buf.texsize.h = desc.Height;

    D3D11TextureEffectRenderer renderer;
    renderer.Initialize(view);
    renderer.SetConstantBuffer(&buf);

    auto XFiltered = renderer.Apply(src, "Gaussian5x5FilterX");
    auto Final = renderer.Apply(XFiltered, "Gaussian5x5FilterY");

    return Final;
}