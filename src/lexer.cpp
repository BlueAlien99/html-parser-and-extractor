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
    char c = source_.getNextChar();

    if (c == '\0') {
        return Token(HtmlToken::END_OF_FILE);
    }

    if (isSpace(c)) {
        std::string str = "";
        do {
            str += c;
            c = source_.getNextChar();
        } while (isSpace(c));
        source_.rewind();
        return Token(HtmlToken::SPACE, str);
    }

    if (isAlphanum(c)) {
        std::string str = "";
        do {
            str += c;
            c = source_.getNextChar();
        } while (isAlphanum(c));
        source_.rewind();
        return Token(HtmlToken::STRING, str);
    }

    if (c == '<') {
        if (source_.getNextChar() == '!') {
            if (source_.getNextChar() == '-') {
                if (source_.getNextChar() == '-') {
                    return Token(HtmlToken::COMMENT_START, "<!--");
                }
                source_.rewind();
            }
            source_.rewind();
            return Token(HtmlToken::START_DOCTYPE, "<!");
        }
        if (source_.getLastChar() == '/') {
            return Token(HtmlToken::START_END_TAG, "</");
        }
        source_.rewind();
        return Token(HtmlToken::START_START_TAG, "<");
    }

    if (c == '-') {
        if (source_.getNextChar() == '-') {
            if (source_.getNextChar() == '>') {
                return Token(HtmlToken::COMMENT_END, "-->");
            }
            source_.rewind();
        }
        source_.rewind();
        return Token(HtmlToken::DASH, "-");
    }

    if (c == '/') {
        if (source_.getNextChar() == '>') {
            return Token(HtmlToken::END_VOID_TAG, "/>");
        }
        source_.rewind();
        return Token(HtmlToken::SLASH, "/");
    }

    if (c == '&') {
        if (source_.getNextChar() == '#') {
            char cx = source_.getNextChar();
            if (cx == 'x' || cx == 'X') {
                return Token(HtmlToken::START_HEX_CHAR_REF, "&#x");
            }
            source_.rewind();
            return Token(HtmlToken::START_DECIMAL_CHAR_REF, "&#");
        }
        source_.rewind();
        return Token(HtmlToken::START_NAMED_CHAR_REF, "&");
    }

    if (c == '>') return Token(HtmlToken::END_TAG, ">");

    if (c == '=') return Token(HtmlToken::EQUALS, "=");

    if (c == '"') return Token(HtmlToken::DOUBLE_QUOTE, "\"");

    if (c == '\'') return Token(HtmlToken::SINGLE_QUOTE, "'");

    if (c == ';') return Token(HtmlToken::END_CHAR_REF, ";");

    if (isSymbol(c)) return Token(HtmlToken::SYMBOL, std::string(1, c));

    return Token(HtmlToken::UNDEFINED);
}

template <>
Token<ConfToken> Lexer<ConfToken>::makeToken() {
    return Token<ConfToken>(ConfToken::UNDEFINED);
}

template class Lexer<HtmlToken>;
template class Lexer<ConfToken>;
