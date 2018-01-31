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

	_vertexCount = model.face_vert_count;
}


Mesh3DModel::~Mesh3DModel()
{
}
