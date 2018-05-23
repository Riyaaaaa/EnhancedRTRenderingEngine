#pragma once

class TextureBuffer {
public:
    TextureBuffer(unsigned int width, unsigned int height, unsigned int channel = 4) :
        width(width),
        height(height),
        channel(channel),
        buf(new unsigned char[width * height * channel]) {}

    unsigned char* Get() {
        return buf;
    }

    unsigned int Size() {
        return width * height * channel;
    }

    unsigned char& operator()(unsigned int x, unsigned int y, unsigned col_offset) {
        return buf[y * width * channel + x * channel + col_offset];
    }

    template<unsigned int Dim>
    void FillColor(_Vector<float, Dim> color) {
        for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width; x++) {
                for (unsigned int c = 0; c < channel; c++) {
                    (*this)(x, y, c) = color[c];
                }
            }
        }
    }

protected:
    unsigned int width, height, channel;
    unsigned char* buf;
};