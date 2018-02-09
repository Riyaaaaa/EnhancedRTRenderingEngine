#pragma once

#include <vector>

template<class VertType>
class MeshBase
{
public:
    typedef VertType Type;

    MeshBase() {};
    virtual ~MeshBase() {};

    const std::vector<VertType>& GetVertexList() const { return _vertexList; }
    const std::vector<uint16_t>& GetIndexList() const { return _indexList; }
    std::size_t GetVertexCount() const { return _vertexCount; }

    bool HasIndexList() const { return !_indexList.empty(); }

protected:
    std::vector<VertType> _vertexList;
    std::vector<uint16_t> _indexList;
    std::size_t _vertexCount;
};

