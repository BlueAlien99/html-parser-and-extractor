#include "parser.hpp"

#include <unicode/unistr.h>

#include <iostream>

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
                if (utils::isReplaceableTagName(open_nodes_.back()->getName())) {
                    parseReplaceableContent();
                } else if (utils::isNonReplaceableTagName(open_nodes_.back()->getName())) {
                    parseNonReplaceableContent();
                }
                break;
            case TokenType::START_END_TAG:
                buildEndTag();
                break;
            default:
                // throw UnexpectedToken(lexer_->getToken());
                parseNormalContent();
        }
    }
    return dom_;
}

void HtmlParser::parseNormalContent() {
    Token token = lexer_->getToken();
    std::string content;
    while (token.getType() != TokenType::END_OF_FILE) {
        std::string temp;
        if (!utils::isValidNormalCharacterDataTT(token.getType())) {
            break;
        }
        if (utils::isCharacterReferenceStart(token.getType())) {
            content += buildCharacterReference();
        }
        token = lexer_->getToken();
        if (!utils::isCharacterReferenceStart(token.getType()) &&
            utils::isValidNormalCharacterDataTT(token.getType())) {
            if(token.getType() == TokenType::SPACE){
                content += " ";
            } else{
                content += token.getContent();
            }
            token = lexer_->buildNextToken();
        }
    }
    auto text = std::make_shared<TextContent>(content);
    open_nodes_.back()->insertNode(text);
}

void HtmlParser::parseReplaceableContent() {
    Token token = lexer_->getToken();
    std::string content;
    while (token.getType() != TokenType::END_OF_FILE) {
        std::string temp;
        if (token.getType() == TokenType::START_END_TAG) {
            temp += token.getContent();
            token = lexer_->buildNextToken();
            if (token.getType() == TokenType::STRING &&
                token.getContent() == open_nodes_.back()->getName()) {
                temp += token.getContent();
                token = lexer_->buildNextToken();
                if (token.getType() == TokenType::SPACE || token.getType() == TokenType::SLASH) {
                    auto text = std::make_shared<TextContent>(content);
                    open_nodes_.back()->insertNode(text);
                    open_nodes_.pop_back();
                    ignoreUntil(TokenType::END_TAG);
                    return;
                } else if (token.getType() == TokenType::END_TAG ||
                           token.getType() == TokenType::END_VOID_TAG) {
                    auto text = std::make_shared<TextContent>(content);
                    open_nodes_.back()->insertNode(text);
                    open_nodes_.pop_back();
                    lexer_->buildNextTokenNoWs();
                    return;
                }
            }
        }
        content += temp;
        if (utils::isCharacterReferenceStart(token.getType())) {
            content += buildCharacterReference();
        }
        token = lexer_->getToken();
        if (!utils::isCharacterReferenceStart(token.getType()) &&
            token.getType() != TokenType::START_END_TAG) {
            content += token.getContent();
            token = lexer_->buildNextToken();
        }
    }
}

void HtmlParser::parseNonReplaceableContent() {
    Token token = lexer_->getToken();
    while (token.getType() != TokenType::END_OF_FILE) {
        ignoreUntil(TokenType::START_END_TAG);
        token = lexer_->getToken();
        if (token.getType() == TokenType::STRING &&
            token.getContent() == open_nodes_.back()->getName()) {
            token = lexer_->buildNextToken();
            if (token.getType() == TokenType::SPACE || token.getType() == TokenType::SLASH) {
                open_nodes_.pop_back();
                ignoreUntil(TokenType::END_TAG);
                return;
            } else if (token.getType() == TokenType::END_TAG ||
                       token.getType() == TokenType::END_VOID_TAG) {
                open_nodes_.pop_back();
                lexer_->buildNextTokenNoWs();
                return;
            }
        }
    }
}

void HtmlParser::buildDoctype() {
    lexer_->buildNextTokenNoWs();
    ignoreUntil(TokenType::END_TAG);
}

void HtmlParser::buildComment() {
    lexer_->buildNextTokenNoWs();
    ignoreUntil(TokenType::COMMENT_END);
}

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
        if (token.getType() == TokenType::SINGLE_QUOTE ||
            token.getType() == TokenType::DOUBLE_QUOTE) {
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
    while (lexer_->getToken().getType() != tokenType &&
           lexer_->getToken().getType() != TokenType::END_OF_FILE) {
        lexer_->buildNextTokenNoWs();
    }
    lexer_->buildNextTokenNoWs();
}

std::string HtmlParser::buildCharacterReference() {
    Token token = lexer_->getToken();
    TokenType type = token.getType();
    std::string ret = token.getContent();
    token = lexer_->buildNextToken();
    if (token.getType() == TokenType::STRING) {
        if (type == TokenType::START_HEX_CHAR_REF || type == TokenType::START_DECIMAL_CHAR_REF) {
            int val;
            int base = 10;
            if (type == TokenType::START_HEX_CHAR_REF) {
                base = 16;
            }
            try {
                val = std::stoi(token.getContent(), nullptr, base);
            } catch (...) {
                return ret;
            }
            ret = "";
            icu::UnicodeString uni_str((UChar32)val);
            uni_str.toUTF8String(ret);
        } else if (type == TokenType::START_NAMED_CHAR_REF) {
            ret = token.getContent();
            if (ret == "amp") {
                ret = "&";
            } else if (ret == "lt") {
                ret = "<";
            } else if (ret == "gt") {
                ret = ">";
            }
        }
        token = lexer_->buildNextToken();
        if (token.getType() == TokenType::END_CHAR_REF) {
            lexer_->buildNextToken();
        }
        return ret;
    }
    return ret;
}