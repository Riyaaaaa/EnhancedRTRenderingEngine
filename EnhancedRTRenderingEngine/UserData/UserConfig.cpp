#include "stdafx.h"
#include "UserConfig.h"

#include <stdio.h>

#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

UserConfig::UserConfig()
{
    char buf[512];
    FILE* fp;
    fopen_s(&fp, "userconfig.json", "rb");
    
    if (fp) {
        Document doc;
        FileReadStream rs(fp, buf, sizeof(buf));
        doc.ParseStream(rs);
        LoadAllConfigFromDocument(&doc);
        fclose(fp);
    }
}

void UserConfig::Flush() {
    Document doc;

    Value obj(kObjectType);
    obj.AddMember("BG.R", Value(_bgcolor.x), doc.GetAllocator());
    obj.AddMember("BG.G", Value(_bgcolor.y), doc.GetAllocator());
    obj.AddMember("BG.B", Value(_bgcolor.z), doc.GetAllocator());
    obj.AddMember("BG.A", Value(_bgcolor.w), doc.GetAllocator());

    doc.AddMember("UserConfig", obj, doc.GetAllocator());

    char buf[512];
    FILE* fp;
    fopen_s(&fp, "userconfig.config", "wb");
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
            _bgcolor.z = ait->value.GetFloat();
        }
        else {
            _bgcolor.w = 0.0f;
        }
    }
#undef Exists

    return true;
}
