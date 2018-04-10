#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "NuklearContexts.h"

class GIImmediateCommands;
class GITexture2D;
class GIShaderResourceView;

struct nk_font;

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

    nk_draw_null_texture* Nulltexture() const {
        return _contexts.null;
    }

    void SetFontStyle(std::string key);

    NuklearContexts _contexts;

    std::unordered_map<std::string, nk_font*> _fontHandles;

    std::shared_ptr<GITexture2D> _atlasTexture;
    std::shared_ptr<GIShaderResourceView> _atlasTextureSRV;
};

