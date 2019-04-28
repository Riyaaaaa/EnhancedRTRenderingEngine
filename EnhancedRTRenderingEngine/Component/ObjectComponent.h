#pragma once

class Scene;
class SceneObject;

class ObjectComponent
{
public:
	virtual void Update(Scene* scene, SceneObject* object);
	virtual ~ObjectComponent() = default;
};

