#include <boost/test/unit_test.hpp>
#include <vector>

#include "lexer.hpp"
#include "sources.hpp"
#include "token.hpp"

BOOST_AUTO_TEST_SUITE(lexer_html)

BOOST_AUTO_TEST_CASE(lexer_html) {
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
