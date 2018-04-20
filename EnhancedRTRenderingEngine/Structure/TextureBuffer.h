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

protected:
    unsigned int width, height, channel;
    unsigned char* buf;
};