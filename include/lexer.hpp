#ifndef LEXER_HPP
#define LEXER_HPP

#include "sources.hpp"

class Lexer{
public:
    Lexer(AbstractSource& source);
    

private:
    AbstractSource &source_;
};

#endif