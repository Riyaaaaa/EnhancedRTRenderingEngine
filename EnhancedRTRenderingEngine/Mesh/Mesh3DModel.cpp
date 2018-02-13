#include "stdafx.h"
#include "Mesh3DModel.h"

#include "Resource/ResourceLoader.h"

Mesh3DModel::Mesh3DModel(const PMDModel& model)
{
    _vertexList.resize(model.vert_count);

    for (int i = 0; i < model.vert_count; i++) {
        _vertexList[i] = PMDVertex{ {model.vertex[i].pos[0], model.vertex[i].pos[1], model.vertex[i].pos[2]}, {model.vertex[i].uv[0], model.vertex[i].uv[1]},{ model.vertex[i].normal_vec[0], model.vertex[i].normal_vec[1], model.vertex[i].normal_vec[2] } };
    }

    _indexList.resize(model.face_vert_count);
    for (int i = 0; i < model.face_vert_count; i++) {
        _indexList[i] = model.face_vert_index[i];
    }
    
    int cnt_idx = 0;
    _materialTextures.resize(model.material_count);
    _speculars.resize(model.material_count);
    _drawTargetIndexes.resize(model.material_count);
    _drawTargetNum = model.material_count;
    for (int i = 0; i < model.material_count; i++) {
        for (int j = 0; j < model.materials[i].face_vert_count; j++) {
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

std::vector<Material> Mesh3DModel::CreatePMDDefaultMaterials() {
    std::vector<Material> materials(_drawTargetNum);

    for (int i = 0; i < _drawTargetNum; i++) {
		materials[i].vShader = ResourceLoader::LoadShader("LightingVertexShader");

		if (_materialTextures[i] != "") {
			materials[i].pShader = ResourceLoader::LoadShader("LightingPSTextureColor");
			ResourceLoader::LoadTexture(_materialTextures[i], &materials[i].texture);
			materials[i].specular = _speculars[i];
		}
		else {
			materials[i].pShader = ResourceLoader::LoadShader("LightingPSVertexColor");
		}
    }

    return materials;
}


Mesh3DModel::~Mesh3DModel()
{
}
