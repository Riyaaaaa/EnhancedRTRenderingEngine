#include "stdafx.h"

#include "StringParser.h"
#include "Common.h"

#include "DXParserNodes.h"

int StringParser::ParseXFile(std::ifstream& ifs, DXModel* model) {
    std::string buffer((std::istreambuf_iterator<char>(ifs)),  std::istreambuf_iterator<char>());
    
    auto itr = buffer.begin(), end = buffer.end();
    DXRootParser XFileParser;
    XFileParser.Parse(itr, end, model);

    return 0;
}
