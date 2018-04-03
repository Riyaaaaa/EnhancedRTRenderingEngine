#pragma once

#include <vector>
#include <memory>

#include "Scene/MeshObject.h"
#include "Resource/ResourceHandle.h"
#include "RenderingContext.h"
#include "Constant/RenderTag.h"
#include "GraphicsInterface/GIDrawMesh.h"

class GIImmediateCommands;

class D3D11DrawElement
{
public:
    bool Draw(GIImmediateCommands* cmd, const GIDrawMesh& element);
};

