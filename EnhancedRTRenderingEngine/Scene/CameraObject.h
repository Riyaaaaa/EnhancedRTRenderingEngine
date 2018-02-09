#pragma once

#include <DirectXMath.h>

class CameraObject
{
public:
    CameraObject();
    ~CameraObject();

    DirectX::XMVECTOR hEye;
    DirectX::XMVECTOR hAt;
    DirectX::XMVECTOR hUp;

    void SetProjParams(float fFOV, float fAspect, float fNearPlane, float fFarPlane);

    DirectX::XMMATRIX GetViewProjection();
    DirectX::XMMATRIX GetPerspectiveProjection();

    float m_fFOV;                 // Field of view
    float m_fAspect;              // Aspect ratio
    float m_fNearPlane;           // Near plane
    float m_fFarPlane;            // Far plane

    DirectX::XMMATRIX mView;              // View matrix 
    DirectX::XMMATRIX mProj;              // Projection matrix
};

