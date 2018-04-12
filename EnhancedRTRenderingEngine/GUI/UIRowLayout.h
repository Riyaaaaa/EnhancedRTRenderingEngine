#pragma once

#include <vector>
#include <memory>

#include "GUIConstant.h"

#include "UIWidget.h"

class UIRowLayout
{
public:
    UIRowLayout(LayoutAttribute att, float height, int itemWidth = 0);

    void SetCols(int cols) {
        _cols = cols;
    }

    void AddWidget(const std::shared_ptr<UIWidget>& widget) {
        _widgets.push_back(widget);
    }

    LayoutAttribute Attribute() const {
        return _attribute;
    }

    const std::vector<std::shared_ptr<UIWidget>>& Widgets() const {
        return _widgets;
    }

    float Height() const {
        return _height;
    }

    int ItemWidth() const {
        return _itemWidth;
    }

    int Cols() const {
        if (_cols == -1) {
            return static_cast<int>(_widgets.size());
        }

        return _cols;
    }

protected:
    float _height;
    int _itemWidth;
    int _cols;
    std::vector<std::shared_ptr<UIWidget>> _widgets;
    LayoutAttribute _attribute;
};

