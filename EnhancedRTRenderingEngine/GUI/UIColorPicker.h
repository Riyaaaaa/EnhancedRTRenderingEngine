#pragma once

#include "Structure/Vector.h"

#include "GUIConstant.h"
#include "UIWidgets.h"

class UIColorPicker : public UIWidget
{
public:
    UIColorPicker(Vector4D* color, bool useAlpha) : 
        UIWidget(UIWidgetType::ColorPicker), 
        _color(color),
    _useAlpha(useAlpha) {}

    Vector4D* Color() const {
        return _color;
    }

    bool UseAlpha() const {
        return _useAlpha;
    }

protected:
    bool _useAlpha;
    Vector4D * _color;
};

