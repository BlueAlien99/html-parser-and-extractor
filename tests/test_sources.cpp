#include <cassert>
#include <fstream>
#include <iostream>

#include "sources.hpp"

int main() {

    std::cout << "Testing SourceFromString... ";
    std::string str = "random string";
    SourceFromString sfs(str);
    assert(sfs.peek() == 'a');
    for (unsigned int i = 0; i < str.size(); ++i) {
        // assert(sfs.getPosition() == i);
        assert(sfs.getChar() == str[i]);
        sfs.advance();
    }
    assert(sfs.getChar() == '\0');
    std::cout << "Done!\n";



    std::cout << "Testing SourceFromFile... ";
    SourceFromFile sff("./data/random_conf.txt");
    std::streampos file_beg, file_end;
    std::ifstream file;
    file.open("./data/random_conf.txt", std::ios::in);
    assert(file.is_open());
    file_beg = file.tellg();
    assert(file.get() == sff.getChar());
    file.seekg(0, std::ios::end);
    file_end = file.tellg();
    while (sff.getChar() != '\0') {
        sff.advance();
    }
    // assert((file_end - file_beg) == sff.getPosition());
    sff.advance(2);
    assert(sff.getChar() == '\0');
    file.close();
    std::cout << "Done!\n";



    std::cout << "Testing SourceFromFile (non-existent)... ";
    SourceFromFile sff2("./data/doesnt_exist.txt");
    assert('\0' == sff2.getChar());
    assert('\0' == sff2.advance());
    std::cout << "Done!\n";



    std::cout << "Testing SourceFromUrl... ";
    SourceFromUrl sfu("https://www.onet.pl/");
    std::string html_beg, html_end;
    int i = 0;
    while (sfu.getChar() != '\0') {
        if (i < 15) {
            html_beg += sfu.getChar();
        }
        ++i;
        sfu.advance();
    }
    sfu.advance(-7);
    while (sfu.getChar() != '\0') {
        html_end += sfu.getChar();
        sfu.advance();
    }
    assert(html_beg == "<!DOCTYPE html>");
    assert(html_end == "</html>");
    std::cout << "Done!\n";



    std::cout << "Testing SourceFromUrl (non-existent)... ";
    SourceFromUrl sfu2("https://this-site-doesnt-exist/");
    assert('\0' == sfu2.getChar());
    assert('\0' == sfu2.advance());
    std::cout << "Done!\n";



    return 0;
}