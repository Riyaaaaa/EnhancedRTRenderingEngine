#pragma once

#include "SpiralLibrary/GameSystem/SingletonBase.hpp"

#include <string>

class FileManager : public libspiral::SingletonBase<FileManager>
{
public:
	FileManager();

	std::string MakeRelativePath(std::string filename);

private:
	std::string _projDir;
};

