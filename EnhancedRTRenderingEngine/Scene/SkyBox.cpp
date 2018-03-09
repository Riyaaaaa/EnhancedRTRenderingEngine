#include "stdafx.h"
#include "SkyBox.h"

#include "Mesh/Mesh3DModel.h"
#include "Resource/ResourceLoader.h"

#include <array>

SkyBox::SkyBox(const std::string& skyboxTextureDirectory)
{
    auto box = ResourceLoader::LoadDXModel("SkyBox");
    _mesh = std::make_shared<Mesh3DModel>(box());
    int i = 0;
    for (auto&& key : {"posx.png", "negx.png", "posy.png", "negy.png", "posz.png", "negz.png"}) {
        _materials.emplace_back(MaterialParameters{"LightingVertexShader", "UnlitCubeTexturePixelShader", ("SkyBox\\" + skyboxTextureDirectory + '\\' + key).c_str(), 0.0f, 0.0f});
        i++;
    }

    _context = RenderingContext{ CreateVertexLayout < typename Mesh3DModel::Type >(), VertexPrimitiveType::TRIANGLELIST };

    transform.location = Vector3D(0.0f, 0.0f, 0.0f);
    transform.scale = Vector3D(100.0f, 100.0f, 100.0f);
}
