#include "stdafx.h"
#include "UserConfig.h"

#include <stdio.h>

#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

#include "FileManager.h"

using namespace rapidjson;

UserConfig::UserConfig()
{
    char buf[512];
    FILE* fp;

    auto path = FileManager::getInstance()->MakeRelativePath("userconfig.json");

    fopen_s(&fp, path.c_str(), "rb");
    
    if (fp) {
        Document doc;
        FileReadStream rs(fp, buf, sizeof(buf));
        doc.ParseStream(rs);
        LoadAllConfigFromDocument(&doc);
        fclose(fp);
    }

    _isDisplayRayPaths = true;
    _visibleIndirectLights = true;
    _visibleReflections = true;
    _rasterizerType = RasterizerState::Default;
}

void UserConfig::Flush() {
    Document doc;
    doc.SetObject();

    doc.AddMember("BG.R", Value(_bgcolor.x), doc.GetAllocator());
    doc.AddMember("BG.G", Value(_bgcolor.y), doc.GetAllocator());
    doc.AddMember("BG.B", Value(_bgcolor.z), doc.GetAllocator());
    doc.AddMember("BG.A", Value(_bgcolor.w), doc.GetAllocator());

    auto path = FileManager::getInstance()->MakeRelativePath("userconfig.json");

    char buf[512];
    FILE* fp;
    fopen_s(&fp, path.c_str(), "wb");
    FileWriteStream ws(fp, buf, sizeof(buf));
    Writer<FileWriteStream> writer(ws);

    doc.Accept(writer);
    fclose(fp);
}

bool UserConfig::LoadAllConfigFromDocument(Document* doc) {
#define Exists(itr) itr != doc->MemberEnd()
    {
        auto rit = doc->FindMember("BG.R");
        if (Exists(rit)) {
            _bgcolor.x = rit->value.GetFloat();
        }
        else {
            _bgcolor.x = 0.0f;
        }
        auto git = doc->FindMember("BG.G");
        if (Exists(git)) {
            _bgcolor.y = git->value.GetFloat();
        }
        else {
            _bgcolor.y = 0.0f;
        }
        auto bit = doc->FindMember("BG.B");
        if (Exists(bit)) {
            _bgcolor.z = bit->value.GetFloat();
        }
        else {
            _bgcolor.z = 0.0f;
        }
        auto ait = doc->FindMember("BG.A");
        if (Exists(ait)) {
            _bgcolor.w = ait->value.GetFloat();
        }
        else {
            _bgcolor.w = 0.0f;
        }
    }
#undef Exists

    return true;
}
