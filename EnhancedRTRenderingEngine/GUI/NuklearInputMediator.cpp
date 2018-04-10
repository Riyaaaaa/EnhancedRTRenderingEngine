#include "stdafx.h"
#include "NuklearInputMediator.h"

#include "nuklear/nuklear.h"

#include "Common/Common.h"

NuklearInputMediator::NuklearInputMediator(NuklearWrapper* nuklear) :
    _nuklear(nuklear)
{
}

void NuklearInputMediator::ProcessInputDown(InputKey key, boost::optional<Index> pos) {
    if (pos) {
        ERTREDebug(L"pos: %d %d\n", pos->x, pos->y);
    }

    switch (key)
    {
    case InputKey::None:
        break;
    case InputKey::Up:
        break;
    case InputKey::Down:
        break;
    case InputKey::LMOUSE:
        if (pos) {
            nk_input_button(_nuklear->Context(), NK_BUTTON_LEFT, pos->x, pos->y, 1);
        }
        break;
    case InputKey::RMOUSE:
        if (pos) {
            nk_input_button(_nuklear->Context(), NK_BUTTON_RIGHT, pos->x, pos->y, 1);
        }
        break;
    case InputKey::Forward:
        break;
    case InputKey::Backward:
        break;
    case InputKey::Right:
        break;
    case InputKey::Left:
        break;
    default:
        break;
    }
}

void NuklearInputMediator::ProcessInputUp(InputKey key, boost::optional<Index> pos) {
    switch (key)
    {
    case InputKey::None:
        break;
    case InputKey::Up:
        break;
    case InputKey::Down:
        break;
    case InputKey::LMOUSE:
        if (pos) {
            nk_input_button(_nuklear->Context(), NK_BUTTON_LEFT, pos->x, pos->y, 0);
        }
        break;
    case InputKey::RMOUSE:
        if (pos) {
            nk_input_button(_nuklear->Context(), NK_BUTTON_RIGHT, pos->x, pos->y, 0);
        }
        break;
    case InputKey::Forward:
        break;
    case InputKey::Backward:
        break;
    case InputKey::Right:
        break;
    case InputKey::Left:
        break;
    default:
        break;
    }
}

void NuklearInputMediator::ProcessInputMotion(Index Delta, Index pos, InputKey key) {
    ERTREDebug(L"move pos: %d %d\n", pos.x, pos.y);

    switch (key)
    {
    case InputKey::LMOUSE:
        nk_input_motion(_nuklear->Context(), pos.x, pos.y);
        break;
    case InputKey::RMOUSE:
        nk_input_motion(_nuklear->Context(), pos.x, pos.y);
        break;
    default:
        break;
    }
}

void NuklearInputMediator::BeginInput() {
    nk_input_begin(_nuklear->Context());
}

void NuklearInputMediator::EndInput() {
    nk_input_end(_nuklear->Context());
}
