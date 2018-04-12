#pragma once

#include <type_traits>
#include <string>

#include "GUIConstant.h"
#include "UIWidgets.h"

template<class T>
class UINumericProperty : public UIWidget
{
public:
    UINumericProperty(std::string text, T min, T max, T step, T perPixel, T* prop) :
        _text(text),
        _min(min),
        _max(max),
        _step(step),
        _perPixel(perPixel),
        _property(prop) {
        Init<T>();
    }

    T* PropertyRef() const  {
        return _property;
    }

    T Min() const {
        return _min;
    }

    T Max() const {
        return _max;
    }

    T Step() const {
        return _step;
    }

    T PerPixel() const {
        return _perPixel;
    }

protected:
    template<class U>
    void Init();

    template<>
    void Init<int>() {
        _type = UIWidgetType::IntProperty;
    }

    template<>
    void Init<float>() {
        _type = UIWidgetType::FloatProperty;
    }

    std::string _text;
    T _min, _max, _step, _perPixel;
    T * _property;
};

