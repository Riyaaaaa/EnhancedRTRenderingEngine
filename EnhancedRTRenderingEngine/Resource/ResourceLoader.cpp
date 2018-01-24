#include "stdafx.h"
#include "ResourceLoader.h"

//#include <fbxsdk.h>  

ResourceHandle ResourceLoader::LoadShader(std::string filename) {
	std::ifstream ifs;
	ifs.open(filename + ".cso", std::ios::in | std::ios::binary);

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

void ResourceLoader::LoadFBXModel(std::string filename) {
	//FbxManager* manager = FbxManager::Create();
	//FbxImporter* importer = FbxImporter::Create(manager, "");

	//if (!importer->Initialize(filename.c_str())) {
	//	return;
	//}
}