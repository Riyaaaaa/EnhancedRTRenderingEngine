#pragma once

#include <vector>

#include "Structure/Vector.h"
#include "Structure/Structure.h"

#include "GUIConstant.h"
#include "UIWidgets.h"
#include "UIRowLayout.h"

class UIComboColor : public UIWidget
{
public:
    UIComboColor(Size2D size, Vector4D* col) :
        UIWidget(UIWidgetType::ComboColor),
        _size(size),
        _col(col),
        _fitWindow(false){}

    UIComboColor(float height, Vector4D* col) :
        UIWidget(UIWidgetType::ComboColor),
        _size(Size2D(-1, height)),
        _col(col),
        _fitWindow(true) {}

    Size2D Size() const {
        return _size;
    }

    Vector4D Color() const {
        return *_col;
    }

    bool IsFitting() const {
        return _fitWindow;
    }

    const std::vector<UIRowLayout>& ContainWidgets() const {
        return _containWidgets;
    }

    void AddRow(const UIRowLayout& row) {
        _containWidgets.push_back(row);
    }

protected:
    bool _fitWindow;
    Size2D _size;
    Vector4D * _col;

    std::vector<UIRowLayout> _containWidgets;
};

