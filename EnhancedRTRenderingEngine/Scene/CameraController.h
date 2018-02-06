#pragma once

#include "CameraObject.h"
#include "Constant/InputKey.h"

class CameraController
{
public:
	CameraController(CameraObject* camera);

	void ControllCamera(InputKey key);

	~CameraController();

protected:
	CameraObject* _camera;
};

