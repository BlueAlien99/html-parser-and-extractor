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

    if (isspace(c)) {
        std::string str(1, c);
        source_.advance();
        while (isspace(source_.getChar())) {
            str += source_.getChar();
            source_.advance();
        }
        return Token(TokenType::SPACE, pos, str);
    }

    if (isalnum(c) || (signed char)c < 0) {
        std::string str(1, c);
        source_.advance();
        char nc = source_.getChar();
        while (isalnum(nc) || (signed char)nc < 0) {
            str += nc;
            source_.advance();
            nc = source_.getChar();
        }
        return Token(TokenType::STRING, pos, str);
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

    if (c == '>') {
        source_.advance();
        return Token(TokenType::END_TAG, pos, ">");
    }

    if (c == '=') {
        source_.advance();
        return Token(TokenType::EQUALS, pos, "=");
    }

    if (c == '"') {
        source_.advance();
        return Token(TokenType::DOUBLE_QUOTE, pos, "\"");
    }

    if (c == '\'') {
        source_.advance();
        return Token(TokenType::SINGLE_QUOTE, pos, "'");
    }

    if (c == ';') {
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

    if (isspace(c)) {
        std::string str(1, c);
        source_.advance();
        while (isspace(source_.getChar())) {
            str += source_.getChar();
            source_.advance();
        }
        return Token(TokenType::SPACE, pos, str);
    }

    if (isalnum(c) || (signed char)c < 0) {
        std::string str(1, c);
        source_.advance();
        char nc = source_.getChar();
        while (isalnum(nc) || (signed char)nc < 0) {
            str += nc;
            source_.advance();
            nc = source_.getChar();
        }
        return Token(TokenType::STRING, pos, str);
    }

    if (c == '(') {
        source_.advance();
        return Token(TokenType::START_RANGE, pos, "(");
    }

    if (c == ')') {
        source_.advance();
        return Token(TokenType::END_RANGE, pos, ")");
    }

    if (c == '[') {
        source_.advance();
        return Token(TokenType::START_ATTRIBUTE, pos, "[");
    }

    if (c == ']') {
        source_.advance();
        return Token(TokenType::END_ATTRIBUTE, pos, "]");
    }

    if (c == '=') {
        source_.advance();
        return Token(TokenType::EQUALS, pos, "=");
    }

    if (c == '"') {
        source_.advance();
        return Token(TokenType::DOUBLE_QUOTE, pos, "\"");
    }

    if (c == '\'') {
        source_.advance();
        return Token(TokenType::SINGLE_QUOTE, pos, "'");
    }

    if (c == ':') {
        source_.advance();
        return Token(TokenType::RANGE_SEPARATOR, pos, ":");
    }

    if (c == '-') {
        source_.advance();
        return Token(TokenType::DASH, pos, "-");
    }

    if (c == '_') {
        source_.advance();
        return Token(TokenType::UNDERSCORE, pos, "_");
    }

    if (c == '.') {
        source_.advance();
        return Token(TokenType::CLASS, pos, ".");
    }

    if (c == '#') {
        source_.advance();
        return Token(TokenType::ID, pos, "#");
    }

    if (ispunct(c)) {
        source_.advance();
        return Token(TokenType::SYMBOL, pos, std::string(1, c));
    }

    return Token(TokenType::UNDEFINED, pos);
}
