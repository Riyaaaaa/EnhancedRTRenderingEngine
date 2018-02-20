#include "stdafx.h"
#include "CameraController.h"

#include "WindowManager.h"

#include <cmath>

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
    auto dir = _camera->hAt - _camera->hEye;
    dir = XMVector3Normalize(dir);
    Vector3D norDir{ XMVectorGetByIndex(dir, 0),XMVectorGetByIndex(dir, 1), XMVectorGetByIndex(dir, 2),};
    float verticalXZ_z = 1 / std::sqrt(std::powf(norDir.z, 2) / std::powf(norDir.x, 2) + 1);
    float verticalXZ_x = std::sqrt(1 - std::pow(verticalXZ_z, 2));

    switch (key) {
    case InputKey::Up:
        _camera->hEye += XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
        _camera->hAt += XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
        break;
    case InputKey::Down:
        _camera->hEye += XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
        _camera->hAt += XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
        break;
    case InputKey::Forward:
        _camera->hEye += XMVectorSet(norDir.x / 5, 0.0f, norDir.z / 5, 0.0f);
        _camera->hAt += XMVectorSet(norDir.x / 5, 0.0f, norDir.z / 5, 0.0f);
        break;
    case InputKey::Backward:
        _camera->hEye -= XMVectorSet(norDir.x / 5, 0.0f, norDir.z / 5, 0.0f);
        _camera->hAt -= XMVectorSet(norDir.x / 5, 0.0f, norDir.z / 5, 0.0f);
        break;
    case InputKey::Right:
        _camera->hEye += XMVectorSet(verticalXZ_x / 5, 0.0f, verticalXZ_z / 5, 0.0f);
        _camera->hAt += XMVectorSet(verticalXZ_x / 5, 0.0f, verticalXZ_z / 5, 0.0f);
        break;
    case InputKey::Left:
        _camera->hEye -= XMVectorSet(verticalXZ_x / 5, 0.0f, verticalXZ_z / 5, 0.0f);
        _camera->hAt -= XMVectorSet(verticalXZ_x / 5, 0.0f, verticalXZ_z / 5, 0.0f);
        break;
    }
}

void CameraController::ControllCamera(Vector2D Delta, InputKey key) {
    switch (key) {
    case InputKey::LMOUSE:
    case InputKey::RMOUSE:
        auto yAxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        auto axis = XMVector3Normalize(XMVector3Cross(_camera->hEye, _camera->hUp));
        XMVECTOR qt_xz = XMQuaternionRotationAxis(axis, XMConvertToRadians(Delta.y / 5.0f));
        XMVECTOR qt_y = XMQuaternionRotationAxis(yAxis, XMConvertToRadians(Delta.x / 5.0f));
        XMVECTOR qt = XMQuaternionMultiply(qt_xz, qt_y);

        _camera->hEye = XMVector3Rotate(_camera->hEye, qt);
        _camera->hUp = XMVector3Rotate(_camera->hUp, qt);
        break;
    }
}
