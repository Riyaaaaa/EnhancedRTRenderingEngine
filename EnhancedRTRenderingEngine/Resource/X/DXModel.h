#pragma once

#include <string>
#include <vector>

#include "Resource/ResourceEntity.h"
#include "Structure/Structure.h"

class DXModel : ResourceEntity<DXModel>
{
public:
    void Dispose() { }
    void Release() { }

    enum class FileFormat {
        NONE,
        TEXT,
        BINARY,
        TZIP,
        BZIP
    };

    struct XHeader {
        std::string magic;
        std::string version;
        FileFormat format;
        int floatSize;
    };
    XHeader header;

    struct XProperty {
        std::string type;
        std::string name;
        int size;
    };

    //typedef std::tuple<std::string, std::string, int> XProperty;

    struct XTemplate {
        std::string TemplateName;
        std::string UID;
        std::vector<XProperty> properties;
    };

    std::vector<XTemplate> templates;

    struct MeshTextureCoords {
        uint32_t nTextureCoords;
        std::vector<Vector2D> textureCoords;
    };

    struct MeshFace {
        uint32_t nFaceVertexIndices;
        std::vector<uint32_t> faceVertexIndices;
    };

    struct MeshNormals {
        uint32_t nNormals;
        std::vector<Vector3D> normals;
        uint32_t nFaceNormals;
        std::vector<MeshFace> faceNormals;
    };

    struct MeshVertexColors {
        uint32_t nVertexColors;
        std::vector<Vector4D> vertexColors;
    };

    struct Material {
        Vector4D faceColor;
        float power;
        Vector3D specularColor;
        Vector3D emissiveColor;
        std::string textureFileName;
    };

    struct MeshMaterialList {
        uint32_t nMaterials;
        uint32_t nFaceIndexes;
        std::vector<uint32_t> faceIndexes;
        std::vector<DXModel::Material> materials;
    };

    struct Mesh {
        uint32_t nVertices;
        std::vector<Vector3D> vertices;
        uint32_t nFaces;
        std::vector<std::vector<int>> faces;

        // optional data elements
        MeshTextureCoords meshTextureCoords;
        MeshNormals meshNormals;
        MeshVertexColors meshVertexColors;
        MeshMaterialList meshMaterialList;
    } mesh;

    void CalcVertexNormals();
};

