#pragma once

#include "NuklearContexts.h"

class NuklearWrapper
{
public:
    NuklearWrapper();
    ~NuklearWrapper();

    nk_context* Context() {
        return _contexts.context;
    }

    nk_font_atlas* FontAtlas() {
        return _contexts.atlas;
    }

    nk_buffer* Commands() {
        return _contexts.nkCmds;
    }

    NuklearContexts _contexts;
};

