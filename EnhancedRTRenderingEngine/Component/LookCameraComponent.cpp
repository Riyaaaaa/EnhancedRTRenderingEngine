#include "LookCameraComponent.h"

void LookCameraComponent::Update(Scene* scene, SceneObject* object)
{
	auto camera = scene->GetMainCamera();
	object->getTransform().LookAt(camera.Position());
}
