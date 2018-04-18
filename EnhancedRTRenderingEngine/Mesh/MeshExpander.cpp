#include "stdafx.h"
#include "MeshExpander.h"

MeshExpander::MeshExpander(unsigned int size, unsigned int margin) {

}

ExpandMap MeshExpander::Build(MeshBase* mesh) {
    ExpandMap map(_expandSize);

    auto triangles = mesh->GetTriangles();


    for (unsigned int i = 0; i < triangles.size(); i++) {
        
    }

    return map;
}