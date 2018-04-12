#pragma once

#include "GUIConstant.h"

class UIWidget
{
public:
    UIWidgetType WidgetType() const {
        return _type;
    }

protected:
    UIWidget() {}
    UIWidget(UIWidgetType type) : _type(type) {}

    UIWidgetType _type;
};

