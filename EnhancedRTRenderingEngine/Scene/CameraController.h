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

    bool ControllCamera(InputKey key, boost::optional<Index> pos);
    bool ControllCamera(Index Delta, Index pos, InputKey key);

    ~CameraController();

protected:
    CameraObject* _camera;
};

