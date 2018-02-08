#include "stdafx.h"
#include "PMDModel.h"


PMDModel::PMDModel(void* ptr, std::size_t size) : RawBinary(ptr, size)
{
	char* raw_byte = reinterpret_cast<char*>(ptr);
	int offset = 0;

	header = reinterpret_cast<t_header*> (raw_byte + offset);
	offset += sizeof(t_header);

	vert_count = *(decltype(vert_count)*)(raw_byte + offset);
	offset += sizeof(vert_count);

	vertex = reinterpret_cast<t_vertex*> (raw_byte + offset);
	offset += sizeof(t_vertex) * vert_count;

	face_vert_count = *(decltype(face_vert_count)*)(raw_byte + offset);
	offset += sizeof(decltype(face_vert_count));

	face_vert_index = reinterpret_cast<decltype(face_vert_index)> (raw_byte + offset);
	offset += sizeof(face_vert_index) * face_vert_count;

	material_count = *(decltype(material_count)*)(raw_byte + offset);
	offset += sizeof(material_count);

	materials = reinterpret_cast<t_material*>(raw_byte + offset);
	offset += sizeof(t_material) * material_count;
}
