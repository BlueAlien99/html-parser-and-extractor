#include <cassert>
#include <iostream>
#include <vector>

#include "sources.hpp"
#include "lexer.hpp"
#include "token.hpp"

int main(){
    using CT = ConfToken;

    std::vector<ConfToken> expected = { 
        CT::STRING, CT::ID, CT::STRING, CT::DASH, CT::STRING, CT::SPACE,
        CT::STRING, CT::CLASS, CT::STRING, CT::SPACE,
        CT::CLASS, CT::STRING, CT::CLASS, CT::STRING, CT::START_ATTRIBUTE, CT::STRING, CT::DASH, CT::STRING, CT::DASH, CT::STRING, CT::EQUALS, CT::DOUBLE_QUOTE, CT::STRING, CT::DOUBLE_QUOTE, CT::END_ATTRIBUTE, CT::SPACE,
        CT::STRING, CT::START_RANGE, CT::STRING, CT::END_RANGE, CT::START_RANGE, CT::STRING, CT::RANGE_SEPARATOR, CT::DASH, CT::STRING, CT::END_RANGE, CT::SPACE
     };

    std::vector<Token<ConfToken> > actual;

    SourceFromFile src("./data/random_conf.txt");
    Lexer<ConfToken> lexer(src);
    Token<ConfToken> token = lexer.getNextToken();
    while(token.getType() != ConfToken::END_OF_FILE){
        actual.push_back(token);
        std::cout<<token.getContent();
        token = lexer.getNextToken();
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
            std::cout<<"Mismatch at "<<i+1<<" token, actual is "<<(int)actual[i].getType()<<", expected "<<(int)expected[i]<<"!\n";
            return -1;
        }
    }

    std::cout<<"\nLexer works as expected!\n";

    return 0;
}