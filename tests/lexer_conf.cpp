#include <cassert>
#include <iostream>
#include <vector>

#include "lexer.hpp"
#include "sources.hpp"
#include "token.hpp"

int main() {
    using TT = TokenType;

    std::vector expected = { 
        TT::STRING, TT::ID, TT::STRING, TT::UNDERSCORE, TT::STRING, TT::SPACE,
        TT::STRING, TT::CLASS, TT::STRING, TT::SPACE,
        TT::CLASS, TT::STRING, TT::CLASS, TT::STRING, TT::START_ATTRIBUTE, TT::STRING, TT::DASH, TT::STRING, TT::DASH, TT::STRING, TT::EQUALS, TT::DOUBLE_QUOTE, TT::STRING, TT::DOUBLE_QUOTE, TT::END_ATTRIBUTE, TT::SPACE,
        TT::STRING, TT::START_RANGE, TT::STRING, TT::END_RANGE, TT::START_RANGE, TT::STRING, TT::RANGE_SEPARATOR, TT::DASH, TT::STRING, TT::END_RANGE, TT::SPACE,
        TT::STRING, TT::START_ATTRIBUTE, TT::STRING, TT::EQUALS, TT::SINGLE_QUOTE, TT::SYMBOL, TT::SINGLE_QUOTE, TT::END_ATTRIBUTE, TT::SPACE
    };

    std::vector<Token> actual;

    SourceFromFile src("./data/random_conf.txt");
    ConfLexer lexer(src);
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