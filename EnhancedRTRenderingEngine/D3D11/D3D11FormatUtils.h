#pragma once

#include "RenderingContext.h"

template<class T, class U = void>
T CastToD3D11Format(U prop);

DXGI_FORMAT GetShaderResourceFormat(DXGI_FORMAT textureFormat);

UINT GetMemoryBlockSize(VertexProperty prop);
