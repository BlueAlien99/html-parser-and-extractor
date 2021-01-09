#include "exceptions.hpp"
#include "lexer.hpp"
#include "node.hpp"
#include "parser.hpp"
#include "sources.hpp"
#include "token.hpp"

#define BOOST_TEST_MODULE parser_html
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(script) {
    SourceFromFile src("./data/parser_html/script");
    HtmlParser parser(src);
    std::shared_ptr<Element> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 2);
    BOOST_CHECK(nodes[0]->getName() == "script");
    BOOST_CHECK(nodes[0]->getHtmlNodes().empty());
    BOOST_CHECK(nodes[1]->getName() == "p");
    BOOST_CHECK(nodes[1]->getAllText() == "ok");
}

BOOST_AUTO_TEST_CASE(textarea) {
    SourceFromFile src("./data/parser_html/textarea");
    HtmlParser parser(src);
    std::shared_ptr<Element> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 2);
    BOOST_CHECK(nodes[0]->getNodes().size() == 1);
    BOOST_CHECK(nodes[0]->getAllText() == "\n    4 spaces\n    <p> e </p>\n    <!--\n");
    BOOST_CHECK(nodes[1]->getAllText() == "  2-on-  both  -sides  ");
}

BOOST_AUTO_TEST_CASE(character_reference) {
    SourceFromFile src("./data/parser_html/character_reference");
    HtmlParser parser(src);
    std::shared_ptr<Element> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 4);
    BOOST_CHECK(nodes[0]->getAllText() == " &  -  >");
    BOOST_CHECK(nodes[1]->getAllText() == "");
    BOOST_CHECK(nodes[2]->getAllText() == "> < Ą Ą");
    BOOST_CHECK(nodes[3]->getAllText() == "&xd; & @ &#123456789; &#abc; ? &#xZ; &#xZ ");
}

BOOST_AUTO_TEST_CASE(attributes) {
    SourceFromFile src("./data/parser_html/attributes");
    HtmlParser parser(src);
    std::shared_ptr<Element> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 1);
    BOOST_CHECK(nodes[0]->getName() == "div");
    BOOST_CHECK(nodes[0]->doesAttributeExist("id"));
    BOOST_CHECK(nodes[0]->getAttributeValue("id") == "div>'\n    '!--");
    BOOST_CHECK(nodes[0]->doesAttributeExist("class"));
    BOOST_CHECK(nodes[0]->getAttributeValue("class") == "--> /html>");
    BOOST_CHECK(nodes[0]->getHtmlNodes().size() == 1);
    auto middle = nodes[0]->getHtmlNodes()[0];
    BOOST_CHECK(middle->getName() == "div");
    BOOST_CHECK(middle->doesAttributeExist("test"));
    BOOST_CHECK(middle->getAttributeValue("test") == "");
    nodes = middle->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 3);
    BOOST_CHECK(nodes[0]->getAttributeValue("no_val-da<sh") == "");
    BOOST_CHECK(nodes[1]->getAttributeValue("unq") == "__--__");
    BOOST_CHECK(nodes[1]->getAttributeValue("__--__") == "");
    BOOST_CHECK(nodes[2]->getAttributeValue("tag") == "li/>");    
}

BOOST_AUTO_TEST_CASE(spaces) {
    SourceFromFile src("./data/parser_html/spaces");
    HtmlParser parser(src);
    std::shared_ptr<Element> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 1);
    BOOST_CHECK(nodes[0]->getAllText() == "somebodyonce told me\nthe world is gonna roll me \nI ain't the sharpest tool in the shed ");
}