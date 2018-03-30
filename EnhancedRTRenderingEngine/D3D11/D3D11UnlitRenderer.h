#pragma once

#include <memory>

#include "D3D11RenderView.h"
#include "Scene/Scene.h"

#include "GraphicsInterface/GIImmediateCommands.h"

class D3D11UnlitRenderer
{
public:
    template<class VertType>
    void render(GIImmediateCommands* cmd, const CameraObject& camera, std::vector<MeshObject<VertType>>& meshes);
};

