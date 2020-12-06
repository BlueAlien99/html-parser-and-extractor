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

    std::string url = "https://www.onet.pl/";
    SourceFromUrl src3(url);
    while(char c = src3.getNextChar()){
        std::cout<<c;
    }
    std::cout<<std::endl;

    return 0;
}