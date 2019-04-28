#pragma once

#include "ObjectComponent.h"

class LookCameraComponent : public ObjectComponent
{
public:
	virtual void Update(Scene* scene, SceneObject* object) override;
};

