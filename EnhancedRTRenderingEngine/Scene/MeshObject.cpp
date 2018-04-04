#include "stdafx.h"

#include "MeshObject.h"
#include "Algorithms/SpaceSplits.h"

using namespace DirectX;

template<class VertType>
AABB MeshObject<VertType>::GetAABB() {
    std::vector<Vector3D> _vertices(_mesh->GetVertexList().size());
    if (AABBDirty) {
        auto world = SceneObject::GetMatrix();
        for (int i = 0; i < _mesh->GetVertexList().size(); i++) {
            auto& pos = _mesh->GetVertexList()[i].pos;
            auto vector = XMVector3Transform(XMVectorSet(pos.x, pos.y, pos.z, 1.0f), world);
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(_vertices[i].x)), vector);
        }
        AABBDirty = false;
        _aabb = SpaceSplits::CalculateOptimizedAABB(_vertices);
    }
    return _aabb;
}

template<class VertType>
void MeshObject<VertType>::DirtyWorldMatrix() {
    SceneObject::DirtyWorldMatrix();
    AABBDirty = true;
}

template MeshObject<PMDVertex>;
template MeshObject<TexVertex>;
template MeshObject<Vertex3D>;
template MeshObject<LineVertex>;
