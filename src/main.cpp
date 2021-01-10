#include <iostream>

#include "conf_parser.hpp"
#include "exceptions.hpp"
#include "html_parser.hpp"
#include "lexer.hpp"
#include "node.hpp"
#include "sources.hpp"
#include "token.hpp"

int main() {
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

    // SourceFromFile src("./data/random_file.html");
    // SourceFromUrl src("https://www.onet.pl/");
    // HtmlParser parser(src);
    // std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
    // if (dom != nullptr) {
    //     std::cout << std::endl << dom->getAllText() << std::endl;
    // }

    // while(char c = src2.getNextChar()){
    //     std::cout<<c;
    // }
    // std::cout<<std::endl;

    SourceFromFile src("./data/random_conf.txt");
    ConfParser parser(src);
    std::shared_ptr<ConfObject> conf = parser.parseSafe(src);
    if (conf != nullptr) {
        std::cout << conf->getHumanReadableDescription() << std::endl << std::endl;
    }

    return 0;
}