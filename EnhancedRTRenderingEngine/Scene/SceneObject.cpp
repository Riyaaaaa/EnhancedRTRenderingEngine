
#include "SceneObject.h"

using namespace DirectX;

SceneObject::SceneObject()
{
    worldMatrixDirty = true;
    _transform.location = Vector3D{ 0,0,0 };
    _transform.rotation = Quaternion::FromEuler(0, 0, 0);
    _transform.scale = Vector3D{ 1,1,1 };
}


SceneObject::~SceneObject() = default;

void SceneObject::Update(Scene* scene)
{
    for (auto& component : _components) {
        component->Update(scene, this);
    }
}

void SceneObject::SetTransform(Transform trans) {
    _transform = trans;
    DirtyWorldMatrix();
}

void SceneObject::SetRotation(Vector3D rot) {
    _transform.rotation = Quaternion::FromEuler(rot.x, rot.y, rot.z);
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
									 XMMatrixRotationQuaternion(XMVectorSet(_transform.rotation.x, _transform.rotation.y, _transform.rotation.z, _transform.rotation.w))),
					XMMatrixTranslation(_transform.location.x, _transform.location.y, _transform.location.z));
        worldMatrixDirty = false;
    }
    return matrix;
}
