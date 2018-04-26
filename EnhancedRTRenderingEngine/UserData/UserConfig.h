#pragma once

#include "SpiralLibrary/GameSystem/SingletonBase.hpp"

#include "rapidjson/document.h"

#include "Constant/RenderConfig.h"

#include "Structure/Structure.h"
#include "Structure/Vector.h"


class UserConfig : public libspiral::SingletonBase<UserConfig>
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

    void SwitchDisplayRayPaths() {
        _isDisplayRayPaths = !_isDisplayRayPaths;
    }

protected:
    bool _isDisplayRayPaths;
    RasterizerState _rasterizerType;
    MSAAQualityType _quality;
    Vector4D _bgcolor;
};

