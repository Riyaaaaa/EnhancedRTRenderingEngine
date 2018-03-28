#include "stdafx.h"
#include "SpaceOctreeVisualizer.h"

#include "Mesh/SimpleModel/Box.h"

#include "Scene/Scene.h"

#include "D3D11/D3D11UnlitRenderer.h"

using namespace SpaceOctree;

void SpaceOctreeVisualizer::Initialize(const SpaceOctree::LinerOctreeFactory& factory) {
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
(const LinerOctreeFactory& factory) {
    std::vector<MeshObject<Vertex3D>> spaceBoxes;

    for (int i = 0; i < factory.GetTree().size(); i++) {
        auto& box = factory.GetTree()[i];
        if (box && box->head) {
            AABB aabb = factory.CalculateOctreeBoxAABBFromMortonNumber(i);
            spaceBoxes.push_back(MeshObject<Vertex3D>(std::make_shared<Box>(aabb.size() / 2.0f)));
            spaceBoxes.back().SetLocation(aabb.Center());
        }
    }

    return spaceBoxes;
}