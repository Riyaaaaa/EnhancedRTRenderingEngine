#include "stdafx.h"

#define BOOST_RESULT_OF_USE_TR1

#include "StringParser.h"
#include "Common.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include <boost/format.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

#define TRANSFORM_STRUCT_MEMBER(ign, name, member) (decltype(name::member), member)
#define FUSION_ADAPT_STRUCT_AUTO(name, members) \
  BOOST_FUSION_ADAPT_STRUCT(name, BOOST_PP_SEQ_FOR_EACH(TRANSFORM_STRUCT_MEMBER, name, members))

FUSION_ADAPT_STRUCT_AUTO(
    DXModel::XProperty,
    (type)
    (name)
    (size)
)

FUSION_ADAPT_STRUCT_AUTO(
    DXModel::XTemplate,
    (TemplateName)
    (UID)
    (properties)
)

FUSION_ADAPT_STRUCT_AUTO(
    DXModel::XHeader,
    (magic)
    (version)
    (format)
    (floatSize)
)

FUSION_ADAPT_STRUCT_AUTO(
    Vector2D,
    (x)
    (y)
)

FUSION_ADAPT_STRUCT_AUTO(
    Vector3D,
    (x)
    (y)
    (z)
)

FUSION_ADAPT_STRUCT_AUTO(
    Vector4D,
    (x)
    (y)
    (z)
    (w)
)

struct FileFormatSymbol_ : public qi::symbols<char, DXModel::FileFormat> {
    FileFormatSymbol_()
    {
        add ("txt", DXModel::FileFormat::TEXT)
            ("bin", DXModel::FileFormat::BINARY)
            ("tzip", DXModel::FileFormat::TZIP)
            ("bzip", DXModel::FileFormat::BZIP);
    }
} FileFormatSymbol;

template<typename Iterator>
struct XHeaderGrammar : public qi::grammar<Iterator, DXModel::XHeader()> {
    XHeaderGrammar() : XHeaderGrammar::base_type(expr) {
        //auto file_format_rules = ascii::string("txt") | ascii::string("bin") | ascii::string("tzip") | ascii::string("bzip");
        expr = +(qi::char_ - qi::lit(' '))
            >> qi::lit(' ') >> +(qi::char_ - (FileFormatSymbol | qi::lit(' ')))
            >> FileFormatSymbol >> qi::lit(' ')
            >> qi::int_
            >> qi::omit[*qi::space];

    }
    qi::rule<Iterator, DXModel::XHeader()> expr;
};

template <typename Iterator>
struct XTemplateGrammar : public qi::grammar<Iterator, DXModel::XTemplate()> {
    XTemplateGrammar() : XTemplateGrammar::base_type(expr) {
        // TODO: Refactor by qi::lexeme
        expr = qi::omit[*qi::space] >> qi::lit("template") >> *qi::lit(' ') >> +(qi::char_ - (qi::lit('{') | qi::space)) >> qi::omit[*qi::space] >> qi::lit('{') >> qi::omit[*qi::space]
            // PARSE UID
            >> qi::lit('<') >> *qi::lit(' ') >> +(qi::char_ - (qi::lit('>') | qi::space)) >> *qi::lit(' ') >> qi::lit('>') >> qi::omit[*qi::space]
            // PARSE PROPERTIES
            >> +prop >> qi::omit[*(qi::char_ - qi::lit('}'))] >> qi::lit('}');

        // PROPERTY RULE
        prop = &qi::alpha >> -qi::lit("array") >> *qi::lit(' ') >> +(qi::char_ - qi::lit(' ')) >> +qi::lit(' ') >> +(
                    qi::char_ - (qi::space | qi::lit(';') | qi::lit('['))
                ) >> *qi::lit(' ') >> ((qi::lit('[') >> qi::int_ >> qi::lit(']')) | qi::attr(1)) >> qi::omit[*(qi::char_ - qi::lit(';'))] >> qi::lit(';') >> qi::omit[*qi::space];
    }

    qi::rule<Iterator, DXModel::XTemplate()> expr;
    qi::rule<Iterator, DXModel::XProperty()> prop;
};

template <typename Iterator>
struct NextDataIdentifierGrammar : public qi::grammar<Iterator, std::string()> {
    NextDataIdentifierGrammar(const std::vector<DXModel::XTemplate>& xtemplates) : NextDataIdentifierGrammar::base_type(expr) {
        for (auto && xtemplate : xtemplates) {
            templateParser.add(xtemplate.TemplateName);
        }

        expr = qi::omit[*(qi::char_ - templateParser)] >> qi::raw[templateParser];
    }
    qi::rule<Iterator, std::string()> expr;
    qi::symbols<> templateParser;
};

template <typename Iterator>
struct TextureFileNameGrammer : public qi::grammar<Iterator, std::string(), qi::space_type> {
    TextureFileNameGrammer() : TextureFileNameGrammer::base_type(expr)
    {
        expr = (qi::lit("TextureFilename") >> qi::lit('{') 
            >> qi::lit('"') >> *(qi::char_ - qi::lit('"')) >> qi::lit('"') >> qi::lit(';') 
            >> qi::lit('}')) | qi::attr("");
    }

    qi::rule<Iterator, std::string(), qi::space_type> expr;
};

//template <typename Iterator>
//struct MeshBodyGrammar : public qi::grammar<Iterator, DXModel::Mesh> {
//
//};

template<class Grammer>
void ParseDataBody(std::string::const_iterator& itr, const std::string::const_iterator end, bool hasOptional) {
    Grammer grammer;
    qi::phrase_parse(itr, end, grammer);
}

bool ParseDataCommand(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model);

void ForwardToBodyEnd(std::string::const_iterator& itr, const std::string::const_iterator end) {
    qi::phrase_parse(itr, end, qi::omit[*(qi::char_ - qi::lit('}'))] >> qi::lit('}'), qi::space);
}

void ExecAllCommandCurrentBody(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
    while (true) {
        ParseDataCommand(itr, end, model);
        bool isEnd = qi::phrase_parse(itr, end, qi::lit('}'), qi::space);

        if (isEnd) {
            break;
        }
    }
}

void ParseMeshMaterialList(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
    qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
    qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, model->mesh.meshMaterialList.nMaterials);
    qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, model->mesh.meshMaterialList.nFaceIndexes);
    qi::phrase_parse(itr, end, qi::int_ % ',' >> qi::lit(';'), qi::space , model->mesh.meshMaterialList.faceIndexes);
    qi::phrase_parse(itr, end, qi::lit(';'), qi::space);

    //parse optioal params;
    ExecAllCommandCurrentBody(itr, end, model);
}

void ParseMaterial(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
    DXModel::Material material;
    qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
    qi::phrase_parse(itr, end, qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;"), qi::space, material.faceColor);
    qi::phrase_parse(itr, end, qi::float_ >> qi::lit(';'), qi::space, material.power);
    qi::phrase_parse(itr, end, qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;"), qi::space, material.specularColor);
    qi::phrase_parse(itr, end, qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;"), qi::space, material.emissiveColor);
    TextureFileNameGrammer<std::remove_reference_t<decltype(itr)>> parser;
    qi::phrase_parse(itr, end, parser, qi::space, material.textureFileName);
    model->mesh.meshMaterialList.materials.push_back(material);
    ForwardToBodyEnd(itr, end);
}

void ParseMeshBody(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
    qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
    qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, model->mesh.nVertices);
	model->mesh.vertices.reserve(model->mesh.nVertices);
    qi::phrase_parse(itr, end, (qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';')) % ',' >> qi::lit(';') , qi::space, model->mesh.vertices);
	qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, model->mesh.nFaces);
	model->mesh.faces.reserve(model->mesh.nFaces);
	qi::phrase_parse(itr, end, (qi::omit[qi::int_] >> qi::lit(';') >> qi::int_ % ',' >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, model->mesh.faces);
	qi::phrase_parse(itr, end, qi::lit(';'), qi::space);

    ExecAllCommandCurrentBody(itr, end, model);
}

void ParseMeshTextureCoords(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
    qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
    qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, model->mesh.meshTextureCoords.nTextureCoords);
    qi::phrase_parse(itr, end, (qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, model->mesh.meshTextureCoords.textureCoords);
    qi::phrase_parse(itr, end, qi::lit(';'), qi::space);
    ForwardToBodyEnd(itr, end);
}

void ParseMeshVertexColors(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
    qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
    qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, model->mesh.meshVertexColors.nVertexColors);

    qi::phrase_parse(itr, end, qi::omit[qi::int_] >> qi::lit(';') >> (qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, model->mesh.meshVertexColors.vertexColors);
    
    qi::phrase_parse(itr, end, qi::lit(';'), qi::space);
    ForwardToBodyEnd(itr, end);
}

bool ParseDataCommand(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
#define BindCommand(func) std::bind(func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
    static std::unordered_map<std::string, std::function<void(std::string::const_iterator&, const std::string::const_iterator, DXModel*)>> commands = {
        std::make_pair("Mesh", BindCommand(ParseMeshBody)),
        std::make_pair("MeshMaterialList", BindCommand(ParseMeshMaterialList)),
        std::make_pair("Material", BindCommand(ParseMaterial)),
        std::make_pair("MeshTextureCoords", BindCommand(ParseMeshTextureCoords)),
        std::make_pair("MeshVertexColors", BindCommand(ParseMeshVertexColors))
    };
#undef BindCommand

    NextDataIdentifierGrammar<std::string::const_iterator> parser(model->templates);
    std::string templateIdentifier;
    bool r = qi::parse(itr, end, parser, templateIdentifier);

    if (r) {
        auto commandItr = commands.find(templateIdentifier);
        if (commandItr != commands.end()) {
            (*commandItr).second(itr, end, model);
            return true;
        }
        else {
            ERTREDebug(L"Found not undefined template identifier: %hs", templateIdentifier.c_str());
            // skip body;
            qi::phrase_parse(itr, end, *(qi::char_ - qi::lit('}')) >> qi::lit('}'), qi::space);
            return true;
        }
    }

    return false;
}

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

    {
        while(ParseDataCommand(itr, end, model));
    }

    return 0;
}
