#ifndef LEXER_HPP
#define LEXER_HPP

#include "sources.hpp"
#include "token.hpp"

class AbstractLexer {
public:
    Token buildNextToken();
    Token getToken() const;

protected:
    AbstractLexer(AbstractSource& source, Token token);
    virtual Token buildToken() = 0;

    Token tryToBuidSpace();
    Token tryToBuidString();

    AbstractSource& source_;
    Token token_;
};

class HtmlLexer : public AbstractLexer {
public:
    HtmlLexer(AbstractSource& source);

private:
    Token buildToken();
};

class ConfLexer : public AbstractLexer {
public:
    ConfLexer(AbstractSource& source);

private:
    Token buildToken();
};

#endif