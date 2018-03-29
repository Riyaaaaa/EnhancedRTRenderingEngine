#pragma once

#include <d3d11.h>

#include "D3D11/ComPtr.h"
#include "GraphicsInterface/GIResource.h"

class D3D11Buffer : public GIBuffer {
public:
    ComPtr<ID3D11Buffer> buffer;
};
