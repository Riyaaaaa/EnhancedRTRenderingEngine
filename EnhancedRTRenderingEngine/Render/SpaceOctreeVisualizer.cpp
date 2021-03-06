
#include "SpaceOctreeVisualizer.h"

#include "Mesh/SimpleModel/Box.h"

#include "Scene/Scene.h"

#include "UnlitRenderer.h"

using namespace SpaceOctree;

void SpaceOctreeVisualizer::Initialize(const SpaceOctree::OctreeFactoryBase* factory) {
    if (factory == nullptr) {
        return;
    }
    _spaceBoxes = GenerateOcreeBoxMeshes(factory);
}

void SpaceOctreeVisualizer::RenderOctreeBoxes(GIImmediateCommands* cmd, GIRenderView* view, Scene* scene) {
    if (_spaceBoxes.empty()){
        return;
    }
    view->SetRasterizerState(cmd, RasterizerState::WireFrame);

    UnlitRenderer::render(cmd, view, scene->GetMainCamera(), _spaceBoxes);

    view->SetRasterizerState(cmd, RasterizerState::Default);
}

std::vector<std::unique_ptr<MeshObject<Vertex3D>>> SpaceOctreeVisualizer::GenerateOcreeBoxMeshes
(const SpaceOctree::OctreeFactoryBase* factory) {
    std::vector<std::unique_ptr<MeshObject<Vertex3D>>> spaceBoxes;

    factory->IterateEnableBox([&](const std::pair<uint32_t, OctreeBox*>& pair) {
        AABB aabb = factory->CalculateOctreeBoxAABBFromMortonNumber(pair.first);
        spaceBoxes.push_back(std::make_unique<MeshObject<Vertex3D>>(std::make_shared<Box>(aabb.size() / 2.0f)));
        spaceBoxes.back()->SetLocation(aabb.Center());
    });

    return spaceBoxes;
}