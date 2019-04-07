#pragma once

#include "Scene/Scene.h"
#include "Scene/SceneObject.h"

class Component
{
public:
	virtual void Update(Scene* scene, SceneObject* object);
	Component();
	virtual ~Component();
};

