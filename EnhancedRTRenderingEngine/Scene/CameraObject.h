#pragma once

#include <DirectXMath.h>

#include "Structure/Vector.h"

class CameraObject
{
public:
    CameraObject();
    ~CameraObject();

    DirectX::XMVECTOR hEye;
    DirectX::XMVECTOR hAt;
    DirectX::XMVECTOR hUp;

	Vector3D Position();

    void SetProjParams(float fFOV, float fAspect, float fNearPlane, float fFarPlane);

    DirectX::XMMATRIX GetViewProjection() const;
    DirectX::XMMATRIX GetPerspectiveProjection() const;

    float m_fFOV;                 // Field of view
    float m_fAspect;              // Aspect ratio
    float m_fNearPlane;           // Near plane
    float m_fFarPlane;            // Far plane

    DirectX::XMMATRIX mView;              // View matrix 
    DirectX::XMMATRIX mProj;              // Projection matrix
};

