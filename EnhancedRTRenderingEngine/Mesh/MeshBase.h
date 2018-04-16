#pragma once

#include <vector>

#include "Constant/RenderConfig.h"

struct Face {
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
    std::size_t GetDrawTargetNum() const { return _drawFacesMap.size(); }
    const std::vector<Face>& GetDrawFacesMap() const { return _drawFacesMap; }

    void SetPrimitiveType(VertexPrimitiveType type) { _primitiveType = type; }
    VertexPrimitiveType GetPrimitiveType() const { return _primitiveType; }

protected:
    int _materialNum;
    // face index / material index
    std::vector<Face> _drawFacesMap;
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
    const std::vector<VertType>& GetVertexList() const { return _vertexList; }

    std::vector<VertType> _vertexList;
};

