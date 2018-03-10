#pragma once

#include "Scene/SceneObject.h"
#include "Structure/Structure.h"
#include "Resource/CubeTexture.h"

class GITexture;

class CubeReflectionCapture : public SceneObject
{
public:
    CubeReflectionCapture() : dirty(true) {}
    CubeReflectionCapture(float size);
    virtual void ReflectGraphics(GITexture* tex);

protected:
    bool dirty;
    Size3D box;
};

