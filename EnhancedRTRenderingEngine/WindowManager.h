#pragma once

#include "SpiralLibrary/GameSystem/SingletonBase.hpp"

class WindowManager : public libspiral::SingletonBase<WindowManager>
{
public:
	void registerWindow(HWND hWnd) { _hWnd = hWnd; }
	HWND getActiveWindow() { return _hWnd;  }

protected:
	HWND _hWnd;
};

