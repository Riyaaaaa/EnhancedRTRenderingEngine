
#include "NuklearWrapper.h"

#include "Resource/Texture2D.h"
#include "nuklear/nuklear.h"
#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIResource.h"
#include "FileManager.h"

NuklearWrapper::NuklearWrapper(GIImmediateCommands* cmd)
{
    _contexts.atlas = new nk_font_atlas;
    _contexts.nkCmds = new nk_buffer;
    _contexts.context = new nk_context;
    _contexts.null = new nk_draw_null_texture;

    nk_init_default(_contexts.context, 0);
    //_context.clip.copy
    //_context.clip.pase
    nk_buffer_init_default(_contexts.nkCmds);
    nk_font_atlas_init_default(_contexts.atlas);
    nk_font_atlas_begin(_contexts.atlas);

    /* ------ FONT ADD CODES ------- */
    //nk_font_atlas_add_from_file(_atlas, "../../extra_font/DroidSans.ttf", 14, 0);
    nk_font* default_font = nk_font_atlas_add_default(_contexts.atlas, 20.0f, nullptr);
    /* ------ FONT ADD CODES ------- */

    const void *buf; int w, h;
    buf = nk_font_atlas_bake(_contexts.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    Texture2D atlasTexture((std::size_t)w, (std::size_t)h, 4ul, buf, w * h * 4);
    //FileManager::getInstance()->AddCache<Texture2D>("FontAtlas_HUD", atlasTexture);

    TextureParam param;
    param.width = w;
    param.height = h;
    param.type = TextureType::Texture2D;
    _atlasTexture = MakeRef(cmd->CreateTexture2D(param, std::vector<Texture2D>{atlasTexture}));
    _atlasTextureSRV = MakeRef(cmd->CreateShaderResourceView(_atlasTexture.get()));

    //nk_font_atlas_add_from_file(_contexts.context, "Asset\\Font\\DroidSans.ttf", 14, 0);

    nk_font_atlas_end(_contexts.atlas, nk_handle_ptr(_atlasTextureSRV.get()), _contexts.null);
    nk_style_set_font(_contexts.context, &default_font->handle);
}


NuklearWrapper::~NuklearWrapper()
{
    nk_font_atlas_clear(_contexts.atlas);
    nk_buffer_free(_contexts.nkCmds);
    nk_free(_contexts.context);

    delete _contexts.atlas;
    delete _contexts.nkCmds;
    delete _contexts.context;
    delete _contexts.null;
}

void NuklearWrapper::SetFontStyle(std::string key) {
    if (_fontHandles.count(key) == 1) {
        nk_style_set_font(_contexts.context, &_fontHandles.at(key)->handle);
    }
}

bool NuklearWrapper::HitGUI(Index pos) {
    for (auto&& rect : _windowRects) {
        if (rect.Contains(pos)) {
            return true;
        }
    }

    return false;
}
