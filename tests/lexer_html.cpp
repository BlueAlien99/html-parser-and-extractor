#include <cassert>
#include <iostream>
#include <variant>
#include <vector>

#include "sources.hpp"
#include "lexer.hpp"
#include "token.hpp"

int main(){
    using HT = HtmlToken;

    std::vector<HtmlToken> expected = {
        HT::START_DOCTYPE, HT::STRING, HT::SPACE, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::SPACE, HT::STRING, HT::EQUALS, HT::DOUBLE_QUOTE, HT::STRING, HT::DOUBLE_QUOTE, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::SPACE, HT::STRING, HT::EQUALS, HT::DOUBLE_QUOTE, HT::STRING, HT::DASH, HT::STRING, HT::DOUBLE_QUOTE, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::END_TAG, HT::STRING, HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::STRING, HT::SPACE, HT::STRING, HT::SPACE, HT::DASH, HT::END_TAG, HT::SPACE, HT::STRING, HT::SPACE, HT::SYMBOL, HT::SPACE, HT::STRING, HT::SPACE, HT::SYMBOL, HT::SPACE, HT::STRING, HT::SPACE, HT::STRING, HT::SPACE,
        HT::STRING, HT::SPACE,
        HT::COMMENT_START, HT::SPACE, HT::STRING, HT::SPACE, HT::STRING, HT::SPACE, HT::COMMENT_END, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::SPACE, HT::STRING, HT::EQUALS, HT::DOUBLE_QUOTE, HT::STRING, HT::SYMBOL, HT::SLASH, HT::SLASH, HT::STRING, HT::SYMBOL, HT::STRING, HT::DOUBLE_QUOTE, HT::END_TAG, HT::STRING, HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::SPACE, HT::STRING, HT::DASH, HT::STRING, HT::EQUALS, HT::STRING, HT::SPACE, HT::STRING, HT::DASH, HT::STRING, HT::EQUALS, HT::SINGLE_QUOTE, HT::STRING, HT::SINGLE_QUOTE, HT::END_VOID_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::END_TAG, HT::START_NAMED_CHAR_REF, HT::STRING, HT::END_CHAR_REF, HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::END_TAG, HT::START_HEX_CHAR_REF, HT::STRING, HT::END_CHAR_REF, HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::END_TAG, HT::START_DECIMAL_CHAR_REF, HT::STRING, HT::END_CHAR_REF, HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_START_TAG, HT::STRING, HT::SPACE, HT::STRING, HT::EQUALS, HT::DOUBLE_QUOTE, HT::STRING, HT::SYMBOL, HT::SPACE, HT::SYMBOL, HT::STRING, HT::END_CHAR_REF, HT::DOUBLE_QUOTE, HT::END_TAG, HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE,
        HT::START_END_TAG, HT::STRING, HT::END_TAG, HT::SPACE
    };

    std::vector<Token<HtmlToken> > actual;

    SourceFromFile src("./data/random_file.html");
    HtmlLexer lexer(src);
    Token<HtmlToken> token = std::get<Token<HtmlToken> >(lexer.buildNextToken());
    while(token.getType() != HtmlToken::END_OF_FILE){
        actual.push_back(token);
        std::cout<<token.getContent();
        lexer.buildNextToken();
        token = std::get<Token<HtmlToken> >(lexer.getToken());
    }

    if(expected.size() != actual.size()){
        std::cout<<"Expected has size of "<<expected.size()<<" but actual is "<<actual.size()<<"!\n";
        return -1;
    }

    for(unsigned int i = 0; i < expected.size(); ++i){
        if(expected[i] == actual[i].getType()){
            // std::cout<<(int)expected[i]<<" ";
            continue;
        } else{
            std::cout<<"Error at position "<<actual[i].getPosition()<<"\n";
            std::cout<<"Mismatch at "<<i+1<<" token, actual is "<<(int)actual[i].getType()<<", expected "<<(int)expected[i]<<"!\n";
            return -1;
        }
    }

    std::cout<<"\nLexer works as expected!\n";

    return 0;
}