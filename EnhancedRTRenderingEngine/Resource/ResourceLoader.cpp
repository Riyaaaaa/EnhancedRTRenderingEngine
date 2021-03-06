
#include "ResourceLoader.h"
#include "FileManager.h"

#include "Common/Defines.h"
#include "Common/Common.h"
#include "WindowsApp.h"
#include "Utility/StringUtils.h"
#include "X/StringParser.h"

#include "libpng/png.h"
#include "libpng/pngstruct.h"
#include "libpng/pnginfo.h"

#include <regex>


std::tuple<char*, std::size_t> LoadBinary(std::ifstream& ifs) {
    ifs.seekg(0, std::fstream::end);
    std::streamoff eofPos = ifs.tellg();

    ifs.clear();
    ifs.seekg(0, std::fstream::beg);

    std::streamoff begPos = ifs.tellg();

    std::size_t size = static_cast<std::size_t>(eofPos - begPos);

    char* buf = new char[size];
    ifs.read(buf, size);
    ifs.close();

    return std::make_tuple(buf, size);
}

//#include <fbxsdk.h>  

RawBinary ResourceLoader::LoadShader(std::string filename) {
    std::ifstream ifs;
    auto manager = FileManager::getInstance();
    auto path = manager->MakeRelativePath(filename + ".cso");

    if (manager->FileExists(path)) {
        return manager->GetResourceHandleFromCache<RawBinary>(path);
    }

    ifs.open(path, std::ios::in | std::ios::binary);

    if (!ifs.is_open()) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return RawBinary();
    }

    char* buf;
    std::size_t size;

    std::tie(buf, size) = LoadBinary(ifs);

    return manager->CreateCachedResourceHandle<RawBinary>(path, buf, size);
}

int ResourceLoader::LoadTexture(std::string filename, Texture2D& outTex) {
    auto ex = StringUtils::GetExtension(filename);

    if (ex == "png") {
        return LoadPNG(filename, outTex);
    }
    else if (ex == "bmp") {
        return LoadBMP(filename, outTex);
    }

    return -1;
}

int ResourceLoader::LoadPNG(std::string filename, Texture2D& outTex) {
    int w, h, d;
    png_structp Png;
    png_infop PngInfo;

    auto manager = FileManager::getInstance();
    auto path = manager->MakeAssetPath("Texture\\" + filename);

    if (manager->FileExists(path)) {
         outTex = manager->GetResourceHandleFromCache<Texture2D>(path);
         return 0;
    }

    //test(path.c_str());

    FILE *fp;
    fopen_s(&fp, path.c_str(), "rb");
    if (fp == NULL)return -1;

    png_byte sig_bytes[8];
    if (fread(sig_bytes, sizeof(sig_bytes), 1, fp) != 1) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return NULL;
    }
    if (png_sig_cmp(sig_bytes, 0, sizeof(sig_bytes))) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return NULL;
    }
    Png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (Png == NULL) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return NULL;
    }
    PngInfo = png_create_info_struct(Png);
    if (PngInfo == NULL) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return NULL;
    }
    if (setjmp(png_jmpbuf(Png))) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return NULL;
    }

    png_init_io(Png, fp);
    png_set_sig_bytes(Png, sizeof(sig_bytes));

    png_read_png(Png, PngInfo, PNG_TRANSFORM_IDENTITY, NULL);

    w = PngInfo->width;
    h = PngInfo->height; 
    d = PngInfo->bit_depth / 8 * 4;

    if (PngInfo->interlace_type != PNG_INTERLACE_NONE)
    {
        png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
        fclose(fp);
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return -7;
    }
    png_bytep buf = new png_byte[w * h * d];
    if (buf == NULL)
    {
        png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
        fclose(fp);
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return -8;
    }

    switch (png_get_color_type(Png, PngInfo)) {
    case PNG_COLOR_TYPE_PALETTE:
    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        // not support
        break;
    case PNG_COLOR_TYPE_RGB: {
        png_bytepp rows = png_get_rows(Png, PngInfo);
        for (unsigned int  y = 0; y < PngInfo->height; y++) {
            auto* row = rows[y];
            for (unsigned int  x = 0; x < PngInfo->width; x++) {
                memcpy(buf + y * w * 4 + (x * 4), row + x * 3, sizeof(png_byte) * 3);
                *(buf + y * w * 4 + (x * 4) + 3) = 0xff;
            }
        }
        break;
    }
    case PNG_COLOR_TYPE_RGB_ALPHA: {
        png_bytepp rows = png_get_rows(Png, PngInfo);
        for (unsigned int  y = 0; y < PngInfo->height; y++) {
            auto* row = rows[y];
            for (unsigned int  x = 0; x < PngInfo->width; x++) {
                memcpy(buf + y * w * 4 + (x * 4), row + x * 4, sizeof(png_byte) * 4);
            }
        }
        break;
    }
    }

    outTex = manager->CreateCachedResourceHandle<Texture2D>(path, PngInfo, (void*)buf, sizeof(buf));
    outTex.SetTextureName(path);

    png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
    fclose(fp);

    return 0;
}

ResourceHandle<DXModel> ResourceLoader::LoadDXModel(std::string filename) {
    std::ifstream ifs;
    auto manager = FileManager::getInstance();
    auto path = manager->MakeAssetPath("3DModel\\" + filename + ".x");

    ifs.open(path, std::ios::in);

    if (!ifs.is_open()) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return ResourceHandle<DXModel>{};
    }

    ResourceHandle<DXModel> handle(DXModel{});
    StringParser::ParseXFile(ifs, handle.Get());
    if (handle().mesh.meshNormals.normals.empty()) {
        handle().CalcVertexNormals();
    }

    return handle;
}

ResourceHandle<PMDModel> ResourceLoader::LoadPMDModel(std::string filename) {
    std::ifstream ifs;
    auto manager = FileManager::getInstance();
    auto path = manager->MakeAssetPath("3DModel\\" + filename + ".pmd");

    ifs.open(path, std::ios::in | std::ios::binary);

    if (!ifs.is_open()) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return ResourceHandle<PMDModel>{};
    }

    char* buf;
    std::size_t size;

    std::tie(buf, size) = LoadBinary(ifs);

    return ResourceHandle<PMDModel>(PMDModel{ buf, size });
}

void ResourceLoader::LoadFBXModel(std::string filename) {
    //FbxManager* manager = FbxManager::Create();
    //FbxImporter* importer = FbxImporter::Create(manager, "");

    //if (!importer->Initialize(filename.c_str())) {
    //    return;
    //}
}

int ResourceLoader::LoadBMP(const std::string& filename, Texture2D& outTex) {
    std::ifstream ifs;

    auto manager = FileManager::getInstance();
    auto path = manager->MakeAssetPath("Texture\\" + filename);

    if (manager->FileExists(path)) {
        outTex = manager->GetResourceHandleFromCache<Texture2D>(path);
        return 0;
    }
    ifs.open(path, std::ios::in | std::ios::binary);

    if (!ifs.is_open()) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
		throw std::runtime_error("Can not open file");
#endif
        return -1;
    }

    ifs.seekg(14, std::fstream::beg);
    
    int headerSize;
    ifs.read(reinterpret_cast<char*>(&headerSize), 4);

    unsigned int w, h;
    short bit_depth;
    if (headerSize == 12) {
        // BITMAPCOREHEADER FORMAT
        short sw, sh;
        ifs.read(reinterpret_cast<char*>(&sw), 2);
        ifs.read(reinterpret_cast<char*>(&sh), 2);
        ifs.seekg(2, std::fstream::cur);
        ifs.read(reinterpret_cast<char*>(&bit_depth), 2);
    }
    else if (headerSize == 40) {
        // BITMAPINFOHEADER FORMAT
        ifs.read(reinterpret_cast<char*>(&w), 4);
        ifs.read(reinterpret_cast<char*>(&h), 4);
        ifs.seekg(2, std::fstream::cur);
        ifs.read(reinterpret_cast<char*>(&bit_depth), 2);
        if (bit_depth != 24) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
			throw std::runtime_error("Can not open file");
#endif
            return -2; // not support bit depth
        }

        int comptype, data_size;
        ifs.read(reinterpret_cast<char*>(&comptype), 4);
        if (comptype != 0) {
#ifdef THROW_EXCEPTION_WHEN_HANDRING_ERROR
			throw std::runtime_error("Can not open file");
#endif
            return -2; // not support compression bmp
        }
        ifs.read(reinterpret_cast<char*>(&data_size), 4);
        ifs.seekg(16, std::fstream::cur);
    }

    unsigned char* buf = new unsigned char[w * h * 24]; // R8G8B8A8
    char* rows = new char[w * h * bit_depth / 8 * 4];
    ifs.read(rows, w * h * bit_depth / 8 * 4);

    if (bit_depth == 24) {
        for (unsigned int y = 0; y < h; y++) {
            auto row = rows + y * w * 3;
            for (unsigned int x = 0; x < w; x++) {
                // BGR -> RGBA
                memcpy(buf + y * w * 4 + (x * 4), row + x * 3 + 2, sizeof(char));
                memcpy(buf + y * w * 4 + (x * 4) + 1, row + x * 3 + 1, sizeof(char));
                memcpy(buf + y * w * 4 + (x * 4) + 2, row + x * 3, sizeof(char));
                *(buf + y * w * 4 + (x * 4) + 3) = 0xff;
            }
        }
    }

    outTex = manager->CreateCachedResourceHandle<Texture2D>(path, w, h, 4u, (void*)buf, sizeof(buf));
    outTex.SetTextureName(path);

    ifs.close();

    return 0;
}