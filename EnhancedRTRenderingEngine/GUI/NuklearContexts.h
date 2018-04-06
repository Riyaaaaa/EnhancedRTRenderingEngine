#pragma once

struct nk_context;
struct nk_font_atlas;
struct nk_buffer;

struct NuklearContexts {
    struct nk_context* context;
    struct nk_font_atlas* atlas;
    struct nk_buffer* nkCmds;
};
