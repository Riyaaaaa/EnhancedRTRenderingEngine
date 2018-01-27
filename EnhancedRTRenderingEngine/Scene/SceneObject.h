#pragma once

#include "Structure/Structure.h"

#include <DirectXMath.h>

class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	DirectX::XMMATRIX* GetMatrixRef();

	void SetTransform(Transform trans);
	void SetRotation(Vector3D rot);
	void SetLocation(Vector3D lot);
	void SetScale(Vector3D scale);

protected:
	bool dirty;
	DirectX::XMMATRIX matrix;
	Transform transform;
};

