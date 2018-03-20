#include "stdafx.h"
#include "SceneObject.h"

using namespace DirectX;

SceneObject::SceneObject()
{
    dirty = true;
    _transform.location = Vector3D{ 0,0,0 };
    _transform.rotation = Vector3D{ 0,0,0 };
    _transform.scale = Vector3D{ 1,1,1 };
}


SceneObject::~SceneObject()
{
}

void SceneObject::SetTransform(Transform trans) {
    _transform = trans;
    dirty = true;
}

void SceneObject::SetRotation(Vector3D rot) {
    _transform.rotation = rot;
    dirty = true;
}

void SceneObject::SetLocation(Vector3D lot) {
    _transform.location = lot;
    dirty = true;
}

void SceneObject::SetScale(Vector3D scale) {
    _transform.scale = scale;
    dirty = true;
}

const XMMATRIX& SceneObject::GetMatrix() {
    if (dirty) {
        matrix = XMMatrixMultiply(
            XMMatrixMultiply(XMMatrixScaling(_transform.scale.x, _transform.scale.y, _transform.scale.z),
                XMMatrixRotationRollPitchYaw(_transform.rotation.x, _transform.rotation.y, _transform.rotation.z)),
            XMMatrixTranslation(_transform.location.x, _transform.location.y, _transform.location.z));
        dirty = false;
    }
    return matrix;
}
