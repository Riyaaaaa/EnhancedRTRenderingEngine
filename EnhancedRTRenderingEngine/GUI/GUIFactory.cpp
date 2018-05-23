#include "stdafx.h"

#include "GUIFactory.h"
#include "UserData/UserConfig.h"
#include "UIWidgets.h"

#include "WindowsApp.h"

UIWindow GUIFactory::CreateGlobalSettingsWindow() {
    UIWindow window("Settings", Size2Dd(400, 400));
    auto* config = UserConfig::getInstance();

    {
        UIRowLayout row(LayoutAttribute::Dynamic, 20);
        row.AddWidget(std::make_shared<UIStaticLabel>("visibles", UIAlign::LEFT));
        window.AddRow(row);
    }

    {
        UIRowLayout row(LayoutAttribute::Dynamic, 30);
        row.AddWidget(std::make_shared<UIButtonLabel>("rays", [config]() {
            config->SwitchDisplayRayPaths();
        }));
        row.AddWidget(std::make_shared<UIButtonLabel>("indirect lights", [config]() {
            config->SwitchVisbleIndirectLights();
        }));
        row.AddWidget(std::make_shared<UIButtonLabel>("reflections", [config]() {
            config->SwitchVisibleReflections();
        }));
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
        UIRowLayout row(LayoutAttribute::Dynamic, 60);
        row.AddWidget(std::make_shared<UIRadioButton>("Lighting",
            [=]() { return config->RasterizerType() == RasterizerState::Default; },
            [=]() { config->RasterizerType() = RasterizerState::Default; }
        ));
        row.AddWidget(std::make_shared<UIRadioButton>("Wireframe",
            [=]() { return config->RasterizerType() == RasterizerState::WireFrame; },
            [=]() { config->RasterizerType() = RasterizerState::WireFrame; }
        ));
        window.AddRow(row);
    }

    {
        UIRowLayout row(LayoutAttribute::Dynamic, 20);
        row.AddWidget(std::make_shared<UIStaticLabel>("background:", UIAlign::LEFT));
        window.AddRow(row);
    }

    {
        UIRowLayout row(LayoutAttribute::Dynamic, 25);
        auto combo_color = std::make_shared<UIComboColor>(400.0f, &config->GetBGColor());

        UIRowLayout contain_row1(LayoutAttribute::Dynamic, 120);
        contain_row1.AddWidget(std::make_shared<UIColorPicker>(&config->GetBGColor(), true));
        combo_color->AddRow(contain_row1);

        UIRowLayout contain_row2(LayoutAttribute::Dynamic, 25);
        contain_row2.AddWidget(std::make_shared<UINumericProperty<float>>("#R:", 0.0f, 1.0f, 0.01f, 0.005f, &config->GetBGColor().x));
        contain_row2.AddWidget(std::make_shared<UINumericProperty<float>>("#G:", 0.0f, 1.0f, 0.01f, 0.005f, &config->GetBGColor().y));
        contain_row2.AddWidget(std::make_shared<UINumericProperty<float>>("#B:", 0.0f, 1.0f, 0.01f, 0.005f, &config->GetBGColor().z));
        contain_row2.AddWidget(std::make_shared<UINumericProperty<float>>("#A:", 0.0f, 1.0f, 0.01f, 0.005f, &config->GetBGColor().w));
        contain_row2.SetCols(1);
        combo_color->AddRow(contain_row2);

        row.AddWidget(combo_color);

        window.AddRow(row);
    }

    return window;
}

UIWindow GUIFactory::CreateApplicationMonitor() {
    UIWindow window("Monitor", Size2D(200.0f, 100.0f));

    UIRowLayout row(LayoutAttribute::Dynamic, 50);
    row.AddWidget(std::make_shared<UIDynamicLabel>(
        []() { return std::to_string(WindowsApp::getInstance()->FPS()) + " fps";  },
        UIAlign::CENTER));
    window.AddRow(row);

    return window;
}

