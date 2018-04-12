#pragma once

#include <functional>
#include <string>

#include "GUIConstant.h"
#include "UIWidgets.h"

class UIButtonLabel : public UIWidget
{
public:
    UIButtonLabel(std::string text, std::function<void()> callback) : 
        UIWidget(UIWidgetType::ButtonLabel),
        _text(text),
        _callback(callback) {}


    void CallHandler() const {
        if (_callback) {
            _callback();
        }
    }

    std::string Text() const {
        return _text;
    }

protected:
    std::string _text;
    std::function<void()> _callback;
};

