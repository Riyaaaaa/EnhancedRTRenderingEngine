#include "stdafx.h"
#include "PrecomputeLightingSystem.h"
#include "Lighting/PhotonMapping.h"
#include "Lighting/LightMapBaker.h"

void PrecomputeLightingSystem::Compute(GIImmediateCommands* cmd, RenderScene* scene) {
    //TODO: Caluculate Scene Voxel Size
    auto staticMeshesOctree = std::make_unique<SpaceOctree::HashedOctreeFactory>(AABB(Vector3D(-16, -16, -16), Vector3D(16, 16, 16)), 8);

    for (auto && object : scene->GetSourceScene()->GetViewObjects()) {
        if (object->IsPhysicalObject()) {
            staticMeshesOctree->Register(object);
        }
    }

    PhotonMapping pm;
    pm.Compute(staticMeshesOctree.get(), scene->GetSourceScene());

    LightMapBaker baker;
    auto& src_mesh_objects = scene->GetSourceScene()->GetViewObjects();
    std::vector<MeshBase*> meshes;

    for (auto && mesh_object : src_mesh_objects) {
        meshes.push_back(mesh_object->GetMesh().get());
    }

    auto lightMap = baker.Bake(cmd, meshes, pm.kdtree, pm.photons);
    scene->_lightMap = lightMap;
    scene->_staticMeshesOctree = std::move(staticMeshesOctree);
    scene->rayPaths = pm.rayPaths;
}