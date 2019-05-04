#pragma once

#include "ERTREObject.h"

#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "Component/ObjectComponent.h"
#include "Structure/Structure.h"

class Scene;

class SceneObject : public ERTREObject
{
public:
    SceneObject();
    virtual ~SceneObject();

	virtual void Update(Scene* scene);

    virtual const DirectX::XMMATRIX& GetMatrix();

    void SetTransform(Transform trans);
    void SetRotation(Vector3D rot);
    void SetLocation(Vector3D lot);
    void SetScale(Vector3D scale);

    Vector3D GetLocation() { return _transform.location; }

    void LookAt(const Vector3D& target);

    template<class Component>
    void AddComponent() {
        auto component = std::make_unique<Component>();
        component->OnAttached(this);
        _components.emplace_back(std::move(component));
    }

protected:
    virtual void DirtyWorldMatrix();

    bool worldMatrixDirty;
    DirectX::XMMATRIX matrix;
    Transform _transform;
    AABB _aabb;

    std::vector<std::unique_ptr<ObjectComponent>> _components;
};

