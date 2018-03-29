#pragma once

#include <vector>

#include "Constant/RenderConfig.h"

struct Face {
    std::size_t faceIdx;
    std::size_t faceNumVerts;
    std::size_t materialIdx;
};

template<class VertType>
class MeshBase
{
public:
    typedef VertType Type;

    MeshBase() : _materialNum(1), _vertexCount(0) {};
    virtual ~MeshBase() {};

    void AddVertex(const VertType& vert) {
        _vertexList.push_back(vert);
        _vertexCount++;
    }

    const std::vector<VertType>& GetVertexList() const { return _vertexList; }
    const std::vector<uint16_t>& GetIndexList() const { return _indexList; }

    std::size_t GetVertexCount() const { return _vertexCount; }
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
    std::vector<VertType> _vertexList;
    std::vector<uint16_t> _indexList;
    std::size_t _vertexCount;

    VertexPrimitiveType _primitiveType;
};

