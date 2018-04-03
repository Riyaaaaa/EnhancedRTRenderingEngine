#pragma once

#include <vector>
#include <memory>

#include "D3D11TextureProxy.h"
#include "Scene/MeshObject.h"
#include "Resource/ResourceHandle.h"
#include "RenderingContext.h"
#include "Constant/RenderTag.h"
#include "GraphicsInterface/GIDrawMesh.h"

class D3D11DrawElement
{
public:
    bool Draw(GIImmediateCommands* cmd, const GIDrawMesh& element);
};

