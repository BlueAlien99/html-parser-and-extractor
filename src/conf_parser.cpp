#include "conf_parser.hpp"

#include <iostream>

#include "exceptions.hpp"
#include "utils.hpp"

ConfParser::ConfParser(AbstractSource& source) {
    lexer_ = std::make_unique<ConfLexer>(source);
    conf_ = std::make_shared<ConfObject>();
    current_conf_ = conf_;
}

std::shared_ptr<ConfObject> ConfParser::parse() {
    lexer_->buildNextTokenNoWs();
    while (lexer_->getToken().getType() != TokenType::END_OF_FILE) {
        Token token = lexer_->getToken();

        switch (token.getType()) {
            case TokenType::STRING:
                current_conf_->setTag(token.getContent());
                lexer_->buildNextToken();
                break;
            case TokenType::ID:
            case TokenType::CLASS:
                buildClassOrId();
                break;
            case TokenType::START_ATTRIBUTE:
                buildAttribute();
                break;
            case TokenType::START_RANGE:
                buildRange();
                break;
            case TokenType::SPACE:
                current_conf_ = current_conf_->createNextConf();
                lexer_->buildNextTokenNoWs();
                break;
            default:
                throw UnexpectedToken(token);
        }
    }
    return conf_;
}

std::shared_ptr<ConfObject> ConfParser::parseSafe(AbstractSource& source) {
    try {
        return parse();
    } catch (UnexpectedToken& err) {
        Position pos = err.getToken().getPosition();
        std::cout << std::endl
                  << "Error at " << pos.line << ':' << pos.column << ": "
                  << source.getTextAtPosition(pos) << std::endl;
        std::cout << "Token Content: " << err.getToken().getContent() << std::endl;
        return nullptr;
    }
}

void ConfParser::buildClassOrId() {
    TokenType class_or_id = lexer_->getToken().getType();
    Token token = lexer_->buildNextToken();
    std::string name;
    while (utils::isValidClassOrIdTT(token.getType()) &&
           token.getType() != TokenType::END_OF_FILE) {
        name += token.getContent();
        token = lexer_->buildNextToken();
    }
    if (!name.empty()) {
        if (class_or_id == TokenType::CLASS) {
            current_conf_->addClass(name);
        } else if (class_or_id == TokenType::ID) {
            current_conf_->addId(name);
        } else {
            throw UnexpectedToken(class_or_id);
        }
    }
}

void ConfParser::buildAttribute() {
    Token token = lexer_->buildNextTokenNoWs();
    std::string name;
    while (utils::isValidAttrNameTT(token.getType()) &&
           token.getType() != TokenType::START_ATTRIBUTE &&
           token.getType() != TokenType::END_ATTRIBUTE) {
        name += token.getContent();
        token = lexer_->buildNextToken();
    }
    if (token.getType() == TokenType::SPACE) {
        token = lexer_->buildNextTokenNoWs();
    }
    if (token.getType() == TokenType::END_ATTRIBUTE) {
        lexer_->buildNextToken();
        current_conf_->addAttribute(name);
        return;
    } else if (token.getType() == TokenType::EQUALS) {
        token = lexer_->buildNextTokenNoWs();
        TokenType quote = token.getType();
        std::string value;
        if (quote == TokenType::SINGLE_QUOTE || quote == TokenType::DOUBLE_QUOTE) {
            token = lexer_->buildNextToken();
            while (utils::isValidAttrValueQuotedTT(token.getType(), quote)) {
                value += token.getContent();
                token = lexer_->buildNextToken();
            }
            if (token.getType() == quote) {
                token = lexer_->buildNextTokenNoWs();
                if (token.getType() == TokenType::END_ATTRIBUTE) {
                    current_conf_->addAttributeValue(name, value);
                    lexer_->buildNextToken();
                    return;
                }
            }
        }
    }
    throw UnexpectedToken(token);
}

void ConfParser::buildRange() {
    bool separator = false;
    Token token = lexer_->buildNextTokenNoWs();
    std::pair<std::string, int> from = buildRangeBoundary();
    token = lexer_->getToken();
    if (token.getType() == TokenType::RANGE_SEPARATOR) {
        separator = true;
        token = lexer_->buildNextTokenNoWs();
    }
    std::pair<std::string, int> to = buildRangeBoundary();
    token = lexer_->getToken();
    if (token.getType() == TokenType::END_RANGE) {
        token = lexer_->buildNextToken();
        if (!separator) {
            current_conf_->addRange(from.second, from.second);
        } else {
            if (from.first == "") {
                from.second = 0;
            }
            if (to.first == "") {
                to.second = -1;
            }
            current_conf_->addRange(from.second, to.second);
        }
    } else {
        throw UnexpectedToken(token);
    }
}

std::pair<std::string, int> ConfParser::buildRangeBoundary() {
    std::string str;
    Token token = lexer_->getToken();
    if (token.getType() == TokenType::DASH || token.getType() == TokenType::STRING) {
        str += token.getContent();
        token = lexer_->buildNextTokenNoWs();
        if (token.getType() == TokenType::STRING) {
            str += token.getContent();
            token = lexer_->buildNextTokenNoWs();
        }
        try {
            int str_int = std::stoi(str);
            return {str, str_int};
        } catch (...) {
            throw UnexpectedToken(token);
        }
    }
    return {"", 0};
}