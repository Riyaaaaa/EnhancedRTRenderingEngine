#include "Billboard.h"

#include "Component/LookCameraComponent.h"
#include "Mesh/Primitive/Primitives.h"
#include "Resource/ResourceLoader.h"

#include <array>

Billboard::Billboard(const std::string& textureName)
	: MeshObject(std::make_shared<PrimitiveMesh::Square<MainVertex>>(Size2D(1, 1)))
{
	int i = 0;
	Texture2D texture;
	ResourceLoader::LoadTexture(textureName, texture);

	_materials.emplace_back(MaterialParameters{ "LightingVertexShader", "UnlitCubeTexturePixelShader", "", Vector3D(), 0.0f, 0.0f });
	_materials.back().texture = texture;
	_materials.back().type = TextureType::Texture2D;
	_materials.back().shadingType = ShadingType::BasePass;

	_transform.location = Vector3D(0.0f, 0.0f, 0.0f);
	_transform.scale = Vector3D(1.0f, 1.0f, 1.0f);
    _transform.rotation = Quaternion{};

    _components.push_back(std::make_unique<LookCameraComponent>());
}
