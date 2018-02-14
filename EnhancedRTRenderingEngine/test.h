#pragma once

#define HAS_UNIT_TEST

#ifdef HAS_UNIT_TEST

#include "Resource/X/StringParser.h"

#include "FileManager.h"

void test() {
    auto manager = FileManager::getInstance();
    auto path = manager->MakeAssetPath("3DModel\\");
    std::ifstream ifs(path + "nolicensed.x");
    DXModel model;
    StringParser::ParseXFile(ifs, &model);
}

#endif

