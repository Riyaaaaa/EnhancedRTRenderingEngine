#pragma once

#include "Scene/Scene.h"

#include "Structure/Primitive.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class D3D11LineRenderer
{
public:
    void render(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, const std::vector<Line>& lines);
};

