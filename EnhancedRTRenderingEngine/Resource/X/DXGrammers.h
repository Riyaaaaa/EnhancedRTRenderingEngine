#pragma once

#include "DXParseCommon.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct FileFormatSymbol : public qi::symbols<char, DXModel::FileFormat> {
    FileFormatSymbol()
    {
        add("txt", DXModel::FileFormat::TEXT)
            ("bin", DXModel::FileFormat::BINARY)
            ("tzip", DXModel::FileFormat::TZIP)
            ("bzip", DXModel::FileFormat::BZIP);
    }
};

template<typename Iterator>
struct XHeaderGrammar : public qi::grammar<Iterator, DXModel::XHeader()> {
    XHeaderGrammar() : XHeaderGrammar::base_type(expr) {
        expr = +(qi::char_ - qi::lit(' '))
            >> qi::lit(' ') >> +(qi::char_ - (format | qi::lit(' ')))
            >> format >> qi::lit(' ')
            >> qi::int_
            >> qi::omit[*qi::space];

    }
    FileFormatSymbol format;
    qi::rule<Iterator, DXModel::XHeader()> expr;
};

template <typename Iterator>
struct XTemplateGrammar : public qi::grammar<Iterator, DXModel::XTemplate(), qi::space_type> {
    XTemplateGrammar() : XTemplateGrammar::base_type(expr) {
        expr = qi::no_skip[qi::lit("template") >> +qi::lit(' ') >> +(qi::char_ - (qi::lit('{') | qi::space)) >> qi::omit[*qi::space] >> qi::lit('{')]
            // PARSE UID
            >> qi::lit('<') >> +(qi::char_ - qi::lit('>')) >> qi::lit('>')
            // PARSE PROPERTIES
            >> +prop >> qi::omit[*(qi::char_ - qi::lit('}'))] >> qi::lit('}');

        // PROPERTY RULE
        prop = &qi::alpha >> -qi::lit("array") >> qi::lexeme[+(qi::char_ - qi::lit(' '))] >> +(
            qi::char_ - (qi::lit(';') | qi::lit('['))
            ) >> ((qi::lit('[') >> qi::int_ >> qi::lit(']')) | qi::attr(1)) >> qi::omit[*(qi::char_ - qi::lit(';'))] >> qi::lit(';');
    }

    qi::rule<Iterator, DXModel::XTemplate(), qi::space_type> expr;
    qi::rule<Iterator, DXModel::XProperty(), qi::space_type> prop;
};

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

template <typename Iterator>
struct MaterialGrammar : public qi::grammar<Iterator, DXModel::Material(), qi::space_type> {
    MaterialGrammar() : MaterialGrammar::base_type(expr)
    {
        expr %= qi::lit('{') >> faceColorExpr >> powerExpr >> specularColorExpr >> emissiveColorExpr >> textureExpr;
        faceColorExpr %= qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;");
        powerExpr %= qi::float_ >> qi::lit(';');
        specularColorExpr %= qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;");
        emissiveColorExpr %= qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(';') >> qi::float_ >> qi::lit(";;");
    }

    qi::rule<Iterator, DXModel::Material(), qi::space_type> expr;
    qi::rule<Iterator, Vector4D(), qi::space_type> faceColorExpr;
    qi::rule<Iterator, float(), qi::space_type> powerExpr;
    qi::rule<Iterator, Vector3D(), qi::space_type> specularColorExpr;
    qi::rule<Iterator, Vector3D(), qi::space_type> emissiveColorExpr;
    TextureFileNameGrammer<Iterator> textureExpr;
};

