#ifndef LEXER_HPP
#define LEXER_HPP

#include "sources.hpp"
#include "token.hpp"

template <typename Type>
class Lexer {
public:
    Lexer(AbstractSource& source);
    Token<Type> buildNextToken();
    Token<Type> getToken() const;

private:
    Token<Type> buildToken();

    AbstractSource& source_;
    Token<Type> token_;
};

#endif