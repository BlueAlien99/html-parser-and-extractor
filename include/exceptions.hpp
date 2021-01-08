#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <exception>

#include "token.hpp"

class UnexpectedToken : public std::exception {
public:
    UnexpectedToken(Token token) : token_(token) {}
    Token getToken() { return token_; }

private:
    Token token_;
};

class MismatchedTags : public std::exception {
public:
    MismatchedTags(Token token, std::string expected) : token_(token), expected_(expected) {}
    Token getToken() { return token_; }
    std::string getExpected() { return expected_; }

private:
    Token token_;
    std::string expected_;
};

#endif