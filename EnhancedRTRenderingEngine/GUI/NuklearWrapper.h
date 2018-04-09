#pragma once

#include <memory>

#include "NuklearContexts.h"

class GIImmediateCommands;
class GITexture2D;
class GIShaderResourceView;

class NuklearWrapper
{
public:
    NuklearWrapper(GIImmediateCommands* cmd);
    ~NuklearWrapper();

    nk_context* Context() const {
        return _contexts.context;
    }

    nk_font_atlas* FontAtlas() const {
        return _contexts.atlas;
    }

    nk_buffer* Commands() const {
        return _contexts.nkCmds;
    }

    NuklearContexts _contexts;
    std::shared_ptr<GITexture2D> _atlasTexture;
    std::shared_ptr<GIShaderResourceView> _atlasTextureSRV;
};

