#include "lexer.hpp"

#include <string>

#include "token.hpp"
#include "utils.hpp"

template <>
Lexer<HtmlToken>::Lexer(AbstractSource& source)
    : source_(source), token_(Token(HtmlToken::UNDEFINED)) {}

template <>
Lexer<ConfToken>::Lexer(AbstractSource& source)
    : source_(source), token_(Token(ConfToken::UNDEFINED)) {}

template <typename Type>
Token<Type> Lexer<Type>::getNextToken() {
    token_ = makeToken();
    return token_;
}

template <typename Type>
Token<Type> Lexer<Type>::getLastToken() const {
    return token_;
}

template <>
Token<HtmlToken> Lexer<HtmlToken>::makeToken() {
    char c = source_.getChar();

    if (c == '\0') {
        return Token(HtmlToken::END_OF_FILE);
    }

    if (isSpace(c)) {
        std::string str(1, c);
        source_.advance();
        while(isSpace(source_.getChar())){
            str += source_.getChar();
            source_.advance();
        }
        return Token(HtmlToken::SPACE, str);
    }

    if (isAlphanum(c)) {
        std::string str(1, c);
        source_.advance();
        while(isAlphanum(source_.getChar())){
            str += source_.getChar();
            source_.advance();
        }
        return Token(HtmlToken::STRING, str);
    }

    if (c == '<') {
        if (source_.peek(1) == '!') {
            if (source_.peek(2) == '-') {
                if (source_.peek(3) == '-') {
                    source_.advance(4);
                    return Token(HtmlToken::COMMENT_START, "<!--");
                }
            }
            source_.advance(2);
            return Token(HtmlToken::START_DOCTYPE, "<!");
        }
        if (source_.peek(1) == '/') {
            source_.advance(2);
            return Token(HtmlToken::START_END_TAG, "</");
        }
        source_.advance();
        return Token(HtmlToken::START_START_TAG, "<");
    }

    if (c == '-') {
        if (source_.peek(1) == '-') {
            if (source_.peek(2) == '>') {
                source_.advance(3);
                return Token(HtmlToken::COMMENT_END, "-->");
            }
        }
        source_.advance();
        return Token(HtmlToken::DASH, "-");
    }

    if (c == '/') {
        if (source_.peek() == '>') {
            source_.advance(2);
            return Token(HtmlToken::END_VOID_TAG, "/>");
        }
        source_.advance();
        return Token(HtmlToken::SLASH, "/");
    }

    if (c == '&') {
        if (source_.peek(1) == '#') {
            char cx = source_.peek(2);
            if (cx == 'x' || cx == 'X') {
                source_.advance(3);
                return Token(HtmlToken::START_HEX_CHAR_REF, "&#x");
            }
            source_.advance(2);
            return Token(HtmlToken::START_DECIMAL_CHAR_REF, "&#");
        }
        source_.advance();
        return Token(HtmlToken::START_NAMED_CHAR_REF, "&");
    }

    if (c == '>') {source_.advance();return Token(HtmlToken::END_TAG, ">");}

    if (c == '=') {source_.advance();return Token(HtmlToken::EQUALS, "=");}

    if (c == '"') {source_.advance();return Token(HtmlToken::DOUBLE_QUOTE, "\"");}

    if (c == '\'') {source_.advance();return Token(HtmlToken::SINGLE_QUOTE, "'");}

    if (c == ';') {source_.advance();return Token(HtmlToken::END_CHAR_REF, ";");}

    if (isSymbol(c)) {source_.advance();return Token(HtmlToken::SYMBOL, std::string(1, c));}

    return Token(HtmlToken::UNDEFINED);
}

template <>
Token<ConfToken> Lexer<ConfToken>::makeToken() {
    char c = source_.getChar();

    if (c == '\0') {
        return Token(ConfToken::END_OF_FILE);
    }

    if (isSpace(c)) {
        std::string str(1, c);
        source_.advance();
        while(isSpace(source_.getChar())){
            str += source_.getChar();
            source_.advance();
        }
        return Token(ConfToken::SPACE, str);
    }

    if (isAlphanum(c)) {
        std::string str(1, c);
        source_.advance();
        while(isAlphanum(source_.getChar())){
            str += source_.getChar();
            source_.advance();
        }
        return Token(ConfToken::STRING, str);
    }

    if (c == '(') {
        source_.advance();
        return Token(ConfToken::START_RANGE, "(");
    }

    if (c == ')') {
        source_.advance();
        return Token(ConfToken::END_RANGE, ")");
    }

    if (c == '[') {
        source_.advance();
        return Token(ConfToken::START_ATTRIBUTE, "[");
    }

    if (c == ']') {
        source_.advance();
        return Token(ConfToken::END_ATTRIBUTE, "]");
    }

    if (c == '=') {
        source_.advance();
        return Token(ConfToken::EQUALS, "=");
    }

    if (c == '"') {
        source_.advance();
        return Token(ConfToken::DOUBLE_QUOTE, "\"");
    }

    if (c == '\'') {
        source_.advance();
        return Token(ConfToken::SINGLE_QUOTE, "'");
    }

    if (c == ':') {
        source_.advance();
        return Token(ConfToken::RANGE_SEPARATOR, ":");
    }

    if (c == '-') {
        source_.advance();
        return Token(ConfToken::DASH, "-");
    }

    if (c == '_') {
        source_.advance();
        return Token(ConfToken::UNDERSCORE, "_");
    }

    if (c == '.') {
        source_.advance();
        return Token(ConfToken::CLASS, ".");
    }

    if (c == '#') {
        source_.advance();
        return Token(ConfToken::ID, "#");
    }

    if (isSymbol(c)) {
        source_.advance();
        return Token(ConfToken::SYMBOL, std::string(1, c));
    }

    return Token(ConfToken::UNDEFINED);
}

template class Lexer<HtmlToken>;
template class Lexer<ConfToken>;
