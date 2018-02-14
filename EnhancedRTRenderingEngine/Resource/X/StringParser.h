#pragma once

#include <fstream>
#include "DXModel.h"

namespace StringParser
{
    int ParseXFile(std::ifstream& ifs, DXModel* model);
}
