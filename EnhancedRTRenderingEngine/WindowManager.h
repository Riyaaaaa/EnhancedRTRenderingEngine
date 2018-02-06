#pragma once
#include <windows.h>
#include <functional>
#include <unordered_map>
#include "SpiralLibrary/GameSystem/SingletonBase.hpp"
#include "Constant/InputKey.h"

class WindowManager : public libspiral::SingletonBase<WindowManager>
{
public:
	void RegisterWindow(HWND hWnd) { _hWnd = hWnd; }
	HWND GetActiveWindow() { return _hWnd;  }

	void RegisterListener(std::string key, const std::function<void(InputKey key)>& listener);
	bool ProcessInput(unsigned int umsg, WPARAM wParam, LPARAM lParam);
	void DispatchInputEvent(InputKey key);
protected:
	HWND _hWnd;

	std::unordered_map<std::string, std::function<void(InputKey key)>> pressedKeyListeners;
};

