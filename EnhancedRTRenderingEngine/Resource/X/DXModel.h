#pragma once

#include <string>
#include <vector>

class DXModel
{
public:
    enum class FileFormat {
        TEXT,
        BINARY,
        TZIP,
        BZIP
    };

    struct XHeader {
        std::string magic;
        std::string version;
        FileFormat format;
        int floatSize;
    };
    XHeader header;

    typedef std::pair<std::string, std::string> XProperty;
    struct XTemplate {
        std::string TemplateName;
        std::string UID;
        std::vector<XProperty> properties;
    };

    std::vector<XTemplate> templates;
};

