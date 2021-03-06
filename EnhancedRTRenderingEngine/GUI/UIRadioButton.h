#pragma once

#include <functional>
#include <string>

#include "UIWidgets.h"

#include "GUIConstant.h"

class UIRadioButton : public UIWidget
{
public:
    UIRadioButton(std::string text, std::function<bool()> condition, std::function<void()> callback) : 
        UIWidget(UIWidgetType::RadioButton),
        _text(text),
        _condition(condition),
        _callback(callback)
    {}

    bool Condition() {
        if (_condition) {
            return _condition();
        }
        return false;
    }

    void CallHandler() {
        if (_callback) {
            _callback();
        }
    }

    std::string Text() const {
        return _text;
    }

protected:
    std::string _text;
    std::function<bool()> _condition;
    std::function<void()> _callback;
};

