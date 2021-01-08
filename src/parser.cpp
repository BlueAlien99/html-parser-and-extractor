#include "parser.hpp"

#include "exceptions.hpp"
#include "utils.hpp"

HtmlParser::HtmlParser(AbstractSource& source) {
    lexer_ = std::make_unique<HtmlLexer>(source);
    dom_ = std::make_shared<Element>("_dom_");
    open_nodes_.push_back(dom_);
}

std::shared_ptr<Element> HtmlParser::parse() {
    lexer_->buildNextTokenNoWs();
    while (lexer_->getToken().getType() != TokenType::END_OF_FILE) {
        Token token = lexer_->getToken();

        switch (token.getType()) {
            case TokenType::START_DOCTYPE:
                buildDoctype();
                break;
            case TokenType::COMMENT_START:
                buildComment();
                break;
            case TokenType::START_START_TAG:
                buildStartTag();
                break;
            case TokenType::START_END_TAG:
                buildEndTag();
                break;
            default:
                throw UnexpectedToken(token);
        }
    }
    return dom_;
}

void HtmlParser::buildDoctype() { ignoreUntil(TokenType::END_TAG); }

void HtmlParser::buildComment() { ignoreUntil(TokenType::COMMENT_END); }

void HtmlParser::buildStartTag() {
    Token token = lexer_->buildNextTokenNoWs();
    if (token.getType() == TokenType::STRING) {
        std::shared_ptr<Element> elem = std::make_shared<Element>(token.getContent());
        open_nodes_.back()->insertNode(elem);
        lexer_->buildNextTokenNoWs();

        buildAttributes(elem);

        token = lexer_->getToken();
        if (token.getType() == TokenType::END_TAG) {
            if (!utils::isVoidElement(elem->getName())) {
                open_nodes_.push_back(elem);
            }
            lexer_->buildNextTokenNoWs();
            return;
        }
        if (token.getType() == TokenType::END_VOID_TAG) {
            lexer_->buildNextTokenNoWs();
            return;
        }
    }
    throw UnexpectedToken(token);
}

void HtmlParser::buildEndTag() {
    Token token = lexer_->buildNextTokenNoWs();
    if (token.getType() == TokenType::STRING) {
        std::string name = token.getContent();
        std::string expected = open_nodes_.back()->getName();
        if (expected == name) {
            open_nodes_.pop_back();
        } else {
            throw MismatchedTags(token, expected);
        }
        token = lexer_->buildNextTokenNoWs();
        if (token.getType() == TokenType::END_TAG) {
            lexer_->buildNextTokenNoWs();
            return;
        }
    }
    throw UnexpectedToken(token);
}

void HtmlParser::buildAttributes(std::shared_ptr<Element> elem) {
    Token token = lexer_->getToken();
    while (token.getType() != TokenType::END_TAG && token.getType() != TokenType::END_VOID_TAG &&
           token.getType() != TokenType::END_OF_FILE) {
        std::string name = buildAttributeNameOrUnquoted(true);
        token = lexer_->getToken();
        if (token.getType() != TokenType::EQUALS) {
            elem->addAttribute(name, "");
            continue;
        }
        token = lexer_->buildNextTokenNoWs();
        std::string value;
        switch (token.getType()) {
            case TokenType::SINGLE_QUOTE:
                lexer_->buildNextToken();
                value = buildAttributeValueQuoted(TokenType::SINGLE_QUOTE);
                break;
            case TokenType::DOUBLE_QUOTE:
                lexer_->buildNextToken();
                value = buildAttributeValueQuoted(TokenType::DOUBLE_QUOTE);
                break;
            default:
                if (utils::isValidAttrValueUnquotedTT(token.getType())) {
                    value = buildAttributeNameOrUnquoted(false);
                } else {
                    throw UnexpectedToken(token);
                }
        }
        token = lexer_->getToken();
        if(token.getType() == TokenType::SINGLE_QUOTE || token.getType() == TokenType::DOUBLE_QUOTE){
            token = lexer_->buildNextTokenNoWs();
        }
        elem->addAttribute(name, value);
    }
}

std::string HtmlParser::buildAttributeNameOrUnquoted(bool is_name) {
    std::string name;
    Token token = lexer_->getToken();
    while ((is_name && utils::isValidAttrNameTT(token.getType())) ||
           (!is_name && utils::isValidAttrValueUnquotedTT(token.getType()))) {
        name += token.getContent();
        token = lexer_->buildNextToken();
    }
    if (token.getType() == TokenType::SPACE) {
        lexer_->buildNextTokenNoWs();
    }
    return name;
}

std::string HtmlParser::buildAttributeValueQuoted(TokenType quote) {
    std::string value;
    Token token = lexer_->getToken();
    while (utils::isValidAttrValueQuotedTT(token.getType(), quote)) {
        value += token.getContent();
        token = lexer_->buildNextToken();
    }
    if (token.getType() == TokenType::SPACE) {
        lexer_->buildNextTokenNoWs();
    }
    return value;
}

void HtmlParser::ignoreUntil(TokenType tokenType) {
    do {
        lexer_->buildNextTokenNoWs();
    } while (lexer_->getToken().getType() != tokenType &&
             lexer_->getToken().getType() != TokenType::END_OF_FILE);
    lexer_->buildNextTokenNoWs();
}