#pragma once

#include "DXParseCommon.h"
#include "DXGrammers.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/variant.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct NextDataIdentifier : public qi::grammar<Iterator, std::string()> {
    NextDataIdentifier(const std::vector<std::string>& identifiers) : NextDataIdentifier::base_type(expr) {
        for (auto && identifier : identifiers) {
            templateParser.add(identifier);
        }

        expr = qi::omit[*(qi::char_ - templateParser)] >> qi::raw[templateParser];
    }
    qi::rule<Iterator, std::string()> expr;
    qi::symbols<> templateParser;
};

struct RootParser;

template<class Parent>
struct MeshParser;

template<class Parent>
struct MeshTextureCoordsParser;

template<class Parent>
struct MaterialParser;

template<class Parent>
struct MaterialListParser;

template<class Parent>
struct MeshVertexColorParser;

template<class Arg>
struct VisitorBase : boost::static_visitor<void> {
    VisitorBase(std::string::const_iterator& itr, const std::string::const_iterator end, Arg& arg) :
        _itr(itr),
        end(end),
        _1(arg)
    {}
    std::string::const_iterator& _itr;
    const std::string::const_iterator end;
    Arg& _1;
};

template<class... VisitNodes>
struct ParserBase {
    typedef boost::variant<VisitNodes...> VisitNodesT;
    const std::unordered_map<std::string, VisitNodesT> VisitMap = {
        std::make_pair(VisitNodes::Identifier, VisitNodes{})...
        //std::make_pair("", VisitNodes{})...
    };

    template<class Visitor>
    void ParseOptionalBody
    (std::string::const_iterator& itr, const std::string::const_iterator end, const Visitor& visitor) {
        std::vector<std::string> keys(VisitMap.size());
        std::transform(VisitMap.begin(), VisitMap.end(), keys.begin(), [](const auto& pair) { return pair.first; });
        NextDataIdentifier<std::string::const_iterator> nextIdentifier(keys);
        std::string templateIdentifier;

        while (true) {
            if (qi::parse(itr, end, nextIdentifier, templateIdentifier)) {
                boost::apply_visitor(visitor, VisitMap.at(templateIdentifier));
            }
            bool isEnd = qi::phrase_parse(itr, end, qi::lit('}'), qi::space);
            if (isEnd) {
                break;
            }
        }
    }
};

template<>
struct ParserBase<> {
    void ParseOptionalBody(std::string::const_iterator& itr, const std::string::const_iterator end) {
        qi::phrase_parse(itr, end, qi::omit[*(qi::char_ - qi::lit('}'))] >> qi::lit('}'), qi::space);
    }
};

template<>
struct MaterialParser<MaterialListParser<MeshParser<RootParser>>> : ParserBase<> {
    static constexpr auto Identifier = "Material";
    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, std::vector<DXModel::Material>& materials) {
        DXModel::Material material;
        qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
        qi::phrase_parse(itr, end, qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;"), qi::space, material.faceColor);
        qi::phrase_parse(itr, end, qi::float_ >> qi::lit(';'), qi::space, material.power);
        qi::phrase_parse(itr, end, qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;"), qi::space, material.specularColor);
        qi::phrase_parse(itr, end, qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;"), qi::space, material.emissiveColor);
        TextureFileNameGrammer<std::remove_reference_t<decltype(itr)>> parser;
        qi::phrase_parse(itr, end, parser, qi::space, material.textureFileName);
        materials.push_back(material);
        ParseOptionalBody(itr, end);
    }
};

template<>
struct MaterialListParser<MeshParser<RootParser>> : ParserBase<MaterialParser<MaterialListParser<MeshParser<RootParser>>>> {
    static constexpr auto Identifier = "Mesh";
    struct Visitor : VisitorBase<DXModel::MeshMaterialList> {
        using VisitorBase<DXModel::MeshMaterialList>::VisitorBase;
        void operator()(MaterialParser<MaterialListParser<MeshParser<RootParser>>> parser) const {
            parser.Parse(_itr, end, _1.materials);
        }
    };

    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel::MeshMaterialList& meshMaterialList) {
        qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, meshMaterialList.nMaterials);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, meshMaterialList.nFaceIndexes);
        qi::phrase_parse(itr, end, qi::int_ % ',' >> qi::lit(';'), qi::space, meshMaterialList.faceIndexes);
        qi::phrase_parse(itr, end, qi::lit(';'), qi::space);

        ParseOptionalBody(itr, end, Visitor(itr, end, meshMaterialList));
    }
};

template<>
struct MeshTextureCoordsParser<MeshParser<RootParser>> : ParserBase<> {
    static constexpr auto Identifier = "MeshTextureCoords";
    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel::MeshTextureCoords& meshTextureCoords) {
        qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, meshTextureCoords.nTextureCoords);
        qi::phrase_parse(itr, end, (qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, meshTextureCoords.textureCoords);
        qi::phrase_parse(itr, end, qi::lit(';'), qi::space);
        ParseOptionalBody(itr, end);
    }
};

template<>
struct MeshVertexColorParser<MeshParser<RootParser>> : ParserBase<> {
    static constexpr auto Identifier = "MeshVertexColors";
    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel::MeshVertexColors& meshVertexColors) {
        qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, meshVertexColors.nVertexColors);
        qi::phrase_parse(itr, end, qi::omit[qi::int_] >> qi::lit(';') >> (qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, meshVertexColors.vertexColors);
        qi::phrase_parse(itr, end, qi::lit(';'), qi::space);
        ParseOptionalBody(itr, end);
    }
};

template<>
struct MeshParser<RootParser> : 
    ParserBase<MeshTextureCoordsParser<MeshParser<RootParser>>, MeshVertexColorParser<MeshParser<RootParser>>, MaterialListParser<MeshParser<RootParser>>> {

    static constexpr auto Identifier = "Mesh";
    struct Visitor : VisitorBase<DXModel::Mesh> {
        using VisitorBase<DXModel::Mesh>::VisitorBase;
        void operator()(MeshTextureCoordsParser<MeshParser<RootParser>> parser) const {
            parser.Parse(_itr, end, _1.meshTextureCoords);
        }
        void operator()(MeshVertexColorParser<MeshParser<RootParser>> parser) const {
            parser.Parse(_itr, end, _1.meshVertexColors);
        }
        void operator()(MaterialListParser<MeshParser<RootParser>> parser) const {
            parser.Parse(_itr, end, _1.meshMaterialList);
        }
    };

    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel::Mesh& mesh) {
        qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, mesh.nVertices);
        mesh.vertices.reserve(mesh.nVertices);
        qi::phrase_parse(itr, end, (qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, mesh.vertices);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, mesh.nFaces);
        mesh.faces.reserve(mesh.nFaces);
        qi::phrase_parse(itr, end, (qi::omit[qi::int_] >> qi::lit(';') >> qi::int_ % ',' >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, mesh.faces);
        qi::phrase_parse(itr, end, qi::lit(';'), qi::space);

        ParseOptionalBody(itr, end, Visitor(itr, end, mesh));
    }
};

struct RootParser : ParserBase<MeshParser<RootParser>> {
    struct Visitor : VisitorBase<DXModel*> {
        using VisitorBase<DXModel*>::VisitorBase;
        void operator()(MeshParser<RootParser> parser) const {
            parser.Parse(_itr, end, _1->mesh);
        }
    };

    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
        ParseOptionalBody(itr, end, Visitor(itr, end, model));
    }
};

