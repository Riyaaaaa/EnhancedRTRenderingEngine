#pragma once

#include "MeshObject.h"

using namespace DirectX;

template<class VertType>
class StaticMeshObject : public MeshObject<VertType>
{
public:
    StaticMeshObject(const std::shared_ptr<MeshBase<VertType>>& mesh, Transform transform) {
        this->_transform = transform;
        auto world = SceneObject::GetMatrix();
        auto& vertList = mesh->GetVertexList();

        _vertexList = vertList;

        for (int i = 0; i < vertList.size(); i++) {
            auto& pos = vertList[i].pos;
            auto vector = XMVector3Transform(XMVectorSet(pos.x, pos.y, pos.z, 1.0f), world);
            _vertexList[i].pos = pos;
            XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&(_vertexList[i].pos.x)), vector);
        }
    }

private:
    std::vector<VertType> _vertexList;

    /*using SceneObject::SetTransform;
    using SceneObject::SetRotation;
    using SceneObject::SetLocation;
    using SceneObject::SetScale;*/
};

