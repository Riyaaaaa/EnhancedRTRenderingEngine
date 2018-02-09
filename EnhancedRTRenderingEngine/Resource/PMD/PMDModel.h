#pragma once

#include "Structure/Structure.h"
#include "Resource/RawBinary.h"

class PMDModel : public RawBinary
{
public:
#pragma pack(push,1)
    struct t_header {
        unsigned char magic[3];
        float version;
        unsigned char model_name[20];
        unsigned char comment[256];
    };
    t_header* header;

    unsigned long vert_count;
    struct t_vertex {
        float pos[3];
        float normal_vec[3];
        float uv[2];
        unsigned short bone_num[2];
        unsigned char bone_weight;
        unsigned char edge_flag;
    };
    t_vertex *vertex;

    unsigned long face_vert_count;
    unsigned short* face_vert_index;

    struct t_material {
        float diffuse_color[3];
        float alpha;
        float specularity;
        float specular_color[3]; 
        float ambient_color[3];
        unsigned char toon_index;
        unsigned char edge_flag;
        unsigned long face_vert_count;
        char texture_file_name[20];
    };

    unsigned long material_count; 
    t_material *materials;
#pragma pack(pop)

    PMDModel() {}
    PMDModel(void* ptr, std::size_t size);
};

