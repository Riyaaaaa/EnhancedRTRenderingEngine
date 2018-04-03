#pragma once

#include "Scene/Scene.h"

#include "Structure/Primitive.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class LineRenderer
{
public:
    static void render(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, const std::vector<Line>& lines);
};

