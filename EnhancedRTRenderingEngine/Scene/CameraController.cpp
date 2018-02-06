#include "stdafx.h"
#include "CameraController.h"

#include "WindowManager.h"

using namespace DirectX;

CameraController::CameraController(CameraObject* camera)
{
	_camera = camera;
	WindowManager::getInstance()->RegisterListener("CameraController", std::bind(&CameraController::ControllCamera, this, std::placeholders::_1));
}


CameraController::~CameraController()
{
}

void CameraController::ControllCamera(InputKey key) {
	switch (key) {
	case InputKey::Up:
		_camera->hEye += XMVectorSet(0, 0.1, 0, 0);
		_camera->hAt += XMVectorSet(0, 0.1, 0, 0);
		break;
	case InputKey::Down:
		_camera->hEye += XMVectorSet(0, -0.1, 0, 0);
		_camera->hAt += XMVectorSet(0, -0.1, 0, 0);
		break;
	}
}
