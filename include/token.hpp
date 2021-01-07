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

struct Position {
    Position() : src_pos(-1), src_line_pos(-1), line(-1), column(-1) {}
    Position(int src_pos, int src_line_pos, int line, int column)
        : src_pos(src_pos), src_line_pos(src_line_pos), line(line), column(column) {}

    int src_pos;
    int src_line_pos;
    int line;
    int column;
};

class Token {
public:
    Token(TokenType type) : type_(type), position_(Position()) {}
    Token(TokenType type, Position position) : type_(type), position_(position) {}
    Token(TokenType type, Position position, std::string content)
        : type_(type), position_(position), content_(content) {}

    TokenType getType() const { return type_; }
    Position getPosition() const { return position_; }
    std::string getContent() const { return content_; }

private:
    TokenType type_;
    Position position_;
    std::string content_;
};

#endif