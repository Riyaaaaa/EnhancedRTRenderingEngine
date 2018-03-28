#include "stdafx.h"
#include "SpaceOctreeVisualizer.h"

#include "Mesh/SimpleModel/Box.h"

#include "Scene/Scene.h"

#include "D3D11/D3D11UnlitRenderer.h"

using namespace SpaceOctree;

void SpaceOctreeVisualizer::Initialize(const SpaceOctree::OctreeFactoryBase* factory) {
    _spaceBoxes = GenerateOcreeBoxMeshes(factory);
}

void SpaceOctreeVisualizer::RenderOctreeBoxes(const std::shared_ptr<D3D11RenderView>& view, Scene* scene) {
    view->SetRasterizerState(RasterizerState::WireFrame);

    // TODO: abstract by graphics API
    D3D11UnlitRenderer renderer;
    renderer.Initialize(view);
    renderer.render(scene->GetMainCamera(), _spaceBoxes);

    view->SetRasterizerState(RasterizerState::Default);
}

std::vector<MeshObject<Vertex3D>> SpaceOctreeVisualizer::GenerateOcreeBoxMeshes
(const SpaceOctree::OctreeFactoryBase* factory) {
    std::vector<MeshObject<Vertex3D>> spaceBoxes;

    factory->IterateEnableBox([&](const std::pair<uint32_t, OctreeBox*>& pair) {
        AABB aabb = factory->CalculateOctreeBoxAABBFromMortonNumber(pair.first);
        spaceBoxes.push_back(MeshObject<Vertex3D>(std::make_shared<Box>(aabb.size() / 2.0f)));
        spaceBoxes.back().SetLocation(aabb.Center());
    });

    return spaceBoxes;
}