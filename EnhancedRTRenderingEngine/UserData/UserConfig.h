#pragma once

#include "SpiralLibrary/GameSystem/SingletonBase.hpp"

#include "rapidjson/document.h"

#include "Constant/RenderConfig.h"

#include "Structure/Structure.h"
#include "Structure/Vector.h"

#include "System/Observer.h"

#include "SettingsEvent.h"


class UserConfig : public libspiral::SingletonBase<UserConfig>, public ERTRESystem::Subject<UserConfigEvent>
{
public:
    UserConfig();

    bool LoadAllConfigFromDocument(rapidjson::Document* doc);

    void Flush();

    Vector4D& GetBGColor() {
        return _bgcolor;
    }

    MSAAQualityType& GetMSAAQuality() {
        return _quality;
    }

    RasterizerState& RasterizerType() {
        return _rasterizerType;
    }

    bool IsDisplayRayPaths() const {
        return _isDisplayRayPaths;
    }

    bool VisibleIndirectLights() const {
        return _visibleIndirectLights;
    }

    bool VisibleReflections() const {
        return _visibleReflections;
    }

    void SwitchDisplayRayPaths() {
        _isDisplayRayPaths = !_isDisplayRayPaths;        
    }

    void SwitchVisbleIndirectLights() {
        _visibleIndirectLights = !_visibleIndirectLights;
        Notify(UserConfigEvent::ChangedLightMapSetting);
    }

    void SwitchVisibleReflections() {
        _visibleReflections = !_visibleReflections;
        Notify(UserConfigEvent::ChangedReflectionSetting);
    }

protected:
    bool _isDisplayRayPaths;
    bool _visibleIndirectLights;
    bool _visibleReflections;
    RasterizerState _rasterizerType;
    MSAAQualityType _quality;
    Vector4D _bgcolor;
};

