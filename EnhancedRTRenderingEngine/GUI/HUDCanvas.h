#pragma once

#include <vector>

#include "UIWindow.h"

class HUDCanvas
{
public:
    const std::vector<UIWindow>& Windows() const {
        return _windows;
    }

    void AddWindow(const UIWindow& window) {
        _windows.push_back(window);
    }

protected:
    std::vector<UIWindow> _windows;
};

