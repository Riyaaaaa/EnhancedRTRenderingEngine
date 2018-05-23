#include "stdafx.h"

#include "MeshObject.h"

#include "Algorithms/SpaceSplits.h"
#include "Utility/MathUtils.h"

using namespace DirectX;

void MeshObjectBase::FindPrecisionReflectionSource(const std::vector<CubeReflectionCapture*>& captures) {
    unsigned int precision = INT_MAX;

    _hasReflectionSource = false;
    for (auto&& capture : captures) {
        if (capture->Contains(_transform.location) && capture->PrecisionSize() < precision) {
            precision = capture->PrecisionSize();
            _reflectionSourceId = capture->GetID();
            _hasReflectionSource = true;
        }
    }
}

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

    auto& face_map = _mesh->GetDrawElementMap();
    unsigned int start_idx = 0;
    for (unsigned int i = 0; i < face_map.size(); i++) {
        for (int j = start_idx; j < start_idx + face_map[i].faceNumVerts; j += 3) {
            Vector3D v1;
            Vector3D v2;
            Vector3D v0;

            if (indices.empty()) {
                XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v0.x)), _vertexTransformedCache[j]);
                XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v1.x)), _vertexTransformedCache[j + 1] - _vertexTransformedCache[j]);
                XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v2.x)), _vertexTransformedCache[j + 2] - _vertexTransformedCache[j]);
            }
            else {
                XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v0.x)), _vertexTransformedCache[indices[j]]);
                XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v1.x)), _vertexTransformedCache[indices[j + 1]] - _vertexTransformedCache[indices[j]]);
                XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(v2.x)), _vertexTransformedCache[indices[j + 2]] - _vertexTransformedCache[indices[j]]);
            }

            float det = Math::Determinant3x3(v1, v2, -ray.dir);

            if (det <= 0.0f) {
                continue;
            }

            auto p = ray.pos - v0;
            float u = Math::Determinant3x3(p, v2, -ray.dir) / det;
            if (u < 0 || u > 1) {
                continue;
            }
            float v = Math::Determinant3x3(v1, p, -ray.dir) / det;
            if (v < 0 || u + v > 1.0) {
                continue;
            }
            float t = Math::Determinant3x3(v1, v2, p) / det;
            if (t < 0) {
                continue;
            }

            auto toPoint = ray.dir * t;
            auto normal = Math::Normalize(Math::Cross(v1, v2));
            intersects.push_back(Hit(ray.pos + toPoint, Math::Normalize(Math::Reflect(toPoint, normal)), ray.dir, toPoint.Length(), face_map[i].materialIdx));
        }
        start_idx += face_map[i].faceNumVerts;
    }

    return intersects;
}

template<class VertType>
std::vector<Triangle> MeshObject<VertType>::GetTransformedTriangles() {
    std::vector<Triangle> triangles(_mesh->GetVertexCount() / 3);
    auto& index_list = _mesh->GetIndexList();
    for (int i = 0; i < _mesh->GetVertexCount(); i += 3) {
        if (_mesh->HasIndexList()) {
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(triangles[i / 3].v0.x)), _vertexTransformedCache[index_list[i]]);
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(triangles[i / 3].v1.x)), _vertexTransformedCache[index_list[i + 1]]);
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(triangles[i / 3].v2.x)), _vertexTransformedCache[index_list[i + 2]]);
        }
        else {
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(triangles[i / 3].v0.x)), _vertexTransformedCache[i]);
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(triangles[i / 3].v1.x)), _vertexTransformedCache[i + 1]);
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(triangles[i / 3].v2.x)), _vertexTransformedCache[i + 2]);
        }

        triangles[i / 3].normal = Math::CalcNormal(triangles[i / 3].v0, triangles[i / 3].v1, triangles[i / 3].v2);
    }

    return triangles;
}

void MeshObjectBase::DirtyWorldMatrix() {
    SceneObject::DirtyWorldMatrix();
    AABBDirty = true;
}

template MeshObject<MainVertex>;
template MeshObject<TexVertex>;
template MeshObject<Vertex3D>;
template MeshObject<LineVertex>;
