#pragma once

struct nk_context;
struct nk_font_atlas;
struct nk_buffer;
struct nk_draw_null_texture;

struct NuklearContexts {
    nk_context* context;
    nk_font_atlas* atlas;
    nk_buffer* nkCmds;
    nk_draw_null_texture* null;
};
