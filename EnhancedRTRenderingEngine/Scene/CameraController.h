#pragma once

#include "CameraObject.h"
#include "Constant/InputKey.h"
#include "Structure/Structure.h"

class CameraController
{
public:
	CameraController(CameraObject* camera);

	void ControllCamera(InputKey key);
	void ControllCamera(Vector2D Delta, InputKey key);

	~CameraController();

protected:
	CameraObject* _camera;
};

