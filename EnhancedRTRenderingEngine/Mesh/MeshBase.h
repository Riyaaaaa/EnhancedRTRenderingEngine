#pragma once

#include <vector>

template<class VertType>
class MeshBase
{
public:
    typedef VertType Type;

    MeshBase() : _materialNum(1) {};
    virtual ~MeshBase() {};

    const std::vector<VertType>& GetVertexList() const { return _vertexList; }
    const std::vector<uint16_t>& GetIndexList() const { return _indexList; }
    std::size_t GetVertexCount() const { return _vertexCount; }

    bool HasIndexList() const { return !_indexList.empty(); }

    int GetMaterialNum() const { return _materialNum; }
    std::size_t GetDrawTargetNum() const { return _drawFacesMap.size(); }
    const std::vector<std::pair<int, int>>& GetDrawFacesMap() const { return _drawFacesMap; }

protected:
    int _materialNum;
    // face index / material index
    std::vector<std::pair<int, int>> _drawFacesMap;
    std::vector<VertType> _vertexList;
    std::vector<uint16_t> _indexList;
    std::size_t _vertexCount;
};

