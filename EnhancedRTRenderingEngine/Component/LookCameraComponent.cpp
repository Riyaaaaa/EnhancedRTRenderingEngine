#include "LookCameraComponent.h"

#include "Scene/Scene.h"
#include "Scene/SceneObject.h"

void LookCameraComponent::OnAttached(SceneObject* object)
{
    object->setAttribute(ObjectAttribute::Dynamic);
}

void LookCameraComponent::Update(Scene* scene, SceneObject* object)
{
	auto camera = scene->GetMainCamera();
	object->LookAt(camera.Position());

    scene->Dirty();
}
