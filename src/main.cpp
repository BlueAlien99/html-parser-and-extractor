#include <iostream>

#include "sources.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "node.hpp"
#include "exceptions.hpp"

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

    // std::string url = "https://www.onet.pl/";
    // SourceFromUrl src3(url);
    // while(char c = src3.advance()){
    //     std::cout<<c;
    // }
    // std::cout<<std::endl;

    SourceFromFile src("./data/random_file.html");
    HtmlParser parser(src);
    std::shared_ptr<Element> dom;
    try{
        dom = parser.parse();
        std::cout<<dom->getAllText();
    } catch(UnexpectedToken &err){
        Position pos = err.getToken().getPosition();
        std::cout<<"Error at "<<pos.line<<':'<<pos.column<<": "<<src.getTextAtPosition(pos)<<std::endl;
    } catch(MismatchedTags &err){
        Position pos = err.getToken().getPosition();
        std::cout<<"Error at "<<pos.line<<':'<<pos.column<<": "<<src.getTextAtPosition(pos)<<std::endl;
        std::cout<<"Expected "<<err.getExpected()<<std::endl;
    }

    // while(char c = src2.getNextChar()){
    //     std::cout<<c;
    // }
    // std::cout<<std::endl;

    return 0;
}