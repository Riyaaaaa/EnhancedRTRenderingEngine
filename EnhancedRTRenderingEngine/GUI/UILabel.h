#pragma once

#include <string>

#include "UIWidgets.h"

class UILabel : public UIWidget
{
public:
    UILabel(UIAlign align) :
        UIWidget(UIWidgetType::Label), 
        _align(align) {}

    virtual std::string Text() const = 0;

    UIAlign AlignType() const {
        return _align;
    }

protected:
    UIAlign _align;
};

class UIStaticLabel : public UILabel
{
public:
    UIStaticLabel(std::string text, UIAlign align) :
        UILabel(align),
        _text(text) {}

    std::string Text() const override {
        return _text;
    }

protected:
    std::string _text;
};

class UIDynamicLabel : public UILabel
{
public:
    UIDynamicLabel(std::function<std::string()> textGenerator, UIAlign align) :
        UILabel(align),
        _textGenerator(textGenerator) {}

    std::string Text() const override {
        return _textGenerator();
    }

protected:
    std::function<std::string()> _textGenerator;
};
