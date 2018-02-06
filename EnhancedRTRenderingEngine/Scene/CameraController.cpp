#include "stdafx.h"
#include "CameraController.h"

#include "WindowManager.h"

using namespace DirectX;

CameraController::CameraController(CameraObject* camera)
{
	_camera = camera;
	WindowManager::getInstance()->RegisterPressListener("CameraController", std::bind<void (CameraController::*)(InputKey)>(&CameraController::ControllCamera, this, std::placeholders::_1));
	WindowManager::getInstance()->RegisterDragListener("CameraController", std::bind<void (CameraController::*)(Vector2D, InputKey)>(&CameraController::ControllCamera, this, std::placeholders::_1, std::placeholders::_2));
}


CameraController::~CameraController()
{
}

void CameraController::ControllCamera(InputKey key) {
	switch (key) {
	case InputKey::Up:
		_camera->hEye += XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
		_camera->hAt += XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
		break;
	case InputKey::Down:
		_camera->hEye += XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
		_camera->hAt += XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
		break;
	}
}

void CameraController::ControllCamera(Vector2D Delta, InputKey key) {
	switch (key) {
	case InputKey::LMOUSE:
		_camera->hAt += XMVectorSet(Delta.x / 100, Delta.y / 100, 0.0f, 0.0f);
		break;
	case InputKey::RMOUSE:
		_camera->hAt += XMVectorSet(Delta.x / 100, Delta.y / 100, 0.0f, 0.0f);
		break;
	}
}
