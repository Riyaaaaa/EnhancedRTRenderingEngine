#include "stdafx.h"
#include "D3DX11Device.h"

#include "../Common.h"

#pragma comment(lib, "d3d11.lib")


bool D3DX11Device::Initialize(HWND hWnd)
{
	HRESULT hr;
	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&hpDevice,
		NULL,
		&hpDeviceContext);
	if FAILED(hr)
	{
		return false;
	}

	//�C���^�[�t�F�[�X�擾
	if (FAILED(hpDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&hpDXGI))) {
		return false;
	}

	//�A�_�v�^�[�擾
	if (FAILED(hpDXGI->GetAdapter(&hpAdapter))) {
		return false;
	}

	//�t�@�N�g���[�擾
	hpAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&hpDXGIFactory);
	if (hpDXGIFactory == NULL) {
		return false;
	}

	//�X���b�v�`�F�C���쐬
	DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
	hDXGISwapChainDesc.BufferDesc.Width = 1980;
	hDXGISwapChainDesc.BufferDesc.Height = 1080;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	hDXGISwapChainDesc.SampleDesc.Count = 1;
	hDXGISwapChainDesc.SampleDesc.Quality = 0;
	hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	hDXGISwapChainDesc.BufferCount = 1;
	hDXGISwapChainDesc.OutputWindow = hWnd;
	hDXGISwapChainDesc.Windowed = TRUE;
	hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (FAILED(hpDXGIFactory->CreateSwapChain(hpDevice, &hDXGISwapChainDesc, &hpDXGISwpChain))) {
		return false;
	}

	//���̃X���b�v�`�F�C���̃o�b�N�o�b�t�@�擾
	if (FAILED(hpDXGISwpChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&hpBackBuffer))) {
		return false;
	}

	//���̃o�b�N�o�b�t�@����`��^�[�Q�b�g����
	if (FAILED(hpDevice->CreateRenderTargetView(hpBackBuffer, NULL, &hpRenderTargetView))) {
		return false;
	}

	//�X�ɂ��̕`��^�[�Q�b�g���R���e�L�X�g�ɐݒ�
	hpDeviceContext->OMSetRenderTargets(1, &hpRenderTargetView, NULL);

	//�r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	hpDeviceContext->RSSetViewports(1, &vp);

	return true;
}

bool D3DX11Device::EnableFullScreen(HWND hWnd) {
	//ALT+Enter�Ńt���X�N���[����������
	if (FAILED(hpDXGIFactory->MakeWindowAssociation(hWnd, 0))) {
		return false;
	}
	return true;
}


D3DX11Device::~D3DX11Device()
{
	SAFE_RELEASE(hpRenderTargetView);
	SAFE_RELEASE(hpBackBuffer);
	SAFE_RELEASE(hpDXGISwpChain);
	SAFE_RELEASE(hpDXGIFactory);
	SAFE_RELEASE(hpAdapter);
	SAFE_RELEASE(hpDXGI);
	SAFE_RELEASE(hpDeviceContext);
	SAFE_RELEASE(hpDevice);
}
