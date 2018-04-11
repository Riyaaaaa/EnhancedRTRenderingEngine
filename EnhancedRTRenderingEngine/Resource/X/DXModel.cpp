#include "stdafx.h"

#include "Common/Common.h"
#include "DXModel.h"

#include "Utility/MathUtils.h"

void DXModel::CalcVertexNormals() {
    mesh.meshNormals.normals.clear();
    mesh.meshNormals.normals.resize(mesh.nFaces);
    mesh.meshNormals.faceNormals.resize(mesh.nFaces);
    for (unsigned long i = 0; i < mesh.nFaces; i++) {
        Vector3D& v0 = mesh.vertices[mesh.faces[i][0]];
        Vector3D& v1 = mesh.vertices[mesh.faces[i][1]];
        Vector3D& v2 = mesh.vertices[mesh.faces[i][2]];

        if (mesh.faces[i].size() == 3) {
            //mesh.meshNormals.faceNormals[i] = MathUtils::Cross(v1 - v2):
            Vector3D normal = MathUtils::Normalize(MathUtils::Cross(v1 - v0, v2 - v1));

            mesh.meshNormals.normals[i] = normal;
            mesh.meshNormals.faceNormals[i].faceVertexIndices[0] = i;
            mesh.meshNormals.faceNormals[i].faceVertexIndices[1] = i;
            mesh.meshNormals.faceNormals[i].faceVertexIndices[2] = i;
        }
        else if (mesh.faces[i].size() == 4) {
            Vector3D& v3 = mesh.vertices[mesh.faces[i][3]];
            Vector3D normal = MathUtils::Normalize(MathUtils::Cross(v0 - v2, v1 - v3));

            mesh.meshNormals.normals[i] = normal;
            mesh.meshNormals.faceNormals[i].faceVertexIndices[0] = i;
            mesh.meshNormals.faceNormals[i].faceVertexIndices[1] = i;
            mesh.meshNormals.faceNormals[i].faceVertexIndices[2] = i;
            mesh.meshNormals.faceNormals[i].faceVertexIndices[3] = i;
        }
        else {
            ERTREDebug(L"Not support face vertex numbers!");
        }
    }
}