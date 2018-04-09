#include "stdafx.h"
#include "NuklearWrapper.h"

#include "Resource/Texture2D.h"
#include "nuklear/nuklear.h"
#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIResource.h"
#include "FileManager.h"


NuklearWrapper::NuklearWrapper(GIImmediateCommands* cmd)
{
    nk_init_default(_contexts.context, 0);
    //_context.clip.copy
    //_context.clip.pase
    nk_buffer_init_default(_contexts.nkCmds);
    nk_font_atlas_init_default(_contexts.atlas);
    nk_font_atlas_begin(_contexts.atlas);
    //nk_font_atlas_add_from_file(_atlas, "../../extra_font/DroidSans.ttf", 14, 0);
    const void *buf; int w, h;
    buf = nk_font_atlas_bake(_contexts.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);

    Texture2D atlasTexture((std::size_t)w, (std::size_t)h, 4ul, buf, w * h * 4);
    FileManager::getInstance()->AddCache<Texture2D>("FontAtlas_HUD", atlasTexture);

    TextureParam param;
    param.width = w;
    param.height = h;
    param.type = TextureType::Texture2D;
    _atlasTexture = MakeRef(cmd->CreateTexture2D(param, std::vector<Texture2D>{atlasTexture}));
    _atlasTextureSRV = MakeRef(cmd->CreateShaderResourceView(_atlasTexture.get()));

    nk_draw_null_texture null;
    nk_font_atlas_end(_contexts.atlas, nk_handle_ptr(_atlasTextureSRV.get()), &null);
    if (_contexts.atlas->default_font)
        nk_style_set_font(_contexts.context, &_contexts.atlas->default_font->handle);
}


NuklearWrapper::~NuklearWrapper()
{
    nk_font_atlas_clear(_contexts.atlas);
    nk_buffer_free(_contexts.nkCmds);
    nk_free(_contexts.context);
}
