#pragma once

#include "DXParseCommon.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct FileFormatSymbol_ : public qi::symbols<char, DXModel::FileFormat> {
    FileFormatSymbol_()
    {
        add("txt", DXModel::FileFormat::TEXT)
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
