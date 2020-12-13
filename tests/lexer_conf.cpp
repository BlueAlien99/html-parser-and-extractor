#include <cassert>
#include <iostream>
#include <vector>

#include "lexer.hpp"
#include "sources.hpp"
#include "token.hpp"

int main() {
    using CT = TokenType;

    std::vector expected = { 
        CT::STRING, CT::ID, CT::STRING, CT::UNDERSCORE, CT::STRING, CT::SPACE,
        CT::STRING, CT::CLASS, CT::STRING, CT::SPACE,
        CT::CLASS, CT::STRING, CT::CLASS, CT::STRING, CT::START_ATTRIBUTE, CT::STRING, CT::DASH, CT::STRING, CT::DASH, CT::STRING, CT::EQUALS, CT::DOUBLE_QUOTE, CT::STRING, CT::DOUBLE_QUOTE, CT::END_ATTRIBUTE, CT::SPACE,
        CT::STRING, CT::START_RANGE, CT::STRING, CT::END_RANGE, CT::START_RANGE, CT::STRING, CT::RANGE_SEPARATOR, CT::DASH, CT::STRING, CT::END_RANGE, CT::SPACE,
        CT::STRING, CT::START_ATTRIBUTE, CT::STRING, CT::EQUALS, CT::SINGLE_QUOTE, CT::SYMBOL, CT::SINGLE_QUOTE, CT::END_ATTRIBUTE, CT::SPACE
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
            std::cout << "Error at position " << actual[i].getPosition() << "\n";
            std::cout << "Mismatch at " << i + 1 << " token, actual is " << (int)actual[i].getType()
                      << ", expected " << (int)expected[i] << "!\n";
            return -1;
        }
    }

    std::cout << "\nLexer works as expected!\n";

    return 0;
}