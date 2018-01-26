#include "stdafx.h"
#include "FileManager.h"

#include <fstream>

FileManager::FileManager()
{
	std::ifstream ifs("ProjectDirPath.txt");
	if (ifs.is_open()) {
		std::getline(ifs, _projDir, ' ');
	}

	RID = 0;
}

std::string FileManager::MakeRelativePath(std::string filename) {
	return _projDir + filename;
}

std::string FileManager::MakeAssetPath(std::string filename) {
	return "Asset\\" + filename;
}

bool FileManager::FileExists(std::string key) {
	return _resourceCache.find(key) != _resourceCache.end();
}

void FileManager::purgeCacheAll() {
	_resourceCache.clear();
	_textureCache.clear();
}
