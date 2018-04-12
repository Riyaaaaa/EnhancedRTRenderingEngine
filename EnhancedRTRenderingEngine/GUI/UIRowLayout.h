#pragma once

#include <vector>
#include <memory>

#include "GUIConstant.h"

#include "UIWidget.h"

class UIRowLayout
{
public:
    UIRowLayout(LayoutAttribute att, float height, int itemWidth = 0);

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

protected:
    float _height;
    int _itemWidth;
    std::vector<std::shared_ptr<UIWidget>> _widgets;
    LayoutAttribute _attribute;
};

