#ifndef LEXER_HPP
#define LEXER_HPP

#include "sources.hpp"
#include "token.hpp"

template <typename Type>
class Lexer {
public:
    Lexer(AbstractSource& source);
    Token<Type> getNextToken();
    Token<Type> getLastToken() const;

private:
    Token<Type> makeToken();

    AbstractSource& source_;
    Token<Type> token_;
};

#endif