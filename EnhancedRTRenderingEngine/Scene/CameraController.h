#pragma once

#include <boost/optional.hpp>

#include "CameraObject.h"
#include "Constant/InputKey.h"
#include "Structure/Aliases.h"
#include "Structure/Structure.h"

class CameraController
{
public:
    CameraController(CameraObject* camera);

    void ControllCamera(InputKey key, boost::optional<Index> pos);
    void ControllCamera(Index Delta, Index pos, InputKey key);

    ~CameraController();

protected:
    CameraObject* _camera;
};

