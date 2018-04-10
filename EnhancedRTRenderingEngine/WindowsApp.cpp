#include "stdafx.h"
#include "Windowsx.h"
#include "WindowsApp.h"
#include "Common/Common.h"

#include <iostream>

using namespace libspiral;

static Size FixClientSize(HWND hWnd, LONG sx, LONG sy);

void WindowsApp::RegisterWindow(HWND hWnd, Size size) {
    _hWnd = hWnd;
    _windowSize = size;

    FixClientSize(_hWnd, size.w, size.h);
}

bool WindowsApp::ProcessInput(unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        Index pos = Index( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
        handleKey = InputKey::LMOUSE;
        oldClickedPos = pos;
        DispatchInputEvent(InputEvent::PRESS, InputKey::LMOUSE, pos);
    }
        break;
    case WM_MOUSEMOVE: {
        if (handleKey != InputKey::None && oldClickedPos.x != -1) {
            Index pos = Index{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            DispathDragEvent(handleKey, oldClickedPos - pos, pos);
            oldClickedPos = pos;
        }
    }
        break;
    case WM_LBUTTONUP: {
        Index pos = Index( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
        handleKey = InputKey::None;
        oldClickedPos = Index{ -1, -1 };
        DispatchInputEvent(InputEvent::RELEASE, InputKey::LMOUSE, pos);
    }
        break;
    case WM_RBUTTONDOWN: {
        Index pos = Index( GET_X_LPARAM(lParam),  GET_Y_LPARAM(lParam) );
        handleKey = InputKey::RMOUSE;
        oldClickedPos = pos;
        DispatchInputEvent(InputEvent::PRESS, InputKey::RMOUSE, pos);
        break;
    }
        break;
    case WM_RBUTTONUP: {
        Index pos = Index( GET_X_LPARAM(lParam),  GET_Y_LPARAM(lParam) );
        handleKey = InputKey::None;
        oldClickedPos = Index{ -1, -1 };
        DispatchInputEvent(InputEvent::RELEASE, InputKey::RMOUSE, pos);
    }
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

void WindowsApp::RegisterDragListener(std::string key, const std::function<void(Index Delta, Index pos, InputKey key)>& listener) {
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

void WindowsApp::DispathDragEvent(InputKey key, Index Delta, Index pos) {
    for (auto && listener : dragListeners) {
        listener.second(Delta, pos, key);
    }
}

static Size FixClientSize(HWND hWnd, LONG sx, LONG sy)
{
    Size fixedSize;
    RECT rc1;
    RECT rc2;

    GetWindowRect(hWnd, &rc1);
    GetClientRect(hWnd, &rc2);

    fixedSize.w = ((rc1.right - rc1.left) - (rc2.right - rc2.left));
    fixedSize.h = ((rc1.bottom - rc1.top) - (rc2.bottom - rc2.top));
    sx += fixedSize.w;
    sy += fixedSize.h;
    SetWindowPos(hWnd, NULL, 0, 0, sx, sy, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));

    return fixedSize;
}
