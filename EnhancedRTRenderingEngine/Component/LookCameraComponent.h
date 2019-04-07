#pragma once

#include "Component.h"

class LookCameraComponent : public Component
{
public:
	virtual void Update(Scene* scene, SceneObject* object) override;
};

