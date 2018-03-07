#pragma once

#include "Common.h"
#include "DXParseCommon.h"
#include "DXGrammers.h"

#include <unordered_map>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/variant.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct DXRootParser;

#define DECLARE_NODE(name) template<class Tag> \
struct name; \
struct Tag##name;


DECLARE_NODE(MeshParser);
DECLARE_NODE(MeshTextureCoordsParser);
DECLARE_NODE(MaterialParser);
DECLARE_NODE(MaterialListParser);
DECLARE_NODE(MeshVertexColorParser);
DECLARE_NODE(MeshNormalParser);

struct DXRootParser;
struct TagDXRootParser;

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

    // Compiler bug
    /*const std::unordered_map<std::string, VisitNodesT> VisitMap = {
        std::make_pair(VisitNodes::Identifier, VisitNodes{})...
    };*/


    /////////// substitute codes. fuck. ////////////
    std::unordered_map<std::string, VisitNodesT> VisitMap;
    template<class T, class... Args>
    void pushNode(const T& t, Args&&... args) {
        VisitMap[t.Identifier] = t;
        pushNode(std::forward<Args>(args)...);
    }
    template<class T>
    void pushNode(const T& t) {
        VisitMap[t.Identifier] = t;
    }
    ParserBase() {
        pushNode(VisitNodes{}...);
    }
    /////////// ---------------------- ////////////

    template<class Visitor>
    void ParseOptionalBody
    (std::string::const_iterator& itr, const std::string::const_iterator end, const Visitor& visitor) {
        std::vector<std::string> keys(VisitMap.size());
        std::transform(VisitMap.begin(), VisitMap.end(), keys.begin(), [](const auto& pair) { return pair.first; });
        NextDataIdentifier<std::string::const_iterator> nextIdentifier(keys);
        while (true) {
            std::string templateIdentifier;
            if (qi::parse(itr, end, nextIdentifier, templateIdentifier)) {
                boost::apply_visitor(visitor, VisitMap.at(templateIdentifier));
            }
            else {
                // skip body
                ERTREDebug(L"Found not undefined template identifier: %hs\n", templateIdentifier.c_str());
                qi::phrase_parse(itr, end, *(qi::char_ - qi::lit('}')) >> qi::lit('}'), qi::space);
            }
            bool isEnd = qi::phrase_parse(itr, end, qi::lit('}'), qi::space);
            if (isEnd || itr == end) {
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
struct MaterialParser<TagMaterialListParser> : ParserBase<> {
    static constexpr auto Identifier = "Material";
    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, std::vector<DXModel::Material>& materials) {
        DXModel::Material material;
        MaterialGrammar<std::string::const_iterator> grammar;
        qi::phrase_parse(itr, end, grammar, qi::space, material);
        materials.push_back(material);
        ParseOptionalBody(itr, end);
    }
};

template<>
struct MaterialListParser<TagMeshParser> : ParserBase<MaterialParser<TagMaterialListParser>> {
    static constexpr auto Identifier = "MeshMaterialList";
    struct Visitor : VisitorBase<DXModel::MeshMaterialList> {
        using VisitorBase<DXModel::MeshMaterialList>::VisitorBase;
        void operator()(MaterialParser<TagMaterialListParser> parser) const {
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
struct MeshTextureCoordsParser<TagMeshParser> : ParserBase<> {
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
struct MeshVertexColorParser<TagMeshParser> : ParserBase<> {
    static constexpr auto Identifier = "MeshVertexColors";
    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel::MeshVertexColors& meshVertexColors) {
        qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, meshVertexColors.nVertexColors);
        qi::phrase_parse(itr, end, (qi::omit[qi::int_] >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, meshVertexColors.vertexColors);
        qi::phrase_parse(itr, end, qi::lit(';'), qi::space);
        ParseOptionalBody(itr, end);
    }
};

template<>
struct MeshNormalParser<TagMeshParser> : ParserBase<> {
    static constexpr auto Identifier = "MeshNormals";
    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel::MeshNormals& meshNormals) {
        qi::phrase_parse(itr, end, qi::lit('{'), qi::space);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, meshNormals.nNormals);
        meshNormals.normals.reserve(meshNormals.nNormals);
        qi::phrase_parse(itr, end, (qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, meshNormals.normals);
        qi::phrase_parse(itr, end, qi::int_ >> qi::lit(';'), qi::space, meshNormals.nFaceNormals);
        qi::phrase_parse(itr, end, (qi::int_ >> qi::lit(';') >> (qi::int_ % ',') >> qi::lit(';')) % ',' >> qi::lit(';'), qi::space, meshNormals.faceNormals);
        ParseOptionalBody(itr, end);
    }
};

template<>
struct MeshParser<TagDXRootParser> : 
    ParserBase<MeshTextureCoordsParser<TagMeshParser>, MeshVertexColorParser<TagMeshParser>, MaterialListParser<TagMeshParser>, MeshNormalParser<TagMeshParser>> {

    static constexpr auto Identifier = "Mesh";
    struct Visitor : VisitorBase<DXModel::Mesh> {
        using VisitorBase<DXModel::Mesh>::VisitorBase;
        void operator()(MeshTextureCoordsParser<TagMeshParser> parser) const {
            parser.Parse(_itr, end, _1.meshTextureCoords);
        }
        void operator()(MeshVertexColorParser<TagMeshParser> parser) const {
            parser.Parse(_itr, end, _1.meshVertexColors);
        }
        void operator()(MaterialListParser<TagMeshParser> parser) const {
            parser.Parse(_itr, end, _1.meshMaterialList);
        }
        void operator()(MeshNormalParser<TagMeshParser> parser) const {
            parser.Parse(_itr, end, _1.meshNormals);
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

struct DXRootParser : ParserBase<MeshParser<TagDXRootParser>> {
    struct Visitor : VisitorBase<DXModel*> {
        using VisitorBase<DXModel*>::VisitorBase;
        void operator()(MeshParser<TagDXRootParser> parser) const {
            parser.Parse(_itr, end, _1->mesh);
        }
    };

    void Parse(std::string::const_iterator& itr, const std::string::const_iterator end, DXModel* model) {
        {
            XHeaderGrammar<std::string::const_iterator> parser;
            qi::parse(itr, end, parser, model->header);
        }

        {
            XTemplateGrammar<std::string::const_iterator> parser;
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
        ParseOptionalBody(itr, end, Visitor(itr, end, model));
    }
};

