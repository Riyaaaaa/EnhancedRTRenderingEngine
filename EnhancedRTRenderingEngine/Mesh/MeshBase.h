#pragma once

#include <vector>

#include "Constant/RenderConfig.h"

#include "Structure/Primitive.h"

struct ElementDesc {
    unsigned int faceIdx;
    unsigned int faceNumVerts;
    unsigned int materialIdx;
};

class MeshBase {
public:
    MeshBase() : _materialNum(1), _vertexCount(0), _primitiveType(VertexPrimitiveType::TRIANGLELIST) {};
    virtual ~MeshBase() {};

    const std::vector<uint16_t>& GetIndexList() const { return _indexList; }

    unsigned int GetVertexCount() const { return _vertexCount; }
    bool HasIndexList() const { return !_indexList.empty(); }
    int GetMaterialNum() const { return _materialNum; }
    std::size_t GetDrawTargetNum() const { return _elements.size(); }
    const std::vector<ElementDesc>& GetDrawElementMap() const { return _elements; }

    void SetPrimitiveType(VertexPrimitiveType type) { _primitiveType = type; }
    VertexPrimitiveType GetPrimitiveType() const { return _primitiveType; }

    virtual std::vector<Triangle> GetTriangles() = 0;

protected:
    int _materialNum;
    // face index / material index
    std::vector<ElementDesc> _elements;
    std::vector<uint16_t> _indexList;
    unsigned int _vertexCount;

    VertexPrimitiveType _primitiveType;
};

template<class VertType>
class Mesh : public MeshBase {
public:
    using MeshBase::MeshBase;
    typedef VertType Type;
    virtual ~Mesh() {};

    void AddVertex(const VertType& vert) {
        _vertexList.push_back(vert);
        _vertexCount++;
    }

    VertType& GetVertex(unsigned int idx) {
        if (HasIndexList()) {
            return _vertexList[_indexList[idx]];
        }
        else {
            return _vertexList[idx];
        }
    }

    const std::vector<VertType>& GetVertexList() const { return _vertexList; }

    std::vector<Triangle> GetTriangles() override {
        std::vector<Triangle> triangles(GetVertexCount() / 3);
        for (int i = 0; i < GetVertexCount(); i+=3) {
            if (HasIndexList()) {
                triangles[i / 3].v0 = _vertexList[_indexList[i]].pos;
                triangles[i / 3].v1 = _vertexList[_indexList[i + 1]].pos;
                triangles[i / 3].v2 = _vertexList[_indexList[i + 2]].pos;
            }
            else {
                triangles[i / 3].v0 = _vertexList[i].pos;
                triangles[i / 3].v1 = _vertexList[i + 1].pos;
                triangles[i / 3].v2 = _vertexList[i + 2].pos;
            }
        }

        return triangles;
    }

    std::vector<VertType> _vertexList;
};

