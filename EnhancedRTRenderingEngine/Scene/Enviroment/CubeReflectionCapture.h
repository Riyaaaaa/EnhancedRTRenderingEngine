#pragma once

#include "Scene/SceneObject.h"
#include "Structure/Structure.h"
#include "Resource/TextureCube.h"

class GITextureProxy;

class CubeReflectionCapture : public SceneObject
{
public:
    CubeReflectionCapture() : dirty(true) {}
    CubeReflectionCapture(float size);
    virtual void SetupTexture(GITextureProxy* tex);
    virtual bool Contains(Vector3D pos);
    virtual float PrecisionSize() { return box.w; }

protected:
    bool dirty;
    Size3D box;
};

