#pragma once

#include <string>
#include <vector>

#include "Structure/Structure.h"

class DXModel
{
public:
    enum class FileFormat {
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

    struct MeshFaceWraps {
        // UNUSED
    };

    struct MeshTextureCoords {
        DWORD nTextureCoords;
        std::vector<Vector2D> textureCoords;
    };

    struct MeshFace {
        DWORD nFaceVertexIndices;
        std::vector<DWORD> faceVertexIndices;
    };

    struct MeshNormals {
        DWORD nNormals;
        std::vector<Vector3D> normals;
        DWORD nFaceNormals;
        std::vector<MeshFace> meshFaces;
    };

    struct IndexedColor {
        DWORD index;
        Vector4D indexColor;
    };

    struct MeshVertexColors {
        DWORD nVertexColors;
        std::vector<IndexedColor> vertexColors;
    };

    struct Material {
        Vector4D faceColor;
        float power;
        Vector3D specularColor;
        Vector3D emissiveColor;
        std::string textureFileName;
    };

    struct MeshMaterialList {
        DWORD nMaterials;
        DWORD nFaceIndexes;
        std::vector<DWORD> faceIndexes;
        std::vector<DXModel::Material> materials;
    } meshMaterialList;

    struct Mesh {
        DWORD nVertices;
        std::vector<Vector3D> vertices;
        DWORD nFaces;
        std::vector<std::vector<int>> faces;

        // optional data elements
        MeshTextureCoords meshTextureCoords;
        MeshNormals meshNormals;
        MeshVertexColors meshVetexColors;
        MeshMaterialList meshMaterialList;
    } mesh;
};

