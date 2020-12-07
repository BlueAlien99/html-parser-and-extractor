#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class HtmlToken {
    END_OF_FILE,
    UNDEFINED,
    COMMENT_START,
    COMMENT_END,
    START_END_TAG,
    START_DOCTYPE,
    START_START_TAG,
    END_VOID_TAG,
    END_TAG,
    EQUALS,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    SLASH,
    DASH,
    START_HEX_CHAR_REF,
    START_DECIMAL_CHAR_REF,
    START_NAMED_CHAR_REF,
    END_CHAR_REF,
    SPECIAL_TAG_NAME,
    SPACE,
    STRING,
    SYMBOL
};

enum class ConfToken {
    END_OF_FILE,
    UNDEFINED,
    START_RANGE,
    END_RANGE,
    START_ATTRIBUTE,
    END_ATTRIBUTE,
    EQUALS,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    RANGE_SEPARATOR,
    DASH,
    UNDERSCORE,
    CLASS,
    ID,
    SPACE,
    STRING,
    SYMBOL
};

template <typename Enum>
class Token {
public:
    Token(Enum type, unsigned int position) : type_(type), position_(position) {}
    Token(Enum type, unsigned int position, std::string content)
        : type_(type), position_(position), content_(content) {}
    Enum getType() const { return type_; }
    unsigned int getPosition() const { return position_; }
    std::string getContent() const { return content_; }

private:
    Enum type_;
    unsigned int position_;
    std::string content_;
};

template class Token<HtmlToken>;
template class Token<ConfToken>;

#endif