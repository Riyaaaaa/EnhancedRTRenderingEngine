#pragma once

#include "RenderingContext.h"

template<class T, class U = void>
T CastToD3D11Formart(U prop);

UINT GetMemoryBlockSize(VertexProperty prop);
