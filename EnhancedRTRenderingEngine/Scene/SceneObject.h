#pragma once

#include "ERTREObject.h"
#include "Structure/Structure.h"

#include <DirectXMath.h>

class SceneObject : public ERTREObject
{
public:
    SceneObject();
    ~SceneObject();

    const DirectX::XMMATRIX& GetMatrix();

    void SetTransform(Transform trans);
    void SetRotation(Vector3D rot);
    void SetLocation(Vector3D lot);
    void SetScale(Vector3D scale);

    Vector3D GetLocation() { return _transform.location; }

protected:
    virtual void DirtyWorldMatrix();

    bool worldMatrixDirty;
    DirectX::XMMATRIX matrix;
    Transform _transform;
    AABB _aabb;
};

