#pragma once

#include <vector>

template<class VertType>
class MeshBase
{
public:
    typedef VertType Type;

    MeshBase() : _drawTargetNum(1), _drawTargetIndexes() {};
    virtual ~MeshBase() {};

    const std::vector<VertType>& GetVertexList() const { return _vertexList; }
    const std::vector<uint16_t>& GetIndexList() const { return _indexList; }
    std::size_t GetVertexCount() const { return _vertexCount; }

    bool HasIndexList() const { return !_indexList.empty(); }

    int GetDrawTargetNum() const { return _drawTargetNum; }
    const std::vector<int>& GetDrawTargetIndexes() const { return _drawTargetIndexes; }

protected:
    int _drawTargetNum;
    std::vector<int> _drawTargetIndexes;
    std::vector<VertType> _vertexList;
    std::vector<uint16_t> _indexList;
    std::size_t _vertexCount;
};

