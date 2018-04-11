#pragma once

#include "Resource/Texture2D.h"

namespace CompressTexture {
    Texture2D NearestNeighbor(Texture2D tex, Size2Dd compressedSize);
}
