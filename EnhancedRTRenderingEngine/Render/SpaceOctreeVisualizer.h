#pragma once

#include "Scene/MeshObject.h"
#include "Algorithms/SpaceOctree.h"
#include "D3D11/D3D11RenderView.h"

class Scene;

class SpaceOctreeVisualizer
{
public:
    void Initialize(const SpaceOctree::OctreeFactoryBase* factory);
    void RenderOctreeBoxes(const std::shared_ptr<D3D11RenderView>& view, Scene* scene);
protected:
    static std::vector<MeshObject<Vertex3D>> GenerateOcreeBoxMeshes(const SpaceOctree::OctreeFactoryBase* factory);

    std::vector<MeshObject<Vertex3D>> _spaceBoxes;
};

