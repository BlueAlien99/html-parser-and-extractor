#ifndef HTML_PARSER_HPP
#define HTML_PARSER_HPP

#include <memory>
#include <vector>

#include "lexer.hpp"
#include "node.hpp"
#include "sources.hpp"
#include "token.hpp"

class HtmlParser {
public:
    HtmlParser(AbstractSource& source);
    std::shared_ptr<HtmlElement> parse();
    std::shared_ptr<HtmlElement> parseSafe(AbstractSource& source);
    std::vector<std::shared_ptr<HtmlElement> > getOpenNodes();

private:
    void parseNormalContent();
    void parseReplaceableContent();
    void parseNonReplaceableContent();

    void buildDoctype();
    void buildComment();
    void buildStartTag();
    void buildEndTag();
    void buildAttributes(std::shared_ptr<HtmlElement> elem);
    std::string buildAttributeNameOrUnquoted(bool is_name);
    std::string buildAttributeValueQuoted(TokenType quote);
    std::string buildCharacterReference();

    void ignoreUntil(TokenType tokenType);

    std::unique_ptr<HtmlLexer> lexer_;
    std::shared_ptr<HtmlElement> dom_;
    std::vector<std::shared_ptr<HtmlElement> > open_nodes_;
};

#endif