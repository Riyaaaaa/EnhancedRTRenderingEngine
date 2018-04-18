#pragma once

#include "Structure/Vector.h"
#include "Structure/Structure.h"

#include "MeshBase.h"

struct ExpandedTexel {
    Vector4D worldPosition;
};

class ExpandMap {
public:
    ExpandMap(unsigned int size) : 
        _size(size), 
        _map(size * size) {}

    ExpandedTexel& operator()(unsigned int x, unsigned int y) {
        return _map[y * _size + x];
    }

protected:
    unsigned int _size;
    std::vector<ExpandedTexel> _map;
};

class MeshExpander
{
public:
    MeshExpander(unsigned int size, unsigned int margin = 1);

    ExpandMap Build(MeshBase* mesh);

protected:
    unsigned int  _expandSize, _margin;
};

