#include <boost/test/unit_test.hpp>
#include <vector>

#include "lexer.hpp"
#include "sources.hpp"
#include "token.hpp"

BOOST_AUTO_TEST_SUITE(lexer_conf)

BOOST_AUTO_TEST_CASE(lexer_conf) {
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
        // std::cout << token.getContent();
        lexer.buildNextToken();
        token = lexer.getToken();
    }

    if (expected.size() != actual.size()) {
        std::string msg = "Expected has size of " + std::to_string(expected.size()) +
                          " but actual is " + std::to_string(actual.size()) + "!\n";
        BOOST_FAIL(msg);
    }

    for (unsigned int i = 0; i < expected.size(); ++i) {
        if (expected[i] == actual[i].getType()) {
            continue;
        } else {
            Position pos = actual[i].getPosition();
            std::string msg = "Error at position " + std::to_string(pos.line) + ":" +
                              std::to_string(pos.column) + " -> " + src.getTextAtPosition(pos) +
                              "\n";
            msg += "Mismatch at " + std::to_string(i + 1) + " token, actual is " +
                   std::to_string((int)actual[i].getType()) + ", expected " +
                   std::to_string((int)expected[i]) + "!\n";
            BOOST_FAIL(msg);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
