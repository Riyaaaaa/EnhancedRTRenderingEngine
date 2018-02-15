#include "stdafx.h"
#include "StringParser.h"

#include <iostream>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <boost/fusion/include/std_pair.hpp>

#include <boost/format.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

#define TRANSFORM_STRUCT_MEMBER(ign, name, member) (decltype(name::member), member)
#define FUSION_ADAPT_STRUCT_AUTO(name, members) \
  BOOST_FUSION_ADAPT_STRUCT(name, BOOST_PP_SEQ_FOR_EACH(TRANSFORM_STRUCT_MEMBER, name, members))

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
        expr = qi::omit[*qi::space] >> qi::lit("template") >> *qi::lit(' ') >> +(qi::char_ - (qi::lit('{') | qi::space)) >> qi::omit[*qi::space] >> qi::lit('{') >> qi::omit[*qi::space]
            >> qi::lit('<') >> *qi::lit(' ') >> +(qi::char_ - (qi::lit('>') | qi::space)) >> *qi::lit(' ') >> qi::lit('>') >> qi::omit[*qi::space]
            >> +(+(qi::char_ - qi::lit(' ')) >> +qi::lit(' ') >> +(qi::char_ - (qi::space | qi::lit(';'))) >> *qi::lit(' ') >> qi::lit(';') >> qi::omit[*qi::space])
            >> qi::lit('}');
    }

    qi::rule<Iterator, DXModel::XTemplate()> expr;
};

template <typename Iterator>
struct NextDataIdentifierGrammar : public qi::grammar<Iterator, std::string()> {
    NextDataIdentifierGrammar(const std::vector<DXModel::XTemplate>& xtemplates) : NextDataIdentifierGrammar::base_type(expr) {
        for (auto && xtemplate : xtemplates) {
            templateParser |= ascii::string(xtemplate.TemplateName);
        }

        expr = *(qi::char_ - templateParser) >> templateParser;
    }
    qi::rule<Iterator, std::string()> expr;
    qi::rule<Iterator, std::string()> templateParser;
};

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
        NextDataIdentifierGrammar<std::string::iterator> parser(model->templates);
        std::string templateIdentifier;
        while (true) {
            bool r = qi::parse(itr, end, parser, templateIdentifier);

            if (r) {
                std::cout << templateIdentifier << std::endl;
                // TODO: parse data
            }
            if (!r || itr == end) {
                break;
            }
        }
    }

    return 0;
}
