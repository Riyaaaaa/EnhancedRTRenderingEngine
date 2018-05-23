#include "stdafx.h"

#include <boost/timer.hpp>

#include "Common/Common.h"
#include "PrecomputeLightingSystem.h"
#include "Lighting/PhotonMapping.h"
#include "Lighting/LightMapBaker.h"

void PrecomputeLightingSystem::Compute(GIImmediateCommands* cmd, RenderScene* scene) {
    //TODO: Caluculate Scene Voxel Size
    auto staticMeshesOctree = std::make_unique<SpaceOctree::HashedOctreeFactory>(scene->GetSourceScene()->GetSceneAABB() * 1.2f, 8);

    for (auto && object : scene->GetSourceScene()->GetViewObjects()) {
        if (object->IsPhysicalObject()) {
            staticMeshesOctree->Register(object);
        }
    }

    boost::timer timer;

    PhotonMapping pm;
    pm.Compute(staticMeshesOctree.get(), scene->GetSourceScene());

    ERTREDebug(L"emit photons elapsed time: %lfs\n", timer.elapsed());

    timer.restart();

    LightMapBaker baker;
    auto& src_mesh_objects = scene->GetSourceScene()->GetViewObjects();

    auto lightMap = baker.Bake(cmd, src_mesh_objects, pm.kdtree, pm.photons);
    ERTREDebug(L"bake texture elapsed time: %lfs\n", timer.elapsed());

    scene->_lightMap = lightMap;
    scene->_staticMeshesOctree = std::move(staticMeshesOctree);
    scene->rayPaths = pm.rayPaths;
}
