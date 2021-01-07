#include <cassert>
#include <iostream>
#include <vector>

#include "lexer.hpp"
#include "sources.hpp"
#include "token.hpp"

int main() {
    using TT = TokenType;

    std::vector expected = {
        TT::START_DOCTYPE, TT::STRING, TT::SPACE, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::SPACE, TT::STRING, TT::EQUALS, TT::DOUBLE_QUOTE, TT::STRING, TT::DOUBLE_QUOTE, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::SPACE, TT::STRING, TT::EQUALS, TT::DOUBLE_QUOTE, TT::STRING, TT::DASH, TT::STRING, TT::DOUBLE_QUOTE, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::END_TAG, TT::STRING, TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::STRING, TT::SPACE, TT::STRING, TT::SPACE, TT::DASH, TT::END_TAG, TT::SPACE, TT::STRING, TT::SPACE, TT::SYMBOL, TT::SPACE, TT::STRING, TT::SPACE, TT::SYMBOL, TT::SPACE, TT::STRING, TT::SPACE, TT::STRING, TT::SPACE,
        TT::STRING, TT::SPACE,
        TT::COMMENT_START, TT::SPACE, TT::STRING, TT::SPACE, TT::STRING, TT::SPACE, TT::COMMENT_END, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::SPACE, TT::STRING, TT::EQUALS, TT::DOUBLE_QUOTE, TT::STRING, TT::SYMBOL, TT::SLASH, TT::SLASH, TT::STRING, TT::SYMBOL, TT::STRING, TT::DOUBLE_QUOTE, TT::END_TAG, TT::STRING, TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::SPACE, TT::STRING, TT::DASH, TT::STRING, TT::EQUALS, TT::STRING, TT::SPACE, TT::STRING, TT::DASH, TT::STRING, TT::EQUALS, TT::SINGLE_QUOTE, TT::STRING, TT::SINGLE_QUOTE, TT::END_VOID_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::END_TAG, TT::START_NAMED_CHAR_REF, TT::STRING, TT::END_CHAR_REF, TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::END_TAG, TT::START_HEX_CHAR_REF, TT::STRING, TT::END_CHAR_REF, TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::END_TAG, TT::START_DECIMAL_CHAR_REF, TT::STRING, TT::END_CHAR_REF, TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_START_TAG, TT::STRING, TT::SPACE, TT::STRING, TT::EQUALS, TT::DOUBLE_QUOTE, TT::STRING, TT::SYMBOL, TT::SPACE, TT::SYMBOL, TT::STRING, TT::END_CHAR_REF, TT::DOUBLE_QUOTE, TT::END_TAG, TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE,
        TT::START_END_TAG, TT::STRING, TT::END_TAG, TT::SPACE
    };

    std::vector<Token> actual;

    SourceFromFile src("./data/random_file.html");
    HtmlLexer lexer(src);
    Token token = lexer.buildNextToken();
    while (token.getType() != TokenType::END_OF_FILE) {
        actual.push_back(token);
        std::cout << token.getContent();
        lexer.buildNextToken();
        token = lexer.getToken();
    }

    if (expected.size() != actual.size()) {
        std::cout << "Expected has size of " << expected.size() << " but actual is "
                  << actual.size() << "!\n";
        return -1;
    }

    for (unsigned int i = 0; i < expected.size(); ++i) {
        if (expected[i] == actual[i].getType()) {
            // std::cout<<(int)expected[i]<<" ";
            continue;
        } else {
            Position pos = actual[i].getPosition();
            std::cout << "Error at position " << pos.line << ":" << pos.column << " -> " << src.getTextAtPosition(pos)
                      << "\n";
            std::cout << "Mismatch at " << i + 1 << " token, actual is " << (int)actual[i].getType()
                      << ", expected " << (int)expected[i] << "!\n";
            return -1;
        }
    }

    std::cout << "\nLexer works as expected!\n";

    return 0;
}