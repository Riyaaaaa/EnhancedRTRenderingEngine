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
    _drawTargetIndexes.resize(model.material_count);
    _drawTargetNum = model.material_count;
    for (std::size_t  i = 0; i < model.material_count; i++) {
        for (std::size_t  j = 0; j < model.materials[i].face_vert_count; j++) {
            int idx = model.face_vert_index[cnt_idx];
            cnt_idx++;

            _vertexList[idx].col[0] = model.materials[i].diffuse_color[0];
            _vertexList[idx].col[1] = model.materials[i].diffuse_color[1];
            _vertexList[idx].col[2] = model.materials[i].diffuse_color[2];
            _vertexList[idx].col[3] = model.materials[i].alpha;
        }

        _drawTargetIndexes[i] = model.materials[i].face_vert_count;
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
			{ mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z },
			{ mesh.meshTextureCoords.textureCoords[i].x, mesh.meshTextureCoords.textureCoords[i].y },
			{ mesh.meshNormals.normals[i].x, mesh.meshNormals.normals[i].y, mesh.meshNormals.normals[i].z },
		    { 0.0f, 0.0f, 0.0f, 0.0f }
		};
	}

	// Sort by material indexes
	std::vector<std::pair<int, std::size_t>> indexedArr(mesh.meshMaterialList.nFaceIndexes);
	for (std::size_t i = 0; i < mesh.meshMaterialList.faceIndexes.size(); i++) {
		indexedArr[i] = std::make_pair(mesh.meshMaterialList.faceIndexes[i], i);
	}
	std::sort(indexedArr.begin(), indexedArr.end());

	// Expand face indexes and make index list 
	_indexList.resize(mesh.nFaces);
	for (std::size_t  i = 0; i < mesh.nFaces; i++) {
		int index = indexedArr[i].second;
		for (std::size_t j = 0; j < mesh.faces.size(); i++) {
			_indexList[i] = mesh.faces[index][j];
		}
	}

	int cnt_idx = 0;
	_materialTextures.resize(mesh.meshMaterialList.nMaterials);
	_speculars.resize(mesh.meshMaterialList.nMaterials);
	_drawTargetIndexes.resize(mesh.meshMaterialList.nMaterials);
	_drawTargetNum = mesh.meshMaterialList.nMaterials;

	for (auto && elem : indexedArr) {
		_drawTargetIndexes[elem.first] += mesh.faces[elem.second].size();
	}

	// Set vertex color from face color
	int matIdx = 0; int faceCount = 0;
	for (std::size_t  i = 0; i < mesh.meshMaterialList.nFaceIndexes; i++) {
		int idx = mesh.meshMaterialList.faceIndexes[i];
		faceCount++;

		for (std::size_t  j = 0; j < mesh.faces[idx].size(); j++) {
			_vertexList[mesh.faces[idx][j]].col[0] = mesh.meshMaterialList.materials[matIdx].faceColor.x;
			_vertexList[mesh.faces[idx][j]].col[1] = mesh.meshMaterialList.materials[matIdx].faceColor.y;
			_vertexList[mesh.faces[idx][j]].col[2] = mesh.meshMaterialList.materials[matIdx].faceColor.z;
			_vertexList[mesh.faces[idx][j]].col[3] = mesh.meshMaterialList.materials[matIdx].faceColor.w;
		}

		if (_drawTargetIndexes[matIdx] == faceCount) {
			matIdx++;
			faceCount = 0;
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

	_vertexCount = _indexList.size();
}

std::vector<Material> Mesh3DModel::CreatePMDDefaultMaterials() {
    std::vector<Material> materials(_drawTargetNum);

    for (int i = 0; i < _drawTargetNum; i++) {
		materials[i].vShader = ResourceLoader::LoadShader("LightingVertexShader");

		if (_materialTextures[i] != "") {
			materials[i].pShader = ResourceLoader::LoadShader("LightingPSTextureColor");
			ResourceLoader::LoadTexture(_materialTextures[i], &materials[i].texture);
			materials[i].specular = _speculars[i];
            materials[i].metallic = 0.5f;
            materials[i].roughness = 0.2f;
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
