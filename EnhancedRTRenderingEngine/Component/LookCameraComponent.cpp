#include "LookCameraComponent.h"

#include "Scene/Scene.h"
#include "Scene/SceneObject.h"

void LookCameraComponent::Update(Scene* scene, SceneObject* object)
{
	auto camera = scene->GetMainCamera();
	object->getTransform().LookAt(camera.Position());
}
