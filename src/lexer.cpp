#include "lexer.hpp"

#include <string>

#include "token.hpp"

#include <iostream>

AbstractLexer::AbstractLexer(AbstractSource& source, Token token)
    : source_(source), token_(token) {}

Token AbstractLexer::buildNextToken() {
    token_ = buildToken();
    // std::cout<<token_.getContent();
    return token_;
}

Token AbstractLexer::buildNextTokenNoWs() {
    do {
        buildNextToken();
    } while (token_.getType() == TokenType::SPACE);
    return token_;
}

Token AbstractLexer::getToken() const { return token_; }

Token AbstractLexer::tryToBuildSpace() {
    char c = source_.getChar();
    Position pos = source_.getPosition();
    if (isspace(c)) {
        std::string str(1, c);
        while (isspace(source_.advance())) {
            str += source_.getChar();
        }
        return Token(TokenType::SPACE, pos, str);
    }
    return Token(TokenType::UNDEFINED, pos, std::string(1, c));
}

Token AbstractLexer::tryToBuildString() {
    char c = source_.getChar();
    Position pos = source_.getPosition();
    if (isalnum(c) || (signed char)c < 0) {
        std::string str(1, c);
        char nc = source_.advance();
        while (isalnum(nc) || (signed char)nc < 0) {
            str += nc;
            nc = source_.advance();
        }
        return Token(TokenType::STRING, pos, str);
    }
    return Token(TokenType::UNDEFINED, pos, std::string(1, c));
}

HtmlLexer::HtmlLexer(AbstractSource& source) : AbstractLexer(source, Token(TokenType::UNDEFINED)) {}

ConfLexer::ConfLexer(AbstractSource& source) : AbstractLexer(source, Token(TokenType::UNDEFINED)) {}

Token HtmlLexer::buildToken() {
    char c = source_.getChar();
    Position pos = source_.getPosition();

    if (c == '\0') {
        return Token(TokenType::END_OF_FILE, pos);
    }

    Token space = tryToBuildSpace();
    if (space.getType() == TokenType::SPACE) {
        return space;
    }

    Token string = tryToBuildString();
    if (string.getType() == TokenType::STRING) {
        return string;
    }

    if (c == '<') {
        source_.advance();
        if (source_.getChar() == '!') {
            source_.advance();
            if (source_.getChar() == '-') {
                if (source_.peek() == '-') {
                    source_.advance(2);
                    return Token(TokenType::COMMENT_START, pos, "<!--");
                }
            }
            return Token(TokenType::START_DOCTYPE, pos, "<!");
        }
        if (source_.getChar() == '/') {
            source_.advance();
            return Token(TokenType::START_END_TAG, pos, "</");
        }
        return Token(TokenType::START_START_TAG, pos, "<");
    }

    if (c == '-') {
        source_.advance();
        if (source_.getChar() == '-') {
            if (source_.peek() == '>') {
                source_.advance(2);
                return Token(TokenType::COMMENT_END, pos, "-->");
            }
        }
        return Token(TokenType::DASH, pos, "-");
    }

    if (c == '/') {
        source_.advance();
        if (source_.getChar() == '>') {
            source_.advance();
            return Token(TokenType::END_VOID_TAG, pos, "/>");
        }
        return Token(TokenType::SLASH, pos, "/");
    }

    if (c == '&') {
        source_.advance();
        if (source_.getChar() == '#') {
            source_.advance();
            char cx = source_.getChar();
            if (cx == 'x' || cx == 'X') {
                source_.advance();
                return Token(TokenType::START_HEX_CHAR_REF, pos, "&#x");
            }
            return Token(TokenType::START_DECIMAL_CHAR_REF, pos, "&#");
        }
        return Token(TokenType::START_NAMED_CHAR_REF, pos, "&");
    }

    switch (c) {
        case '>':
            source_.advance();
            return Token(TokenType::END_TAG, pos, ">");
        case '=':
            source_.advance();
            return Token(TokenType::EQUALS, pos, "=");
        case '"':
            source_.advance();
            return Token(TokenType::DOUBLE_QUOTE, pos, "\"");
        case '\'':
            source_.advance();
            return Token(TokenType::SINGLE_QUOTE, pos, "'");
        case ';':
            source_.advance();
            return Token(TokenType::END_CHAR_REF, pos, ";");
    }

    if (ispunct(c)) {
        source_.advance();
        return Token(TokenType::SYMBOL, pos, std::string(1, c));
    }

    return Token(TokenType::UNDEFINED, pos);
}

Token ConfLexer::buildToken() {
    char c = source_.getChar();
    Position pos = source_.getPosition();

    if (c == '\0') {
        return Token(TokenType::END_OF_FILE, pos);
    }

    Token space = tryToBuildSpace();
    if (space.getType() == TokenType::SPACE) {
        return space;
    }

    Token string = tryToBuildString();
    if (string.getType() == TokenType::STRING) {
        return string;
    }

    switch (c) {
        case '(':
            source_.advance();
            return Token(TokenType::START_RANGE, pos, "(");
        case ')':
            source_.advance();
            return Token(TokenType::END_RANGE, pos, ")");
        case '[':
            source_.advance();
            return Token(TokenType::START_ATTRIBUTE, pos, "[");
        case ']':
            source_.advance();
            return Token(TokenType::END_ATTRIBUTE, pos, "]");
        case '=':
            source_.advance();
            return Token(TokenType::EQUALS, pos, "=");
        case '"':
            source_.advance();
            return Token(TokenType::DOUBLE_QUOTE, pos, "\"");
        case '\'':
            source_.advance();
            return Token(TokenType::SINGLE_QUOTE, pos, "'");
        case ':':
            source_.advance();
            return Token(TokenType::RANGE_SEPARATOR, pos, ":");
        case '-':
            source_.advance();
            return Token(TokenType::DASH, pos, "-");
        case '_':
            source_.advance();
            return Token(TokenType::UNDERSCORE, pos, "_");
        case '.':
            source_.advance();
            return Token(TokenType::CLASS, pos, ".");
        case '#':
            source_.advance();
            return Token(TokenType::ID, pos, "#");
    }

    if (ispunct(c)) {
        source_.advance();
        return Token(TokenType::SYMBOL, pos, std::string(1, c));
    }

    return Token(TokenType::UNDEFINED, pos);
}
