#include "stdafx.h"

#include "Common.h"
#include "DXModel.h"

#include "Utility/MathUtils.h"

void DXModel::CalcVertexNormals() {
    mesh.meshNormals.normals.clear();
    for (std::size_t i = 0; i < mesh.nFaces; i++) {
        Vector3D& v0 = mesh.vertices[mesh.faces[i][0]];
        Vector3D& v1 = mesh.vertices[mesh.faces[i][1]];
        Vector3D& v2 = mesh.vertices[mesh.faces[i][2]];

        if (mesh.faces[i].size() == 3) {
            //mesh.meshNormals.faceNormals[i] = MathUtils::Cross(v1 - v2):
            Vector3D normal = MathUtils::Normalize(MathUtils::Cross(v1 - v0, v2 - v1));
            mesh.meshNormals.normals[mesh.faces[i][0]] += normal;
            mesh.meshNormals.normals[mesh.faces[i][1]] += normal;
            mesh.meshNormals.normals[mesh.faces[i][2]] += normal;
        }
        else if (mesh.faces[i].size() == 4) {
            Vector3D& v3 = mesh.vertices[mesh.faces[i][3]];
            Vector3D normal = MathUtils::Normalize(MathUtils::Cross(v0 - v2, v1 - v3));
            mesh.meshNormals.normals[mesh.faces[i][0]] += normal;
            mesh.meshNormals.normals[mesh.faces[i][1]] += normal;
            mesh.meshNormals.normals[mesh.faces[i][2]] += normal;
            mesh.meshNormals.normals[mesh.faces[i][3]] += normal;
        }
        else {
            ERTREDebug(L"Not support face vertex numbers!");
        }
    }

    for (auto && normal : mesh.meshNormals.normals) {
        normal.Normalize();
    }
}