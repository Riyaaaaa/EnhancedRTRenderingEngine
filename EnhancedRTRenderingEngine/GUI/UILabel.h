#pragma once

#include <string>

#include "UIWidgets.h"

class UILabel : public UIWidget
{
public:
    UILabel(std::string text, UIAlign align) :
        UIWidget(UIWidgetType::Label), 
        _text(text),
        _align(align) {}

    const std::string& Text() const {
        return _text;
    }

    UIAlign AlignType() const {
        return _align;
    }

protected:
    UIAlign _align;
    std::string _text;
};

