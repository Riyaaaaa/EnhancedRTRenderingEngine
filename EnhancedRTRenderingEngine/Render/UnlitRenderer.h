#pragma once

#include <memory>

#include "Scene/Scene.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class UnlitRenderer
{
public:
    template<class VertType>
    void render(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, std::vector<MeshObject<VertType>>& meshes);
};

