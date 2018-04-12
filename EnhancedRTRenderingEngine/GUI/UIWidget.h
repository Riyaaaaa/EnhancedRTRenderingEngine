#pragma once

#include "GUIConstant.h"

class UIWidget
{
public:

protected:
    UIWidget() {}
    UIWidget(UIWidgetType type) : _type(type) {}

    UIWidgetType _type;
};

