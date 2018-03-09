#include "stdafx.h"
#include "Mesh3DModel.h"

#include "Resource/ResourceLoader.h"

#include <algorithm>

Mesh3DModel::Mesh3DModel(const PMDModel& model)
{
    _vertexList.resize(model.vert_count);

    for (std::size_t i = 0; i < model.vert_count; i++) {
        _vertexList[i] = PMDVertex{ {model.vertex[i].pos[0], model.vertex[i].pos[1], model.vertex[i].pos[2]}, {model.vertex[i].uv[0], model.vertex[i].uv[1]},{ model.vertex[i].normal_vec[0], model.vertex[i].normal_vec[1], model.vertex[i].normal_vec[2] } };
    }

    _indexList.resize(model.face_vert_count);
    for (std::size_t  i = 0; i < model.face_vert_count; i++) {
        _indexList[i] = model.face_vert_index[i];
    }
    
    int cnt_idx = 0;
    _materialTextures.resize(model.material_count);
    _speculars.resize(model.material_count);
    _drawFacesMap.resize(model.material_count);
    _materialNum = model.material_count;
    for (std::size_t  i = 0; i < model.material_count; i++) {
        for (std::size_t  j = 0; j < model.materials[i].face_vert_count; j++) {
            int idx = model.face_vert_index[cnt_idx];
            cnt_idx++;

            _vertexList[idx].col.x = model.materials[i].diffuse_color[0];
            _vertexList[idx].col.y = model.materials[i].diffuse_color[1];
            _vertexList[idx].col.z = model.materials[i].diffuse_color[2];
            _vertexList[idx].col.w = model.materials[i].alpha;
        }

        _drawFacesMap[i] = Face{ i, model.materials[i].face_vert_count, i };
        _materialTextures[i] = model.materials[i].texture_file_name;
        _speculars[i] = Vector3D{ model.materials[i].specular_color[0],model.materials[i].specular_color[1],model.materials[i].specular_color[2] };
    }

    _vertexCount = model.face_vert_count;
}

Mesh3DModel::Mesh3DModel(const DXModel& model) {
    auto& mesh = model.mesh;
    _vertexList.resize(mesh.nVertices);

    // Vertex data into one bundle
    for (std::size_t  i = 0; i < mesh.nVertices; i++) {
        _vertexList[i] = PMDVertex{
            Vector3D{ mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z },
            Vector2D{ mesh.meshTextureCoords.textureCoords[i].x, mesh.meshTextureCoords.textureCoords[i].y },
            Vector3D{ 0.0f, 0.0f, 0.0f },
            Vector4D{ 0.0f, 0.0f, 0.0f }
        };
    }

    for (std::size_t i = 0; i < mesh.nFaces; i++) {
        for (std::size_t j = 0; j < mesh.faces[i].size(); j++) {
            auto& normal = mesh.meshNormals.normals[mesh.meshNormals.faceNormals[i].faceVertexIndices[j]];
            _vertexList[mesh.faces[i][j]].normal += normal;
        }
    }

    for (auto&& vert : _vertexList) {
        vert.normal.Normalize();
    }

    int cnt_idx = 0;
    _materialTextures.resize(mesh.meshMaterialList.nMaterials);
    _speculars.resize(mesh.meshMaterialList.nMaterials);
    _drawFacesMap.resize(mesh.meshMaterialList.nFaceIndexes);
    _materialNum = mesh.meshMaterialList.nMaterials;

    for (std::size_t i = 0; i < mesh.meshMaterialList.faceIndexes.size(); i++) {
        std::size_t primitiveVerts = mesh.faces[i].size();
        if (primitiveVerts == 4) {
            // TRIANGLE STRIP DRAW
            _drawFacesMap[i] = Face{ i, 6 , mesh.meshMaterialList.faceIndexes[i] };
        }
        else {
            _drawFacesMap[i] = Face{ i, primitiveVerts , mesh.meshMaterialList.faceIndexes[i] };
        }
        
    }

    std::sort(_drawFacesMap.begin(), _drawFacesMap.end(), [](const Face& lhs, const Face& rhs) {
        return lhs.materialIdx != rhs.materialIdx ? lhs.materialIdx < rhs.materialIdx : lhs.faceIdx < rhs.faceIdx;
    });

    // Expand face indexes and make index list 
    _indexList.reserve(mesh.nFaces * 3);
    for (std::size_t i = 0; i < mesh.nFaces; i++) {
        auto& face = mesh.faces[_drawFacesMap[i].faceIdx];
        auto primitiveVerts = face.size();
        std::vector<int> visitVerts;
        if (primitiveVerts == 4) {
            visitVerts = {face[0], face[1], face[2], face[0], face[2], face[3]};
        }
        else {
            visitVerts = face;
        }

        for (int index : visitVerts) {
            _indexList.push_back(index);
        }
    }

    // Set vertex color from face color
    for (std::size_t  i = 0; i < mesh.meshMaterialList.nFaceIndexes; i++) {
        int matIdx = _drawFacesMap[i].materialIdx;
        for (std::size_t  j = 0; j < mesh.faces[i].size(); j++) {
            _vertexList[mesh.faces[i][j]].col = mesh.meshMaterialList.materials[matIdx].faceColor;
        }
    }

    for (std::size_t  i = 0; i < mesh.meshMaterialList.nMaterials; i++) {
        _materialTextures[i] = mesh.meshMaterialList.materials[i].textureFileName;
        _speculars[i] = Vector3D{ 
            mesh.meshMaterialList.materials[i].specularColor.x, 
            mesh.meshMaterialList.materials[i].specularColor.y, 
            mesh.meshMaterialList.materials[i].specularColor.z
        };
    }

    int oldMatIdx = -1;
    std::size_t indexCount = 0;
    std::vector<Face> dist;
    for (int i = 0; i < _drawFacesMap.size(); i++) {
        if (oldMatIdx != _drawFacesMap[i].materialIdx) {
            if (oldMatIdx != -1) {
                dist.push_back(Face{ 0, indexCount, static_cast<std::size_t>(oldMatIdx) });
            }
            oldMatIdx = _drawFacesMap[i].materialIdx;
            indexCount = 0;
        }
        indexCount += _drawFacesMap[i].faceNumVerts;
    }
    dist.push_back(Face{ 0, indexCount, static_cast<std::size_t>(oldMatIdx) });
    _drawFacesMap.swap(dist);

    _vertexCount = _indexList.size();
}

std::vector<Material> Mesh3DModel::CreatePMDDefaultMaterials() {
    std::vector<Material> materials(_materialNum);

    for (int i = 0; i < _materialNum; i++) {
        materials[i].vShader = ResourceLoader::LoadShader("LightingVertexShader");

        if (_materialTextures[i] != "") {
            materials[i].pShader = ResourceLoader::LoadShader("LightingPSTextureColor");
            ResourceLoader::LoadTexture(_materialTextures[i], materials[i].texture);
            materials[i].specular = _speculars[i];
            materials[i].metallic = 0.5f;
            materials[i].roughness = 0.2f;
            materials[i].type = TextureType::Texture2D;
        }
        else {
            materials[i].pShader = ResourceLoader::LoadShader("LightingPSMain");
        }
    }

    return materials;
}


Mesh3DModel::~Mesh3DModel()
{
}
