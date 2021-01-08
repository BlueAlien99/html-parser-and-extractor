#ifndef PARSER_HPP
#define PARSER_HPP

#include "sources.hpp"
#include "node.hpp"
#include "lexer.hpp"
#include "token.hpp"

#include <vector>
#include <memory>

class HtmlParser{
public:
    HtmlParser(AbstractSource& source);
    std::shared_ptr<Element> parse();

private:
    void buildDoctype();
    void buildComment();
    void buildStartTag();
    void buildEndTag();
    void buildAttributes(std::shared_ptr<Element> elem);
    // void buildDoctype();
    // void buildDoctype();
    // void buildDoctype();

    void ignoreUntil(TokenType tokenType);


    std::unique_ptr<HtmlLexer> lexer_;
    std::shared_ptr<Element> dom_;
    std::vector<std::shared_ptr<Element> > open_nodes_;
};

#endif