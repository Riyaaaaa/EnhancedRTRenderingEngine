#include "stdafx.h"

#define BOOST_RESULT_OF_USE_TR1

#include "StringParser.h"
#include "Common.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "DXGrammers.h"
#include "DXParserNodes.h"

#include <boost/format.hpp>

int StringParser::ParseXFile(std::ifstream& ifs, DXModel* model) {
    std::string buffer((std::istreambuf_iterator<char>(ifs)),  std::istreambuf_iterator<char>());
    
    auto itr = buffer.begin(), end = buffer.end();

    {
        XHeaderGrammar<std::string::iterator> parser;
        qi::parse(itr, end, parser, model->header);
    }

    {
        XTemplateGrammar<std::string::iterator> parser;
        while (true) {
            DXModel::XTemplate xtemplate;
            bool r = qi::parse(itr, end, parser, xtemplate);
            if (r) {
                model->templates.push_back(xtemplate);
            }

            if (!r || itr == end) {
                break;
            }
        }
    }

    RootParser bodyParser;
    bodyParser.Parse(itr, end, model);

    return 0;
}
