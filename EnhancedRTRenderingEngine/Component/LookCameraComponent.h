#pragma once

#include "ObjectComponent.h"

class LookCameraComponent : public ObjectComponent
{
public:
    void OnAttached(SceneObject* object) override;
	void Update(Scene* scene, SceneObject* object) override;
};

