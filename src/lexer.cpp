#include "lexer.hpp"

#include <string>
#include <variant>

#include "token.hpp"

AbstractLexer::AbstractLexer(AbstractSource& source, VariantToken token)
    : source_(source), token_(token) {}

VariantToken AbstractLexer::buildNextToken() {
    token_ = buildToken();
    return token_;
}

VariantToken AbstractLexer::getToken() const { return token_; }

HtmlLexer::HtmlLexer(AbstractSource& source)
    : AbstractLexer(source, Token(HtmlToken::UNDEFINED, 0)) {}

ConfLexer::ConfLexer(AbstractSource& source)
    : AbstractLexer(source, Token(ConfToken::UNDEFINED, 0)) {}

VariantToken HtmlLexer::buildToken() {
    char c = source_.getChar();
    unsigned int pos = source_.getPosition();

    if (c == '\0') {
        return Token(HtmlToken::END_OF_FILE, pos);
    }

    if (isspace(c)) {
        std::string str(1, c);
        source_.advance();
        while (isspace(source_.getChar())) {
            str += source_.getChar();
            source_.advance();
        }
        return Token(HtmlToken::SPACE, pos, str);
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
        return Token(HtmlToken::STRING, pos, str);
    }

    if (c == '<') {
        if (source_.peek(1) == '!') {
            if (source_.peek(2) == '-') {
                if (source_.peek(3) == '-') {
                    source_.advance(4);
                    return Token(HtmlToken::COMMENT_START, pos, "<!--");
                }
            }
            source_.advance(2);
            return Token(HtmlToken::START_DOCTYPE, pos, "<!");
        }
        if (source_.peek(1) == '/') {
            source_.advance(2);
            return Token(HtmlToken::START_END_TAG, pos, "</");
        }
        source_.advance();
        return Token(HtmlToken::START_START_TAG, pos, "<");
    }

    if (c == '-') {
        if (source_.peek(1) == '-') {
            if (source_.peek(2) == '>') {
                source_.advance(3);
                return Token(HtmlToken::COMMENT_END, pos, "-->");
            }
        }
        source_.advance();
        return Token(HtmlToken::DASH, pos, "-");
    }

    if (c == '/') {
        if (source_.peek() == '>') {
            source_.advance(2);
            return Token(HtmlToken::END_VOID_TAG, pos, "/>");
        }
        source_.advance();
        return Token(HtmlToken::SLASH, pos, "/");
    }

    if (c == '&') {
        if (source_.peek(1) == '#') {
            char cx = source_.peek(2);
            if (cx == 'x' || cx == 'X') {
                source_.advance(3);
                return Token(HtmlToken::START_HEX_CHAR_REF, pos, "&#x");
            }
            source_.advance(2);
            return Token(HtmlToken::START_DECIMAL_CHAR_REF, pos, "&#");
        }
        source_.advance();
        return Token(HtmlToken::START_NAMED_CHAR_REF, pos, "&");
    }

    if (c == '>') {
        source_.advance();
        return Token(HtmlToken::END_TAG, pos, ">");
    }

    if (c == '=') {
        source_.advance();
        return Token(HtmlToken::EQUALS, pos, "=");
    }

    if (c == '"') {
        source_.advance();
        return Token(HtmlToken::DOUBLE_QUOTE, pos, "\"");
    }

    if (c == '\'') {
        source_.advance();
        return Token(HtmlToken::SINGLE_QUOTE, pos, "'");
    }

    if (c == ';') {
        source_.advance();
        return Token(HtmlToken::END_CHAR_REF, pos, ";");
    }

    if (ispunct(c)) {
        source_.advance();
        return Token(HtmlToken::SYMBOL, pos, std::string(1, c));
    }

    return Token(HtmlToken::UNDEFINED, pos);
}

VariantToken ConfLexer::buildToken() {
    char c = source_.getChar();
    unsigned int pos = source_.getPosition();

    if (c == '\0') {
        return Token(ConfToken::END_OF_FILE, pos);
    }

    if (isspace(c)) {
        std::string str(1, c);
        source_.advance();
        while (isspace(source_.getChar())) {
            str += source_.getChar();
            source_.advance();
        }
        return Token(ConfToken::SPACE, pos, str);
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
        return Token(ConfToken::STRING, pos, str);
    }

    if (c == '(') {
        source_.advance();
        return Token(ConfToken::START_RANGE, pos, "(");
    }

    if (c == ')') {
        source_.advance();
        return Token(ConfToken::END_RANGE, pos, ")");
    }

    if (c == '[') {
        source_.advance();
        return Token(ConfToken::START_ATTRIBUTE, pos, "[");
    }

    if (c == ']') {
        source_.advance();
        return Token(ConfToken::END_ATTRIBUTE, pos, "]");
    }

    if (c == '=') {
        source_.advance();
        return Token(ConfToken::EQUALS, pos, "=");
    }

    if (c == '"') {
        source_.advance();
        return Token(ConfToken::DOUBLE_QUOTE, pos, "\"");
    }

    if (c == '\'') {
        source_.advance();
        return Token(ConfToken::SINGLE_QUOTE, pos, "'");
    }

    if (c == ':') {
        source_.advance();
        return Token(ConfToken::RANGE_SEPARATOR, pos, ":");
    }

    if (c == '-') {
        source_.advance();
        return Token(ConfToken::DASH, pos, "-");
    }

    if (c == '_') {
        source_.advance();
        return Token(ConfToken::UNDERSCORE, pos, "_");
    }

    if (c == '.') {
        source_.advance();
        return Token(ConfToken::CLASS, pos, ".");
    }

    if (c == '#') {
        source_.advance();
        return Token(ConfToken::ID, pos, "#");
    }

    if (ispunct(c)) {
        source_.advance();
        return Token(ConfToken::SYMBOL, pos, std::string(1, c));
    }

    return Token(ConfToken::UNDEFINED, pos);
}
