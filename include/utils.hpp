#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#include "token.hpp"

namespace utils {

bool isVoidElement(std::string name);
bool isReplaceableTagName(std::string name);
bool isNonReplaceableTagName(std::string name);
bool isCharacterReferenceStart(TokenType tokenType);
bool isValidAttrNameTT(TokenType tokenType);
bool isValidAttrValueUnquotedTT(TokenType tokenType);
bool isValidAttrValueQuotedTT(TokenType tokenType, TokenType quote);
bool isValidNormalCharacterDataTT(TokenType tokenType);

}  // namespace utils

#endif