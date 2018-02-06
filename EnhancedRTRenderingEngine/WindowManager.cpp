#include "stdafx.h"
#include "WindowManager.h"

#include "SpiralLibrary/Function/MaybeCall.hpp"

using namespace libspiral;

bool WindowManager::ProcessInput(unsigned int uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case  WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case  WM_MBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case  WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case  WM_XBUTTONDOWN:
	case  WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
	case  WM_MOUSEWHEEL:
		break;

	case WM_CHAR:
		if (L'e' == (wchar_t)wParam) {
			DispatchInputEvent(InputKey::Up);
		}
		else if (L'q' == (wchar_t)wParam){
			DispatchInputEvent(InputKey::Down);
		}
		return true;
	}

	return false;
}

void WindowManager::RegisterListener(std::string key, const std::function<void(InputKey key)>& listener) {
	pressedKeyListeners.insert(std::make_pair(key, listener));
}

void WindowManager::DispatchInputEvent(InputKey key) {
	for (auto && listener : pressedKeyListeners) {
		listener.second(key);
	}
}
