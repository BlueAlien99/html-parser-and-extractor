#include "sources.hpp"

#include <boost/test/unit_test.hpp>
#include <fstream>

#include "token.hpp"

BOOST_AUTO_TEST_SUITE(sources)

BOOST_AUTO_TEST_CASE(source_from_string) {
    std::string str = "random string\nxdxd12345678901234567890some";
    SourceFromString sfs(str);
    BOOST_CHECK(sfs.getPosition().line == 1 && sfs.getPosition().column == 1);
    BOOST_CHECK(sfs.peek() == 'a');
    for (unsigned int i = 0; i < str.size(); ++i) {
        if (i == 12) {
            BOOST_CHECK(sfs.getPosition().src_pos == 13);
            BOOST_CHECK(sfs.getPosition().src_line_pos == 0);
            BOOST_CHECK(sfs.getPosition().line == 1);
            BOOST_CHECK(sfs.getPosition().column == 13);
        }
        if (i == 13) {
            BOOST_CHECK(sfs.getPosition().src_pos == 14);
            BOOST_CHECK(sfs.getPosition().src_line_pos == 14);
            BOOST_CHECK(sfs.getPosition().line == 2);
            BOOST_CHECK(sfs.getPosition().column == 0);
        }
        if (i == 15) {
            BOOST_CHECK(sfs.getPosition().src_pos == 16);
            BOOST_CHECK(sfs.getPosition().src_line_pos == 14);
            BOOST_CHECK(sfs.getPosition().line == 2);
            BOOST_CHECK(sfs.getPosition().column == 2);
        }
        if (i == 41) {
            BOOST_CHECK(sfs.getPosition().src_pos == 42);
            BOOST_CHECK(sfs.getPosition().src_line_pos == 14);
            BOOST_CHECK(sfs.getPosition().line == 2);
            BOOST_CHECK(sfs.getPosition().column == 28);
        }
        BOOST_CHECK(sfs.getChar() == str[i]);
        sfs.advance();
    }
    BOOST_CHECK(sfs.getPosition().src_pos == 42);
    BOOST_CHECK(sfs.getPosition().src_line_pos == 14);
    BOOST_CHECK(sfs.getPosition().line == 2);
    BOOST_CHECK(sfs.getPosition().column == 28);
    BOOST_CHECK(sfs.getChar() == '\0');
}

BOOST_AUTO_TEST_CASE(source_from_file) {
    SourceFromFile sff("./data/random_conf.txt");
    std::streampos file_beg, file_end;
    std::ifstream file;
    file.open("./data/random_conf.txt", std::ios::in);
    BOOST_CHECK(file.is_open());
    file_beg = file.tellg();
    BOOST_CHECK(file.get() == sff.getChar());
    file.seekg(0, std::ios::end);
    file_end = file.tellg();
    while (sff.getChar() != '\0') {
        sff.advance();
    }
    BOOST_CHECK(sff.getPosition().line == 7);
    BOOST_CHECK(sff.getPosition().column == 0);
    sff.advance(2);
    BOOST_CHECK(sff.getChar() == '\0');
    file.close();
}

BOOST_AUTO_TEST_CASE(source_from_file_non_existent) {
    SourceFromFile sff2("./data/doesnt_exist.txt");
    BOOST_CHECK('\0' == sff2.getChar());
    BOOST_CHECK('\0' == sff2.advance());
}

BOOST_AUTO_TEST_CASE(source_from_url) {
    SourceFromUrl sfu("https://www.onet.pl/");
    std::string html_beg, html_end;
    int i = 0;
    while (sfu.getChar() != '\0') {
        if (i < 15) {
            html_beg += sfu.getChar();
        }
        html_end += sfu.getChar();
        if (html_end.size() > 7) {
            html_end = html_end.substr(1);
        }
        ++i;
        sfu.advance();
    }
    BOOST_CHECK(html_beg == "<!DOCTYPE html>");
    BOOST_CHECK(html_end == "</html>");
}

BOOST_AUTO_TEST_CASE(source_from_url_non_existent) {
    SourceFromUrl sfu2("https://this-site-doesnt-exist/");
    BOOST_CHECK('\0' == sfu2.getChar());
    BOOST_CHECK('\0' == sfu2.advance());
}

BOOST_AUTO_TEST_SUITE_END()
