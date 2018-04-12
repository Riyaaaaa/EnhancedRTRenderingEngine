#include "stdafx.h"
#include "NuklearFormatUtils.h"

#include "nuklear/nuklear.h"

#include "GUI/GUIConstant.h"

template<>
nk_text_alignment CastToNuklearFormat<nk_text_alignment, UIAlign>(UIAlign align) {
    switch (align) {
    case UIAlign::LEFT:
        return nk_text_alignment::NK_TEXT_LEFT;
    case UIAlign::CENTER:
        return nk_text_alignment::NK_TEXT_CENTERED;
    case UIAlign::RIGHT:
        return nk_text_alignment::NK_TEXT_RIGHT;
    }

    return nk_text_alignment::NK_TEXT_LEFT;
}