#include "stdafx.h"

#include "GUIFactory.h"

#include "UserData/UserConfig.h"
#include "UIWidgets.h"

UIWindow GUIFactory::CreateGlobalSettingsWindow() {
    UIWindow window;
    auto* config = UserConfig::getInstance();

    {
        UIRowLayout row(LayoutAttribute::Static, 30, 80);
        row.AddWidget(std::make_shared<UIButtonLabel>("button", []() {}));
        window.AddRow(row);
    }
    
    {
        UIRowLayout row(LayoutAttribute::Dynamic, 60);
        row.AddWidget(std::make_shared<UIRadioButton>("Low Quality", 
            [=]() { return config->GetMSAAQuality() == MSAAQualityType::LOW_QUALITY; },
            [=]() { config->GetMSAAQuality() = MSAAQualityType::LOW_QUALITY; }
        ));
        row.AddWidget(std::make_shared<UIRadioButton>("High Quality",
            [=]() { return config->GetMSAAQuality() == MSAAQualityType::HIGH_QUALITY; },
            [=]() { config->GetMSAAQuality() = MSAAQualityType::HIGH_QUALITY; }
        ));
        window.AddRow(row);
    }

    {
        UIRowLayout row(LayoutAttribute::Dynamic, 20);
        row.AddWidget(std::make_shared<UILabel>("background:", UIAlign::LEFT));
        window.AddRow(row);
    }

    {
        UIRowLayout row(LayoutAttribute::Dynamic, 25);
        auto combo_color = std::make_shared<UIComboColor>(100.0f, &config->GetBGColor());

        UIRowLayout contain_row1(LayoutAttribute::Dynamic, 120);
        contain_row1.AddWidget(std::make_shared<UIColorPicker>(&config->GetBGColor(), true));
        combo_color->AddRow(contain_row1);

        UIRowLayout contain_row2(LayoutAttribute::Dynamic, 25);
        contain_row2.AddWidget(std::make_shared<UINumericProperty<float>>("#R:", 0, 1.0f, 0.01f, 0.005f, &config->GetBGColor().x));
        contain_row2.AddWidget(std::make_shared<UINumericProperty<float>>("#G:", 0, 1.0f, 0.01f, 0.005f, &config->GetBGColor().y));
        contain_row2.AddWidget(std::make_shared<UINumericProperty<float>>("#B:", 0, 1.0f, 0.01f, 0.005f, &config->GetBGColor().z));
        contain_row2.AddWidget(std::make_shared<UINumericProperty<float>>("#A:", 0, 1.0f, 0.01f, 0.005f, &config->GetBGColor().w));
        combo_color->AddRow(contain_row2);

        row.AddWidget(combo_color);

        window.AddRow(row);
    }

    return window;
}


