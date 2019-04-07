
#include "CameraObject.h"

using namespace DirectX;

CameraObject::CameraObject()
{
    hEye = XMVectorSet(0.0f, 10.0f, -30.0f, 0.0f);
    hAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    hUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

CameraObject::~CameraObject()
{
}

Vector3D CameraObject::Position() {
	return Vector3D(XMVectorGetByIndex(hEye, 0), XMVectorGetByIndex(hEye, 1), XMVectorGetByIndex(hEye, 2));
}

void CameraObject::SetProjParams(float fFOV, float fAspect, float fNearPlane, float fFarPlane) {
    m_fFOV = fFOV;
    m_fAspect = fAspect;
    m_fNearPlane = fNearPlane;
    m_fFarPlane = fFarPlane;

    mProj = XMMatrixPerspectiveFovLH(fFOV, fAspect, fNearPlane, fFarPlane);
}

XMMATRIX CameraObject::GetViewProjection() const {
    return XMMatrixLookAtLH(hEye, hAt, hUp);
}

::XMMATRIX CameraObject::GetPerspectiveProjection() const {
    return mProj;
}
