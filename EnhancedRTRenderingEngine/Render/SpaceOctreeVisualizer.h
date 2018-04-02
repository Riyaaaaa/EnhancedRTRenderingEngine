#pragma once

#include "Scene/MeshObject.h"
#include "Algorithms/SpaceOctree.h"

#include "GraphicsInterface/GIRenderView.h"
#include "GraphicsInterface/GIImmediateCommands.h"

class Scene;

class SpaceOctreeVisualizer
{
public:
    void Initialize(const SpaceOctree::OctreeFactoryBase* factory);
    void RenderOctreeBoxes(GIImmediateCommands* cmd, GIRenderView* view, Scene* scene);
protected:
    static std::vector<MeshObject<Vertex3D>> GenerateOcreeBoxMeshes(const SpaceOctree::OctreeFactoryBase* factory);

    std::vector<MeshObject<Vertex3D>> _spaceBoxes;
};

