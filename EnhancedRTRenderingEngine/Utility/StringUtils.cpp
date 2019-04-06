
#include "StringUtils.h"

#include <regex>
#include <string>


std::string StringUtils::GetExtension(std::string path) {
//todo: backward match
    std::regex re(R"(.+\.(\w+))");
    std::cmatch match;
    std::string ex;

    if (std::regex_match(path.c_str(), match, re)) {
        return match.str(1);
    }

    return "";
}