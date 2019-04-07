
#include "SceneObject.h"

using namespace DirectX;

SceneObject::SceneObject()
{
    worldMatrixDirty = true;
    _transform.location = Vector3D{ 0,0,0 };
    _transform.rotation = Vector3D{ 0,0,0 };
    _transform.scale = Vector3D{ 1,1,1 };
}


SceneObject::~SceneObject()
{
}

void SceneObject::Update(Scene* scene)
{

}

void SceneObject::SetTransform(Transform trans) {
    _transform = trans;
    DirtyWorldMatrix();
}

void SceneObject::SetRotation(Vector3D rot) {
    _transform.rotation = rot;
    DirtyWorldMatrix();
}

void SceneObject::SetLocation(Vector3D lot) {
    _transform.location = lot;
    DirtyWorldMatrix();
}

void SceneObject::SetScale(Vector3D scale) {
    _transform.scale = scale;
    DirtyWorldMatrix();
}

void SceneObject::DirtyWorldMatrix() {
    worldMatrixDirty = true;
}

const XMMATRIX& SceneObject::GetMatrix() {
    if (worldMatrixDirty) {
        matrix = XMMatrixMultiply(
            XMMatrixMultiply(XMMatrixScaling(_transform.scale.x, _transform.scale.y, _transform.scale.z),
                XMMatrixRotationRollPitchYaw(_transform.rotation.x, _transform.rotation.y, _transform.rotation.z)),
            XMMatrixTranslation(_transform.location.x, _transform.location.y, _transform.location.z));
        worldMatrixDirty = false;
    }
    return matrix;
}
