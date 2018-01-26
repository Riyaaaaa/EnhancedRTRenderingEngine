#include "stdafx.h"
#include "CameraObject.h"

using namespace DirectX;

CameraObject::CameraObject()
{
	hEye = XMVectorSet(0.0f, 0.0f, -2.0f, 0.0f);
	hAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	hUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}


CameraObject::~CameraObject()
{
}

void CameraObject::SetProjParams(float fFOV, float fAspect, float fNearPlane, float fFarPlane) {
	m_fFOV = fFOV;
	m_fAspect = fAspect;
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;

	m_mProj = XMMatrixPerspectiveFovLH(fFOV, fAspect, fNearPlane, fFarPlane);
}

XMMATRIX CameraObject::CreatePerspectiveProjection() {
	return XMMatrixLookAtLH(hEye, hAt, hUp);
}