#include "stdafx.h"
#include "ResourceLoader.h"
#include "FileManager.h"

#include "Common.h"

#include "WindowManager.h"

#include "libpng/png.h"
#include "libpng/png.h"
#include "libpng/pngstruct.h"
#include "libpng/pnginfo.h"


//#include <fbxsdk.h>  

ResourceHandle<> ResourceLoader::LoadShader(std::string filename) {
	std::ifstream ifs;
	auto manager = FileManager::getInstance();
	auto path = manager->MakeRelativePath(filename + ".cso");

	if (manager->FileExists(path)) {
		return manager->GetResourceHandleFromCache<ResourceEntity>(path);
	}

	ifs.open(path, std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		return ResourceHandle<>{};
	}

	ifs.seekg(0, std::fstream::end);
	int eofPos = ifs.tellg();

	ifs.clear();
	ifs.seekg(0, std::fstream::beg);

	int begPos = ifs.tellg();

	std::size_t size = eofPos - begPos;

	char* buf = new char[size];
	ifs.read(buf, size);
	ifs.close();

	return manager->CreateCachedResourceHandle<ResourceEntity>(path, buf, size);
}

void* test(const char *filename) {
	FILE *fp;
	fopen_s(&fp, filename, "rb");
	if (fp == NULL) {
		perror(filename);
		return NULL;
	}

	int i, x, y;
	int width, height;
	int num;
	png_colorp palette;
	png_structp png = NULL;
	png_infop info = NULL;
	png_bytep row;
	png_bytepp rows;
	png_byte sig_bytes[8];
	if (fread(sig_bytes, sizeof(sig_bytes), 1, fp) != 1) {
		return NULL;
	}
	if (png_sig_cmp(sig_bytes, 0, sizeof(sig_bytes))) {
		return NULL;
	}
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png == NULL) {
		return NULL;
	}
	info = png_create_info_struct(png);
	if (info == NULL) {
		return NULL;
	}
	if (setjmp(png_jmpbuf(png))) {
		return NULL;
	}
	png_init_io(png, fp);
	png_set_sig_bytes(png, sizeof(sig_bytes));
	png_read_png(png, info, PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16, NULL);
	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	rows = png_get_rows(png, info);

	png_bytep buf = new png_byte[width * height * 4 * info->channels];

	switch (png_get_color_type(png, info)) {
	case PNG_COLOR_TYPE_PALETTE:
	case PNG_COLOR_TYPE_GRAY:
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		// not support
		break;
	case PNG_COLOR_TYPE_RGB: {
		png_bytepp rows = png_get_rows(png, info);
		for (int y = 0; y < info->height; y++) {
			auto* row = rows[y];
			for (int x = 0; x < info->width; x++) {
				memcpy(buf + y * x + x, row + x, sizeof(png_byte) * 3);
				*(buf + y * x + x) = 0xff;
			}
		}
		break;
	}
	case PNG_COLOR_TYPE_RGB_ALPHA: {
		png_uint_32 i = 0;
		int offset_point = 0;
		while (i < info->height)
		{
			png_read_row(png, buf + offset_point, NULL);
			offset_point += width * 4;
			i++;
		}
		break;
	}
	}
error:
	png_destroy_read_struct(&png, &info, NULL);
}

int ResourceLoader::LoadTexture(std::string filename, ResourceHandle<Texture2D>* outTex) {
	int w, h, d;
	png_structp Png;
	png_infop PngInfo;
	png_byte sig[4];

	auto manager = FileManager::getInstance();
	auto path = manager->MakeAssetPath("Texture\\" + filename + ".png");

	if (manager->FileExists(path)) {
		 *outTex = manager->GetResourceHandleFromCache<Texture2D>(path);
		 return 0;
	}

	//test(path.c_str());

	FILE *fp;
	fopen_s(&fp, path.c_str(), "rb");
	if (fp == NULL)return -1;

	png_byte sig_bytes[8];
	if (fread(sig_bytes, sizeof(sig_bytes), 1, fp) != 1) {
		return NULL;
	}
	if (png_sig_cmp(sig_bytes, 0, sizeof(sig_bytes))) {
		return NULL;
	}
	Png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (Png == NULL) {
		return NULL;
	}
	PngInfo = png_create_info_struct(Png);
	if (PngInfo == NULL) {
		return NULL;
	}
	if (setjmp(png_jmpbuf(Png))) {
		return NULL;
	}

	png_init_io(Png, fp);
	png_set_sig_bytes(Png, sizeof(sig_bytes));
	//png_read_info(Png, PngInfo);

	png_read_png(Png, PngInfo, PNG_TRANSFORM_IDENTITY, NULL);

	w = PngInfo->width;
	h = PngInfo->height; 
	d = PngInfo->bit_depth / 8 * 4;

	char str[256];
	sprintf_s(str, "%d x %d (%d bit/channel)\nrow %d byte\nchannnel count : %d",
		PngInfo->width, PngInfo->height, PngInfo->bit_depth,
		PngInfo->rowbytes,
		PngInfo->channels);

	if (PngInfo->interlace_type != PNG_INTERLACE_NONE)
	{
		png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
		fclose(fp);
		return -7;
	}
	png_bytep buf = new png_byte[w * h * d * 4];
	if (buf == NULL)
	{
		png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
		fclose(fp);
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
		for (int y = 0; y < PngInfo->height; y++) {
			auto* row = rows[y];
			for (int x = 0; x < PngInfo->width; x++) {
				memcpy(buf + y * w * 4 + (x * 4), row + x * 3, sizeof(png_byte) * 3);
				*(buf + y * w * 4 + (x * 4) + 3) = 0xff;
			}
		}
		break;
	}
	case PNG_COLOR_TYPE_RGB_ALPHA: {
		png_bytepp rows = png_get_rows(Png, PngInfo);
		for (int y = 0; y < PngInfo->height; y++) {
			auto* row = rows[y];
			for (int x = 0; x < PngInfo->width; x++) {
				memcpy(buf + y * w * 4 + (x * 4), row + x * 4, sizeof(png_byte) * 4);
			}
		}
		break;
	}
	}

	//png_read_end(Png, NULL);

	*outTex = manager->CreateCachedResourceHandle<Texture2D>(path, PngInfo, (void*)buf, sizeof(buf));

	png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
	fclose(fp);

	return 0;
}

void ResourceLoader::LoadFBXModel(std::string filename) {
	//FbxManager* manager = FbxManager::Create();
	//FbxImporter* importer = FbxImporter::Create(manager, "");

	//if (!importer->Initialize(filename.c_str())) {
	//	return;
	//}
}