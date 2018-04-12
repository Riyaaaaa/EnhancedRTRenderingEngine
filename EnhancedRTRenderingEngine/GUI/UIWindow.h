#pragma once

#include <string>
#include "UIRowLayout.h"

#include "Structure/Structure.h"

class UIWindow
{
public:
    UIWindow(std::string title, Size2D size) : 
        _title(title),
        _size(size)
    {}

    std::string Title() const {
        return _title;
    }

    Size2D Size() const {
        return _size;
    }

    void AddRow(const UIRowLayout& row) {
        _rows.push_back(row);
    }

    const std::vector<UIRowLayout>& Rows() const {
        return _rows;
    }

protected:
    std::string _title;
    Size2D _size;
    std::vector<UIRowLayout> _rows;
};

