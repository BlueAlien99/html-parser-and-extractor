#include "lexer.hpp"

#include <string>

#include "token.hpp"

AbstractLexer::AbstractLexer(AbstractSource& source, Token token)
    : source_(source), token_(token) {}

Token AbstractLexer::buildNextToken() {
    token_ = buildToken();
    return token_;
}

Token AbstractLexer::getToken() const { return token_; }

Token AbstractLexer::tryToBuidSpace() {
    char c = source_.getChar();
    unsigned int pos = source_.getPosition();
    if (isspace(c)) {
        std::string str(1, c);
        while (isspace(source_.advance())) {
            str += source_.getChar();
        }
        return Token(TokenType::SPACE, pos, str);
    }
    return Token(TokenType::UNDEFINED, pos, std::string(1, c));
}

Token AbstractLexer::tryToBuidString() {
    char c = source_.getChar();
    unsigned int pos = source_.getPosition();
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

HtmlLexer::HtmlLexer(AbstractSource& source)
    : AbstractLexer(source, Token(TokenType::UNDEFINED, 0)) {}

ConfLexer::ConfLexer(AbstractSource& source)
    : AbstractLexer(source, Token(TokenType::UNDEFINED, 0)) {}

Token HtmlLexer::buildToken() {
    char c = source_.getChar();
    unsigned int pos = source_.getPosition();

    if (c == '\0') {
        return Token(TokenType::END_OF_FILE, pos);
    }

    Token space = tryToBuidSpace();
    if (space.getType() == TokenType::SPACE) {
        return space;
    }

    Token string = tryToBuidString();
    if (string.getType() == TokenType::STRING) {
        return string;
    }

    if (c == '<') {
        if (source_.peek(1) == '!') {
            if (source_.peek(2) == '-') {
                if (source_.peek(3) == '-') {
                    source_.advance(4);
                    return Token(TokenType::COMMENT_START, pos, "<!--");
                }
            }
            source_.advance(2);
            return Token(TokenType::START_DOCTYPE, pos, "<!");
        }
        if (source_.peek(1) == '/') {
            source_.advance(2);
            return Token(TokenType::START_END_TAG, pos, "</");
        }
        source_.advance();
        return Token(TokenType::START_START_TAG, pos, "<");
    }

    if (c == '-') {
        if (source_.peek(1) == '-') {
            if (source_.peek(2) == '>') {
                source_.advance(3);
                return Token(TokenType::COMMENT_END, pos, "-->");
            }
        }
        source_.advance();
        return Token(TokenType::DASH, pos, "-");
    }

    if (c == '/') {
        if (source_.peek() == '>') {
            source_.advance(2);
            return Token(TokenType::END_VOID_TAG, pos, "/>");
        }
        source_.advance();
        return Token(TokenType::SLASH, pos, "/");
    }

    if (c == '&') {
        if (source_.peek(1) == '#') {
            char cx = source_.peek(2);
            if (cx == 'x' || cx == 'X') {
                source_.advance(3);
                return Token(TokenType::START_HEX_CHAR_REF, pos, "&#x");
            }
            source_.advance(2);
            return Token(TokenType::START_DECIMAL_CHAR_REF, pos, "&#");
        }
        source_.advance();
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
    unsigned int pos = source_.getPosition();

    if (c == '\0') {
        return Token(TokenType::END_OF_FILE, pos);
    }

    Token space = tryToBuidSpace();
    if (space.getType() == TokenType::SPACE) {
        return space;
    }

    Token string = tryToBuidString();
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
