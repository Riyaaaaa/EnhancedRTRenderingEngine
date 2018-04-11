#pragma once

#include "SpiralLibrary/GameSystem/SingletonBase.hpp"

#include "rapidjson/document.h"

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

protected:
    Vector4D _bgcolor;
};

