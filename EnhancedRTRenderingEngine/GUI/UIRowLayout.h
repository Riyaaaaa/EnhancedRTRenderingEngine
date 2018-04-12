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

    LayoutAttribute Attribute() {
        return _attribute;
    }

protected:
    float _height;
    int _itemWidth;
    std::vector<std::shared_ptr<UIWidget>> _widgets;
    LayoutAttribute _attribute;
};

