#include "stdafx.h"
#include "Mesh3DModel.h"

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
    for (int i = 0; i < model.material_count; i++) {
        for (int j = 0; j < model.face_vert_count; j++) {
            int idx = model.face_vert_index[cnt_idx];
            cnt_idx++;

            _vertexList[idx].col[0] = model.materials[i].diffuse_color[0];
            _vertexList[idx].col[1] = model.materials[i].diffuse_color[1];
            _vertexList[idx].col[2] = model.materials[i].diffuse_color[2];
            _vertexList[idx].col[3] = model.materials[i].alpha;
        }

        std::string filename = model.materials[i].texture_file_name;

        //todo: bmp‘Î‰ž
    }

    _vertexCount = model.face_vert_count;
}


Mesh3DModel::~Mesh3DModel()
{
}
