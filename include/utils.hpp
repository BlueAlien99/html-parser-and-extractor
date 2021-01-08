#ifndef UTILS_HPP
#define UTILS_HPP

#include <set>
#include <string>

#include "token.hpp"

namespace utils {

bool isVoidElement(std::string name) {
    static const std::set<std::string> void_elems_ = {
        "area",  "base",   "br",   "col",  "command", "embed",  "hr",    "img",
        "input", "keygen", "link", "meta", "param",   "source", "track", "wbr"};
    if (void_elems_.find(name) != void_elems_.end()) {
        return true;
    }
    return false;
}

bool isValidAttrNameTT(TokenType tokenType) {
    static const std::set<TokenType> invalid_token_types_ = {
        TokenType::SINGLE_QUOTE, TokenType::DOUBLE_QUOTE,  TokenType::END_TAG,
        TokenType::END_VOID_TAG, TokenType::SLASH,         TokenType::EQUALS,
        TokenType::COMMENT_END,  TokenType::START_END_TAG, TokenType::SPACE};
    if (invalid_token_types_.find(tokenType) != invalid_token_types_.end()) {
        return false;
    }
    return true;
}

bool isValidAttrValueUnquotedTT(TokenType tokenType) {
    static const std::set<TokenType> invalid_token_types_ = {
        TokenType::SINGLE_QUOTE, TokenType::DOUBLE_QUOTE, TokenType::END_TAG,
        TokenType::END_VOID_TAG, TokenType::EQUALS,       TokenType::COMMENT_END,
        TokenType::SPACE};
    if (invalid_token_types_.find(tokenType) != invalid_token_types_.end()) {
        return false;
    }
    return true;
}

bool isValidAttrValueQuotedTT(TokenType tokenType, TokenType quote) {
    static const std::set<TokenType> invalid_token_types_ = {
        TokenType::COMMENT_START, TokenType::START_END_TAG, TokenType::START_DOCTYPE,
        TokenType::START_START_TAG};
    if (invalid_token_types_.find(tokenType) != invalid_token_types_.end() || tokenType == quote) {
        return false;
    }
    return true;
}

}  // namespace utils

#endif