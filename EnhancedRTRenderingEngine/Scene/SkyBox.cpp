#include "stdafx.h"
#include "SkyBox.h"

#include "Mesh/Mesh3DModel.h"
#include "Resource/ResourceLoader.h"

#include <array>

SkyBox::SkyBox(const std::string& skyboxTextureDirectory)
{
    auto box = ResourceLoader::LoadDXModel("reversed_box");
    _mesh = std::make_shared<Mesh3DModel>(box());
    int i = 0;
    std::vector<Texture2D> textures(6);
    for (auto&& key : {"posx.png", "negx.png", "posy.png", "negy.png", "posz.png", "negz.png"}) {
        ResourceLoader::LoadTexture(("SkyBox\\" + skyboxTextureDirectory + '\\' + key).c_str(), textures[i]);
        i++;
    }

    _materials.emplace_back(MaterialParameters{ "LightingVertexShader", "UnlitCubeTexturePixelShader", "", Vector3D(), 0.0f, 0.0f });
    _materials.back().cubeTexture = TextureCube(textures);
    _materials.back().type = TextureType::TextureCube;
    _materials.back().shadingType = ShadingType::Unlit;

    _transform.location = Vector3D(0.0f, 0.0f, 0.0f);
    _transform.scale = Vector3D(100.0f, 100.0f, 100.0f);

    _isPhysicallyObject = false;
}

const TextureCube& SkyBox::GetCubeTextureResource() {
    return _materials[0].cubeTexture;
}
