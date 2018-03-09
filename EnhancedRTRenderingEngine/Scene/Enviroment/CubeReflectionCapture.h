#pragma once

#include "Scene/SceneObject.h"
#include "Structure/Structure.h"
#include "Resource/CubeTexture.h"

class CubeReflectionCapture : public SceneObject
{
public:
    CubeReflectionCapture(){}

protected:
    bool dirty;
    Size3D box;

    CubeTexture cubeTexture;
};

