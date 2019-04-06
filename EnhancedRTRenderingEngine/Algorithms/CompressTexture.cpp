
#include "CompressTexture.h"

#include "Structure/Aliases.h"

Texture2D CompressTexture::NearestNeighbor(Texture2D tex, Size2Dd compressedSize) {
    Size2Dd src_size(tex.Width(), tex.Height());
    unsigned int channels = tex.Channels();
    std::size_t size = channels * compressedSize.w * compressedSize.h;
    char* buf = new char[size];
    const char* srcbuf = reinterpret_cast<const char*>(tex.get());

    for (unsigned int i = 0; i < compressedSize.h; i++) {
        for (unsigned int j = 0; j < compressedSize.w; j++) {
            _Vector2D<unsigned int> src_idx(
                j / static_cast<float>(compressedSize.w) * src_size.w,
                i / static_cast<float>(compressedSize.h) * src_size.h);

            unsigned int idx = i * compressedSize.w * channels + j * channels;

            for (unsigned int c = 0; c < channels; c++) {
                buf[idx + c] = srcbuf[static_cast<int>(src_idx.y * channels * src_size.w + src_idx.x * channels + c)];
            }
        }
    }

    Texture2D dst(compressedSize.w, compressedSize.h, channels, buf, size);
    return dst;
}
