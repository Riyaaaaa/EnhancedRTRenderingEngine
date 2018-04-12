#pragma once

#include <string>
#include "UIRowLayout.h"

class UIWindow
{
public:
    std::string Title() const {
        return _title;
    }

    void AddRow(const UIRowLayout& row) {
        _rows.push_back(row);
    }

    const std::vector<UIRowLayout>& Rows() const {
        return _rows;
    }

protected:
    std::string _title;
    std::vector<UIRowLayout> _rows;
};

