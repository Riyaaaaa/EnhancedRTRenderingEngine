#pragma once

#include "Scene/SceneObject.h"
#include "Structure/Structure.h"
#include "Resource/TextureCube.h"
#include "GraphicsInterface/GITextureProxy.h"

class CubeReflectionCapture : public SceneObject
{
public:
    CubeReflectionCapture() : dirty(true) {}
    CubeReflectionCapture(unsigned int size);
    virtual void SetupTexture(GIImmediateCommands* cmd, GITextureProxy tex);
    virtual bool Contains(Vector3D pos);
    virtual unsigned int PrecisionSize() { return box.w; }

    virtual bool Dirty() { return dirty; }

protected:
    bool dirty;
    Size3Dd box;
};

