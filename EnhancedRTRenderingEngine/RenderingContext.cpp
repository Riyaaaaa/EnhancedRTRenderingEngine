
#include "RenderingContext.h"

UINT GetMemoryBlockSize(VertexProperty prop) {
    switch (prop)
    {
    case VertexProperty::FloatRG:
        return 4 * 2;
    case VertexProperty::FloatRGB:
        return 4 * 3;
    case VertexProperty::FloatRGBA:
        return 4 * 4;
    case VertexProperty::UnormRGBA:
        return 4;
    default:
        return 0;
    }
}
