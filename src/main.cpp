#include <iostream>

#include "sources.hpp"
#include "token.hpp"
#include "lexer.hpp"

int main(){
    // SourceFromString src("123456789 xd");
    // while(char c = src.getNextChar()){
    //     std::cout<<c;
    // }
    // std::cout<<std::endl;

    // SourceFromFile src2("./data/random_file.html");
    // while(char c = src2.getNextChar()){
    //     std::cout<<c;
    // }
    // std::cout<<std::endl;

    // std::string url = "https://wolnelektury.pl/media/book/txt/lalka-tom-pierwszy.txt";
    // std::string url = "https://www.onet.pl/";
    // std::string url = "https://dfsadsdgd.pl";
    // SourceFromUrl src3(url);
    // while(char c = src3.getNextChar()){
    //     std::cout<<c;
    // }
    // std::cout<<std::endl;
    // Token<HtmlToken> token(HtmlToken::SPACE);
    // Token<ConfToken> token2(ConfToken::SPACE, "something");

    // std::cout<<(int)token.getType()<<"\n";
    // std::cout<<(int)token2.getType()<<token2.getContent()<<"\n";

    // std::cout<<Html::xd;
    // SourceFromString src("123456789 xd");
    // SourceFrom src("123456789 xd");
    SourceFromFile src("./data/random_file.html");
    Lexer<HtmlToken> lexer(src);
    Token<HtmlToken> token = lexer.getNextToken();
    while(token.getType() != HtmlToken::END_OF_FILE){
        // std::cout<<(int)token.getType()<<' ';
        std::cout<<token.getContent();
        token = lexer.getNextToken();
    }

    return 0;
}