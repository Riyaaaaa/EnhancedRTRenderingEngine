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

ResourceHandle ResourceLoader::LoadShader(std::string filename) {
	std::ifstream ifs;
	ifs.open(FileManager::getInstance()->MakeRelativePath(filename + ".cso"), std::ios::in | std::ios::binary);

	if (!ifs.is_open()) {
		return ResourceHandle{};
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

	return ResourceHandle(buf, size);
}

int ResourceLoader::LoadTexture(std::string filename, Texture2D* outTex) {
	int w, h, d;
	png_structp Png;
	png_infop PngInfo;
	png_byte sig[4];

	auto path = FileManager::getInstance()->MakeAssetPath("Texture\\" + filename + ".png");

	FILE *fp;
	fopen_s(&fp, path.c_str(), "rb");
	if (fp == NULL)return -1;

	if (fread(sig, 4, 1, fp) < 1)
	{
		fclose(fp);
		return -2;
	}
	if (!png_check_sig(sig, 4))
	{
		fclose(fp);
		return -3;
	}
	if ((Png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL)
	{
		fclose(fp);
		return -4;
	}
	if ((PngInfo = png_create_info_struct(Png)) == NULL)
	{
		png_destroy_read_struct(&Png, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return -5;
	}
	if (setjmp(png_jmpbuf(Png)))
	{
		png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
		fclose(fp);
		return -6;
	}

	png_init_io(Png, fp);
	png_set_sig_bytes(Png, 4);
	png_read_info(Png, PngInfo);
	png_set_bgr(Png);

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
	png_bytep buf = new png_byte[w * h * d * PngInfo->channels];
	if (buf == NULL)
	{
		png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
		fclose(fp);
		return -8;
	}

	png_uint_32 i = 0;
	int offset_point = 0;
	while (i < PngInfo->height)
	{
		png_read_row(Png, buf + offset_point, NULL);
		offset_point += w * d;
		i++;
	}

	png_read_end(Png, NULL);
	png_destroy_read_struct(&Png, &PngInfo, (png_infopp)NULL);
	fclose(fp);

	*outTex = Texture2D{ (void*)buf, sizeof(buf) };

	return 0;
}

void ResourceLoader::LoadFBXModel(std::string filename) {
	//FbxManager* manager = FbxManager::Create();
	//FbxImporter* importer = FbxImporter::Create(manager, "");

	//if (!importer->Initialize(filename.c_str())) {
	//	return;
	//}
}