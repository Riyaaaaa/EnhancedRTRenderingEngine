#pragma once

#include "MeshObject.h"

#include "Algorithms/SpaceSplits.h"

using namespace DirectX;

template<class VertType>
class StaticMeshObject : public MeshObject<VertType>
{
public:
    StaticMeshObject(const std::shared_ptr<Mesh<VertType>>& mesh, Transform transform) {
        this->_transform = transform;
    }

private:
    std::vector<VertType> _vertexList;

    using SceneObject::SetTransform;
    using SceneObject::SetRotation;
    using SceneObject::SetLocation;
    using SceneObject::SetScale;
};

