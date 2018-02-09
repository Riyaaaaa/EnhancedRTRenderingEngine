#pragma once
#include <windows.h>
#include <functional>
#include <unordered_map>
#include "SpiralLibrary/GameSystem/SingletonBase.hpp"
#include "Constant/InputKey.h"
#include "Structure/Structure.h"


class WindowManager : public libspiral::SingletonBase<WindowManager>
{
public:
    void RegisterWindow(HWND hWnd) { _hWnd = hWnd; }
    HWND GetActiveWindow() { return _hWnd;  }

    void RegisterPressListener(std::string key, const std::function<void(InputKey key)>& listener);
    void RegisterReleaseListener(std::string key, const std::function<void(InputKey key)>& listener);
    void RegisterDragListener(std::string key, const std::function<void(Vector2D Delta, InputKey key)>& listener);

    bool ProcessInput(unsigned int umsg, WPARAM wParam, LPARAM lParam);
    void DispatchInputEvent(InputEvent e, InputKey key);
    void DispathDragEvent(InputKey key, Vector2D Delta);
protected:
    HWND _hWnd;
    Vector2D oldClickedPos;
    InputKey handleKey;

    std::unordered_map<std::string, std::function<void(InputKey key)>> pressedKeyListeners;
    std::unordered_map<std::string, std::function<void(InputKey key)>> releasedKeyListeners;
    std::unordered_map<std::string, std::function<void(Vector2D Delta, InputKey key)>> dragListeners;
};

