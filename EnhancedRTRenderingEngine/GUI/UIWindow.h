#pragma once

#include "UIRowLayout.h"

class UIWindow
{
public:
    void AddRow(const UIRowLayout& row) {
        _rows.push_back(row);
    }

protected:
    std::vector<UIRowLayout> _rows;
};

