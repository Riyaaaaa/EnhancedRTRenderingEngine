#include "stdafx.h"
#include "SceneObject.h"

using namespace DirectX;

SceneObject::SceneObject()
{
	dirty = true;
	transform.location = Vector3D{ 0,0,0 };
	transform.rotation = Vector3D{ 0,0,0 };
	transform.scale = Vector3D{ 1,1,1 };
}


SceneObject::~SceneObject()
{
}

void SceneObject::SetTransform(Transform trans) {
	transform = trans;
	dirty = true;
}

void SceneObject::SetRotation(Vector3D rot) {
	transform.rotation = rot;
	dirty = true;
}

void SceneObject::SetLocation(Vector3D lot) {
	transform.location = lot;
	dirty = true;
}

void SceneObject::SetScale(Vector3D scale) {
	transform.scale = scale;
	dirty = true;
}

XMMATRIX* SceneObject::GetMatrixRef() {
	if (dirty) {
		matrix = XMMatrixMultiply(
			XMMatrixMultiply(XMMatrixTranslation(transform.location.x, transform.location.x, transform.location.x),
				XMMatrixRotationRollPitchYaw(transform.rotation.x, transform.rotation.y, transform.rotation.z)),
			XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z));
	}
	return &matrix;
}
