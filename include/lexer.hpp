#ifndef LEXER_HPP
#define LEXER_HPP

#include <variant>

#include "sources.hpp"
#include "token.hpp"

using VariantToken = std::variant<Token<HtmlToken>, Token<ConfToken> >;

class AbstractLexer {
public:
    VariantToken buildNextToken();
    VariantToken getToken() const;

protected:
    AbstractLexer(AbstractSource& source, VariantToken token);
    virtual VariantToken buildToken() = 0;

    AbstractSource& source_;
    VariantToken token_;
};

class HtmlLexer : public AbstractLexer {
public:
    HtmlLexer(AbstractSource& source);

private:
    VariantToken buildToken();
};

class ConfLexer : public AbstractLexer {
public:
    ConfLexer(AbstractSource& source);

private:
    VariantToken buildToken();
};

#endif