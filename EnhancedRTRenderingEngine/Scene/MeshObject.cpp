#include "stdafx.h"

#include "MeshObject.h"

#include "Algorithms/SpaceSplits.h"
#include "Utility/MathUtils.h"

using namespace DirectX;

template<class VertType>
AABB MeshObject<VertType>::GetAABB() {
    std::vector<Vector3D> _vertices(_mesh->GetVertexList().size());
    _vertexTransformedCache.resize(_mesh->GetVertexList().size());
    if (AABBDirty) {
        auto world = SceneObject::GetMatrix();
        for (int i = 0; i < _mesh->GetVertexList().size(); i++) {
            auto& pos = _mesh->GetVertexList()[i].pos;
            auto vector = XMVector3Transform(XMVectorSet(pos.x, pos.y, pos.z, 1.0f), world);
            _vertexTransformedCache[i] = vector;
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(_vertices[i].x)), vector);
        }
        AABBDirty = false;
        _aabb = SpaceSplits::CalculateOptimizedAABB(_vertices);
    }
    return _aabb;
}

template<class VertType>
Vector3D MeshObject<VertType>::GetVertexPosition(unsigned int idx) {
    return _mesh->GetVertexList()[idx].pos;
}

template<class VertType>
unsigned int MeshObject<VertType>::GetVertexNums() {
    return _mesh->GetVertexCount();
}

template<class VertType>
std::vector<Hit> MeshObject<VertType>::IntersectPositions(Ray ray) {
    std::vector<Hit> intersects;
    auto& indices = _mesh->GetIndexList();
    auto& vertices = _mesh->GetVertexList();

    if (_mesh->GetPrimitiveType() != VertexPrimitiveType::TRIANGLELIST) {
        // not support;
        return intersects;
    }

    for (int i = 0; i < indices.size(); i += 3) {
        Vector3D v1;
        Vector3D v2;
        Vector3D v0;

        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v0.x)), _vertexTransformedCache[indices[i]]);
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v1.x)), _vertexTransformedCache[indices[i + 1]] - _vertexTransformedCache[indices[i]]);
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v2.x)), _vertexTransformedCache[indices[i + 2]] - _vertexTransformedCache[indices[i]]);

        float det = MathUtils::Determinant3x3(v1, v2, -ray.dir);

        if (det <= 0.0f) {
            continue;
        }

        auto p = ray.pos - v0;
        float u = MathUtils::Determinant3x3(p, v2, -ray.dir) / det;
        if (u < 0 || u > 1) {
            continue;
        }
        float v = MathUtils::Determinant3x3(v1, p, -ray.dir) / det;
        if (v < 0 || v > 1) {
            continue;
        }
        float t = MathUtils::Determinant3x3(v1, v2, p) / det;
        if (t < 0) {
            continue;
        }

        auto toPoint = ray.dir * t;
        auto normal = MathUtils::Normalize(MathUtils::Cross(v1, v2));
        intersects.push_back(Hit(ray.pos + toPoint, MathUtils::Reflect(toPoint, normal), toPoint.Length()));
    }

    return intersects;
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
