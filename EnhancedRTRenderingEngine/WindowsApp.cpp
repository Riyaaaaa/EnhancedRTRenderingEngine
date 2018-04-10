#include "stdafx.h"
#include "Windowsx.h"
#include "WindowsApp.h"
#include "Common/Common.h"

#include <iostream>

using namespace libspiral;

bool WindowsApp::ProcessInput(unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        Index pos = Index{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ERTREDebug(_T("pos: %f"), oldClickedPos.x);
        handleKey = InputKey::LMOUSE;
        oldClickedPos = pos;
        DispatchInputEvent(InputEvent::PRESS, InputKey::LMOUSE, pos);
    }
        break;
    case WM_MOUSEMOVE: {
        if (handleKey != InputKey::None && oldClickedPos.x != -1) {
            Index pos = Index{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            DispathDragEvent(handleKey, oldClickedPos - pos);
            oldClickedPos = pos;
        }
    }
        break;
    case WM_LBUTTONUP:
        handleKey = InputKey::None;
        oldClickedPos = Index{ -1, -1 };
        break;
    case WM_RBUTTONDOWN: {
        Index pos = Index{ GET_X_LPARAM(lParam),  GET_Y_LPARAM(lParam) };
        handleKey = InputKey::RMOUSE;
        oldClickedPos = pos;
        break;
    }
        break;
    case WM_RBUTTONUP:
        handleKey = InputKey::None;
        oldClickedPos = Index{ -1, -1 };
        break;
    case WM_CHAR:
        if (L'e' == (wchar_t)wParam) {
            DispatchInputEvent(InputEvent::PRESS, InputKey::Up, boost::none);
        }
        else if (L'q' == (wchar_t)wParam){
            DispatchInputEvent(InputEvent::PRESS, InputKey::Down, boost::none);
        }
        else if (L'w' == (wchar_t)wParam) {
            DispatchInputEvent(InputEvent::PRESS, InputKey::Forward, boost::none);
        }
        else if (L's' == (wchar_t)wParam) {
            DispatchInputEvent(InputEvent::PRESS, InputKey::Backward, boost::none);
        }
        else if (L'a' == (wchar_t)wParam) {
            DispatchInputEvent(InputEvent::PRESS, InputKey::Left, boost::none);
        }
        else if (L'd' == (wchar_t)wParam) {
            DispatchInputEvent(InputEvent::PRESS, InputKey::Right, boost::none);
        }
        return true;
    }

    return false;
}

void WindowsApp::RegisterPressListener(std::string key, const std::function<void(InputKey key, boost::optional<Index>)>& listener) {
    pressedKeyListeners.insert(std::make_pair(key, listener));
}

void WindowsApp::RegisterReleaseListener(std::string key, const std::function<void(InputKey key, boost::optional<Index>)>& listener) {
    releasedKeyListeners.insert(std::make_pair(key, listener));
}

void WindowsApp::RegisterDragListener(std::string key, const std::function<void(Index Delta, InputKey key)>& listener) {
    dragListeners.insert(std::make_pair(key, listener));
}

void WindowsApp::DispatchInputEvent(InputEvent e, InputKey key, boost::optional<Index> pos) {
    switch (e)
    {
    case InputEvent::PRESS:
        for (auto && listener : pressedKeyListeners) {
            listener.second(key, pos);
        }
        break;
    case InputEvent::RELEASE:
        for (auto && listener : releasedKeyListeners) {
            listener.second(key, pos);
        }
        break;
    default:
        break;
    }
}

void WindowsApp::DispathDragEvent(InputKey key, Index Delta) {
    for (auto && listener : dragListeners) {
        listener.second(Delta, key);
    }
}
