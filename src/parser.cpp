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
    //TODO
}

void HtmlParser::ignoreUntil(TokenType tokenType) {
    do {
        lexer_->buildNextTokenNoWs();
    } while (lexer_->getToken().getType() != tokenType &&
             lexer_->getToken().getType() != TokenType::END_OF_FILE);
    lexer_->buildNextTokenNoWs();
}