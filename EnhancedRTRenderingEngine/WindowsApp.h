#pragma once
#include <windows.h>
#include <functional>
#include <map>
#include "SpiralLibrary/GameSystem/SingletonBase.hpp"
#include "boost/optional.hpp"
#include "Constant/InputKey.h"
#include "Structure/Aliases.h"
#include "Structure/Structure.h"


class WindowsApp : public libspiral::SingletonBase<WindowsApp>
{
public:
    void RegisterWindow(HWND hWnd, Size2Dd size);
    HWND GetActiveWindow() { return _hWnd;  }
    Size2Dd GetWindowSize() { return _windowSize; }

    void RegisterPressListener(int priority, const std::function<bool(InputKey key, boost::optional<Index>)>& listener);
    void RegisterReleaseListener(int priority, const std::function<bool(InputKey key, boost::optional<Index>)>& listener);
    void RegisterDragListener(int priority, const std::function<bool(Index Delta, Index pos, InputKey key)>& listener);

    bool ProcessInput(unsigned int umsg, WPARAM wParam, LPARAM lParam);
    void DispatchInputEvent(InputEvent e, InputKey key, boost::optional<Index> pos);
    void DispathDragEvent(InputKey key, Index Delta, Index pos);

    void SetFPS(double fps) {
        _fps = fps;
    }

    double FPS() {
        return _fps;
    }

protected:
    HWND _hWnd;
    Size2Dd _windowSize;
    Index oldClickedPos;
    InputKey handleKey;

    double _fps = 0.0f;

    std::map<int, std::function<bool(InputKey key, boost::optional<Index>)>, std::greater<int>> pressedKeyListeners;
    std::map<int, std::function<bool(InputKey key, boost::optional<Index>)>, std::greater<int>> releasedKeyListeners;
    std::map<int, std::function<bool(Index Delta, Index pos, InputKey key)>, std::greater<int>> dragListeners;
};

