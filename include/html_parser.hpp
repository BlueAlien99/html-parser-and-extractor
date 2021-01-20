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
    std::unique_ptr<Node> parse();
    std::unique_ptr<Node> parseSafe(AbstractSource& source);
    std::size_t getOpenNodesSize() const;

private:
    void parseNormalContent();
    void parseReplaceableContent();
    void parseNonReplaceableContent();

    void buildDoctype();
    void buildComment();
    void buildStartTag();
    void buildEndTag();

    std::unique_ptr<HtmlElement> buildAttributes(std::unique_ptr<HtmlElement> elem);

    std::string buildAttributeNameOrUnquoted(bool is_name);
    std::string buildAttributeValueQuoted(TokenType quote);
    std::string buildCharacterReference();

    void ignoreUntil(TokenType tokenType);
    void closeNode();

    unsigned int getNextId();

    std::unique_ptr<HtmlLexer> lexer_;
    std::vector<std::unique_ptr<HtmlElement> > open_nodes_;

    unsigned int next_id_;
    bool parsed_;
};

#endif