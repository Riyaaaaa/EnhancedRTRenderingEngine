#include "stdafx.h"
#include "FileManager.h"

#include <fstream>

FileManager::FileManager()
{
	std::ifstream ifs("ProjectDirPath.txt");
	if (ifs.is_open()) {
		std::getline(ifs, _projDir, ' ');
	}
}

std::string FileManager::MakeRelativePath(std::string filename) {
	return _projDir + filename;
}