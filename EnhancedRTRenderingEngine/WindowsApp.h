#pragma once
#include <windows.h>
#include <functional>
#include <unordered_map>
#include "SpiralLibrary/GameSystem/SingletonBase.hpp"
#include "boost/optional.hpp"
#include "Constant/InputKey.h"
#include "Structure/Aliases.h"
#include "Structure/Structure.h"


class WindowsApp : public libspiral::SingletonBase<WindowsApp>
{
public:
    void RegisterWindow(HWND hWnd, Size size);
    HWND GetActiveWindow() { return _hWnd;  }
    Size GetWindowSize() { return _windowSize; }

    void RegisterPressListener(std::string key, const std::function<void(InputKey key, boost::optional<Index>)>& listener);
    void RegisterReleaseListener(std::string key, const std::function<void(InputKey key, boost::optional<Index>)>& listener);
    void RegisterDragListener(std::string key, const std::function<void(Index Delta, Index pos, InputKey key)>& listener);

    bool ProcessInput(unsigned int umsg, WPARAM wParam, LPARAM lParam);
    void DispatchInputEvent(InputEvent e, InputKey key, boost::optional<Index> pos);
    void DispathDragEvent(InputKey key, Index Delta, Index pos);
protected:
    HWND _hWnd;
    Size _windowSize;
    Index oldClickedPos;
    InputKey handleKey;

    std::unordered_map<std::string, std::function<void(InputKey key, boost::optional<Index>)>> pressedKeyListeners;
    std::unordered_map<std::string, std::function<void(InputKey key, boost::optional<Index>)>> releasedKeyListeners;
    std::unordered_map<std::string, std::function<void(Index Delta, Index pos, InputKey key)>> dragListeners;
};

