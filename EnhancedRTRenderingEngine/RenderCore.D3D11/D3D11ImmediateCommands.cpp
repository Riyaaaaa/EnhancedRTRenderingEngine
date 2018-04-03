#include "stdafx.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "D3D11ImmediateCommands.h"
#include "D3D11FormatUtils.h"
#include "D3D11Resources.h"

D3D11ImmediateCommands::D3D11ImmediateCommands(bool checkDeviceObjectLeaks) :
    _checkDeviceObjectLeaks(checkDeviceObjectLeaks) {
    HRESULT hr = D3D11CreateDevice(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_DEBUG,
        NULL,
        0,
        D3D11_SDK_VERSION,
        _device.ToCreator(),
        NULL,
        _deviceContext.ToCreator());
    if (FAILED(hr))
    {
        throw std::runtime_error("failed initialize graphics device");
    }

    if (checkDeviceObjectLeaks) {
        hr = _device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(_pD3dDebug.ToCreator()));
        if (FAILED(hr))
        {
            throw std::runtime_error("failed initialize debug device");
        }
    }
}

D3D11ImmediateCommands::~D3D11ImmediateCommands() {
    if (_checkDeviceObjectLeaks) {
        _pD3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    }
}

void D3D11ImmediateCommands::OMSetRenderTargets(const std::vector<std::shared_ptr<GIRenderTargetView>>& renderTargets, std::shared_ptr<GIDepthStencilView> stv) {
    std::vector<ID3D11RenderTargetView*> rtvs_;

    for (auto && rtv : renderTargets) {
        rtvs_.push_back(CastRes<D3D11RenderTargetView>(rtv.get()).Get());
    }

    auto res = NullableCastRes<D3D11DepthStencilView>(stv.get());
    _deviceContext->OMSetRenderTargets(rtvs_.size(), &rtvs_[0], res.Get());
}

GISwapChain* D3D11ImmediateCommands::CreateSwapChain(const ViewportParam& param) {
    D3D11SwapChain* swapchain = new D3D11SwapChain;
    _device->QueryInterface(__uuidof(IDXGIDevice1), (void**)swapchain->dxgiDevice.ToCreator());
    swapchain->dxgiDevice->GetAdapter(swapchain->adapter.ToCreator());
    swapchain->adapter->GetParent(__uuidof(IDXGIFactory), (void**)swapchain->factory.ToCreator());

    DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
    hDXGISwapChainDesc.BufferDesc.Width = param.cfg.width;
    hDXGISwapChainDesc.BufferDesc.Height = param.cfg.height;
    hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    hDXGISwapChainDesc.SampleDesc = CastToD3D11Format<DXGI_SAMPLE_DESC>(param.MSAAQuality);
    hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    hDXGISwapChainDesc.BufferCount = 1;
    hDXGISwapChainDesc.OutputWindow = reinterpret_cast<HWND>(param.runtimeWindowHandle);
    hDXGISwapChainDesc.Windowed = TRUE;
    hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    swapchain->factory->CreateSwapChain(_device.Get(), &hDXGISwapChainDesc, swapchain->resource.ToCreator());
    
    swapchain->backBuffer = std::make_shared<D3D11Texture2D>();
    swapchain->resource->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)CastRes<D3D11Texture2D>(swapchain->backBuffer.get()).ToCreator());

    return swapchain;
}

void D3D11ImmediateCommands::SetViewport(const ViewportCfg& cfg) {
    D3D11_VIEWPORT d3d11_cfg;

    d3d11_cfg.TopLeftX = cfg.topLeftX;
    d3d11_cfg.TopLeftY = cfg.topLeftY;
    d3d11_cfg.Width = cfg.width;
    d3d11_cfg.Height = cfg.height;
    d3d11_cfg.MinDepth = cfg.minDepth;
    d3d11_cfg.MaxDepth = cfg.maxDepth;

    _deviceContext->RSSetViewports(1, &d3d11_cfg);
}

void D3D11ImmediateCommands::UpdateSubresource(GIBuffer* buffer, void* srcData, unsigned int srcRowPitch) {
    _deviceContext->UpdateSubresource(CastRes<D3D11Buffer>(buffer).Get(), 0, nullptr, srcData, srcRowPitch, 0);
}

void D3D11ImmediateCommands::CopyTexture2D(GITexture2D* dst, unsigned int idx, unsigned int mipLevels, GITexture2D* src) {
    _deviceContext->CopySubresourceRegion(CastRes<D3D11Texture2D>(dst).Get(), D3D11CalcSubresource(0, idx, mipLevels), 0, 0, 0, CastRes<D3D11Texture2D>(src).Get(), 0, nullptr);
}

void D3D11ImmediateCommands::CopyTexture2DFromArray(GITexture2D* dst, GITexture2D* src, unsigned int srcIdx, unsigned int srcMipLevels) {
    _deviceContext->CopySubresourceRegion(CastRes<D3D11Texture2D>(dst).Get(), 0, 0, 0, 0, CastRes<D3D11Texture2D>(src).Get(), D3D11CalcSubresource(0, srcIdx, srcMipLevels), nullptr);
}

GITexture2D* D3D11ImmediateCommands::CreateTexture2D(const TextureParam& param, const std::vector<Texture2D>& textures) {
    D3D11Texture2D* tex = new D3D11Texture2D;
    
    std::vector<D3D11_SUBRESOURCE_DATA> initData;
    D3D11_SUBRESOURCE_DATA* initDataPtr = nullptr;

    unsigned int width, height, arraySize;
    if (!textures.empty()) {
        width = textures[0].Width();
        height = textures[0].Height();
        arraySize = textures.size();

        initData.resize(arraySize);
        for (int i = 0; i < arraySize; i++) {
            initData[i].pSysMem = textures[i].get();
            initData[i].SysMemPitch = textures[i].Stride();
        }

        initDataPtr = &initData[0];
    }
    else {
        width = param.width;
        height = param.height;
        arraySize = param.arraySize;
    }

    D3D11_TEXTURE2D_DESC desc;
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = arraySize;
    desc.Format = CastToD3D11Format<DXGI_FORMAT>(param.format);
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = CastToD3D11Format<D3D11_USAGE>(param.usage);
    desc.BindFlags = CastToD3D11Format<UINT>(param.bindFlag);
    desc.CPUAccessFlags = CastToD3D11Format<UINT>(param.accessFlag);
    desc.MiscFlags = 0;

    switch (param.type) {
    case TextureType::TextureCube:
        desc.MiscFlags |= D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_TEXTURECUBE;
        break;
    }

    _device->CreateTexture2D(&desc, initDataPtr, tex->resource.ToCreator());

    return tex;
}

GIShaderResourceView* D3D11ImmediateCommands::CreateShaderResourceView(GITexture2D* tex) {
    D3D11ShaderResourceView* srv = new D3D11ShaderResourceView;
    auto texparam = tex->GetTextureParam();

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = GetShaderResourceFormat(CastToD3D11Format<DXGI_FORMAT>(texparam.format));

    if (texparam.type == TextureType::TextureCube) {
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        SRVDesc.TextureCube.MostDetailedMip = 0;
        SRVDesc.TextureCube.MipLevels = texparam.mipLevels;
    }
    else {
        if (!texparam.isMultiSampling) {
            if (texparam.arraySize == 1) {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                SRVDesc.Texture2DArray.MipLevels = texparam.mipLevels;
            }
            else {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                SRVDesc.Texture2DArray.MipLevels = texparam.mipLevels;
                SRVDesc.Texture2DArray.ArraySize = texparam.arraySize;
            }
        }
        else {
            if (texparam.arraySize == 1) {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
            }
            else {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
            }
        }
    }

    _device->CreateShaderResourceView(CastRes<D3D11Texture2D>(tex).Get(), &SRVDesc, srv->resource.ToCreator());
    return srv;
}

GISamplerState* D3D11ImmediateCommands::CreateSamplerState(const SamplerParam& param) {
    D3D11SamplerState* state = new D3D11SamplerState;

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.addressMode);
    samplerDesc.AddressV = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.addressMode);
    samplerDesc.AddressW = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.addressMode);
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    _device->CreateSamplerState(&samplerDesc, state->resource.ToCreator());

    return state;
}


GIBuffer* D3D11ImmediateCommands::CreateBuffer(ResourceType type, unsigned int stride, float byteWidth, void* initPtr) {
    D3D11_BUFFER_DESC bufferDesc;
    D3D11Buffer* buffer = new D3D11Buffer;

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = stride;
    bufferDesc.ByteWidth = byteWidth;

    switch (type) {
    case ResourceType::VertexList: 
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        break;
    case ResourceType::IndexList:
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        break;
    case ResourceType::VSConstantBuffer:
    case ResourceType::PSConstantBuffer:
    case ResourceType::GSConstantBuffer:
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        break;
    }

    D3D11_SUBRESOURCE_DATA srd;
    D3D11_SUBRESOURCE_DATA* resPtr = nullptr;

    if (initPtr) {
        srd.pSysMem = initPtr;
        resPtr = &srd;
    }

    _device->CreateBuffer(&bufferDesc, resPtr, buffer->resource.ToCreator());
    return buffer;
}

GIInputLayout* D3D11ImmediateCommands::CreateInputLayout(const std::vector<VertexLayout>& layouts, GIVertexShader* shader) {
    std::vector<D3D11_INPUT_ELEMENT_DESC> inElemDesc;
    for (auto&& layout : layouts) {
        inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{
            layout.name, layout.semanticsIndex,
            CastToD3D11Format<DXGI_FORMAT>(layout.vProperty),
            layout.slot,
            layout.memoryOffset,
            D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }

    D3D11InputLayout* inputLayout = new D3D11InputLayout;
    _device->CreateInputLayout(&inElemDesc[0], inElemDesc.size(), shader->byteCode.get(), shader->byteCode.size(), inputLayout->resource.ToCreator());
    return inputLayout;
}

GIRenderTargetView* D3D11ImmediateCommands::CreateRenderTargetView(GITexture2D* tex) {
    D3D11RenderTargetView* rtv = new D3D11RenderTargetView;

    _device->CreateRenderTargetView(CastRes<D3D11Texture2D>(tex).Get(), nullptr, rtv->resource.ToCreator());

    return rtv;
}

GIDepthStencilView* D3D11ImmediateCommands::CreateDepthStencilView(GITexture2D* tex, TextureFormat format) {
    D3D11DepthStencilView* dsv = new D3D11DepthStencilView;

    D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
    hDepthStencilViewDesc.Format = CastToD3D11Format<DXGI_FORMAT>(format);
    hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    hDepthStencilViewDesc.Flags = 0;

    _device->CreateDepthStencilView(CastRes<D3D11Texture2D>(tex).Get(), &hDepthStencilViewDesc, dsv->resource.ToCreator());

    return dsv;
}

void D3D11ImmediateCommands::ClearRenderTargetView(GIRenderTargetView* view, Vector4D color) {
    _deviceContext->ClearRenderTargetView(CastRes<D3D11RenderTargetView>(view).Get(), reinterpret_cast<float*>(&color));
}

void D3D11ImmediateCommands::ClearDepthStencilView(GIDepthStencilView* view, float depthClearVal, float stencilClearVal) {
    _deviceContext->ClearDepthStencilView(CastRes<D3D11DepthStencilView>(view).Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depthClearVal, stencilClearVal);
}

GIPixelShader* D3D11ImmediateCommands::CreatePixelShader(RawBinary byteCode) {
    D3D11PixelShader* shader = new D3D11PixelShader;
    _device->CreatePixelShader(byteCode.get(), byteCode.size(), nullptr, shader->resource.ToCreator());
    shader->byteCode = byteCode;
    return shader;
}

GIVertexShader* D3D11ImmediateCommands::CreateVertexShader(RawBinary byteCode) {
    D3D11VertexShader* shader = new D3D11VertexShader;
    _device->CreateVertexShader(byteCode.get(), byteCode.size(), nullptr, shader->resource.ToCreator());
    shader->byteCode = byteCode;
    return shader;
}

void D3D11ImmediateCommands::RSSetState(GIRasterizerState* state) {
    _deviceContext->RSSetState(CastRes<D3D11RasterizerState>(state).Get());
}

void D3D11ImmediateCommands::PSSetShaderResources(unsigned int slot, GIShaderResourceView* texture) {
    _deviceContext->PSSetShaderResources(slot, 1, NullableCastRes<D3D11ShaderResourceView>(texture).Ref());
}

void D3D11ImmediateCommands::PSSetSamplers(unsigned int slot, GISamplerState* sampler) {
    _deviceContext->PSSetSamplers(slot, 1, NullableCastRes<D3D11SamplerState>(sampler).Ref());
}

void D3D11ImmediateCommands::PSSetShader(GIPixelShader* shader) {
    _deviceContext->PSSetShader(NullableCastRes<D3D11PixelShader>(shader).Get(), nullptr, 0);
}

void D3D11ImmediateCommands::ResetPS() {
    _deviceContext->PSSetShader(nullptr, nullptr, 0);
}

void D3D11ImmediateCommands::VSSetShader(GIVertexShader* shader) {
    _deviceContext->VSSetShader(CastRes<D3D11VertexShader>(shader).Get(), nullptr, 0);
}

void D3D11ImmediateCommands::PSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) {
    _deviceContext->PSSetConstantBuffers(slot, 1, CastRes<D3D11Buffer>(buffer).Ref());
}

void D3D11ImmediateCommands::VSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) {
    _deviceContext->VSSetConstantBuffers(slot, 1, CastRes<D3D11Buffer>(buffer).Ref());
}

GIRasterizerState* D3D11ImmediateCommands::CreateRasterizerState(RasterizerType type) {
    D3D11RasterizerState* state = new D3D11RasterizerState;
    CD3D11_RASTERIZER_DESC desc(D3D11_DEFAULT);

    switch (type) {
    case RasterizerType::Defalt:
        break;
    case RasterizerType::DoubleSided:
        desc.CullMode = D3D11_CULL_NONE;
        break;
    case RasterizerType::WireFrame:
        desc.CullMode = D3D11_CULL_NONE;
        desc.FillMode = D3D11_FILL_WIREFRAME;
        break;
    }
    
    _device->CreateRasterizerState(&desc, state->resource.ToCreator());
    return state;
}


void D3D11ImmediateCommands::IASetPrimitiveTopology(VertexPrimitiveType primitiveType) {
    _deviceContext->IASetPrimitiveTopology(CastToD3D11Format<D3D_PRIMITIVE_TOPOLOGY>(primitiveType));
}

void D3D11ImmediateCommands::IASetIndexBuffer(GIBuffer* buffer, unsigned int offset) {
    _deviceContext->IASetIndexBuffer(CastRes<D3D11Buffer>(buffer).Get(), DXGI_FORMAT_R16_UINT, 0);
}

void D3D11ImmediateCommands::IASetVertexBuffer(GIBuffer* buffer, unsigned int stride, unsigned int offset) {
    _deviceContext->IASetVertexBuffers(0, 1, CastRes<D3D11Buffer>(buffer).Ref(), &stride, &offset);
}

void D3D11ImmediateCommands::IASetInputLayout(GIInputLayout* layout) {
    _deviceContext->IASetInputLayout(CastRes<D3D11InputLayout>(layout).Get());
}

GITextureProxyEntity* D3D11ImmediateCommands::CreateTextureProxy(TextureParam param, const Texture2D & tex) {
    GITextureProxyEntity* proxy = new GITextureProxyEntity();
    proxy->Initialize(this, param, tex);
    return proxy;
}

GITextureProxyEntity* D3D11ImmediateCommands::CreateTextureProxy(TextureParam param, const std::vector<Texture2D> & tex) {
    GITextureProxyEntity* proxy = new GITextureProxyEntity();
    proxy->Initialize(this, param, tex);
    return proxy;
}

GITextureProxyEntity* D3D11ImmediateCommands::CreateTextureProxy(std::shared_ptr<GITexture2D> tex, SamplerParam param) {
    GITextureProxyEntity* proxy = new GITextureProxyEntity();
    proxy->Initialize(this, tex, param);
    return proxy;
}

void D3D11ImmediateCommands::DrawIndexed(unsigned int indexCount, unsigned int startIndex, unsigned int baseIndex) {
    _deviceContext->DrawIndexed(indexCount, startIndex, baseIndex);
}

void D3D11ImmediateCommands::Draw(unsigned int vertexCount, unsigned int startIndex) {
    _deviceContext->Draw(vertexCount, startIndex);
}

