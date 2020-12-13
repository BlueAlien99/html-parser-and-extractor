#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class TokenType {
    // Shared
    END_OF_FILE,
    UNDEFINED,
    SPACE,
    STRING,
    SYMBOL,
    EQUALS,
    DASH,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,

    // Html
    COMMENT_START,
    COMMENT_END,
    START_END_TAG,
    START_DOCTYPE,
    START_START_TAG,
    END_VOID_TAG,
    END_TAG,
    SLASH,
    START_HEX_CHAR_REF,
    START_DECIMAL_CHAR_REF,
    START_NAMED_CHAR_REF,
    END_CHAR_REF,

    // Config
    START_RANGE,
    END_RANGE,
    START_ATTRIBUTE,
    END_ATTRIBUTE,
    RANGE_SEPARATOR,
    UNDERSCORE,
    CLASS,
    ID
};

class Token {
public:
    Token(TokenType type, unsigned int position) : type_(type), position_(position) {}
    Token(TokenType type, unsigned int position, std::string content)
        : type_(type), position_(position), content_(content) {}
    TokenType getType() const { return type_; }
    unsigned int getPosition() const { return position_; }
    std::string getContent() const { return content_; }

private:
    TokenType type_;
    unsigned int position_;
    std::string content_;
};

#endif