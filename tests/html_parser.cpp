#include "html_parser.hpp"

#include <boost/test/unit_test.hpp>

#include "exceptions.hpp"
#include "lexer.hpp"
#include "node.hpp"
#include "sources.hpp"
#include "token.hpp"

BOOST_AUTO_TEST_SUITE(html_parser)

BOOST_AUTO_TEST_CASE(script) {
    SourceFromFile src("./data/parser_html/script");
    HtmlParser parser(src);
    std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
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
    std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
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
    std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 4);
    BOOST_CHECK(nodes[0]->getAllText() == " &  -  >");
    BOOST_CHECK(nodes[1]->getAllText() == "");
    BOOST_CHECK(nodes[2]->getAllText() == "> < Ą Ą");
    BOOST_CHECK(nodes[3]->getAllText() == "&xd; & @ &#123456789; &#abc; ? &#xZ; &#xZ & &; &xd xd ");
}

BOOST_AUTO_TEST_CASE(attributes) {
    SourceFromFile src("./data/parser_html/attributes");
    HtmlParser parser(src);
    std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
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
    std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 1);
    BOOST_CHECK(nodes[0]->getAllText() ==
                "somebodyonce told me\nthe world is gonna roll me \nI ain't the sharpest tool in "
                "the shed ");
}

BOOST_AUTO_TEST_CASE(misnested_tags) {
    SourceFromFile src("./data/parser_html/misnested_tags");
    HtmlParser parser(src);
    std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 1);
    auto p = nodes[0];
    BOOST_CHECK(p->getName() == "p");
    nodes = p->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 1);
    auto b = nodes[0];
    BOOST_CHECK(b->getName() == "b");
    nodes = b->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 1);
    auto i = nodes[0];
    BOOST_CHECK(i->getName() == "i");
    BOOST_CHECK(i->getHtmlNodes().size() == 0);
    BOOST_CHECK(i->getAllText() == "bar baz");
    BOOST_CHECK(i->getImmediateText() == "bar baz");
    BOOST_CHECK(b->getAllText() == "foo bar baz");
    BOOST_CHECK(b->getImmediateText() == "foo ");
    BOOST_CHECK(p->getAllText() == "foo bar baz ");
    BOOST_CHECK(p->getImmediateText() == "");
}

BOOST_AUTO_TEST_CASE(big_page) {
    SourceFromFile src("./data/parser_html/big_page");
    HtmlParser parser(src);
    std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 1);
    auto html = nodes[0];
    BOOST_CHECK(html->getName() == "html");
    nodes = html->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 2);
    auto head = nodes[0];
    auto body = nodes[1];
    BOOST_CHECK(head->getName() == "head");
    nodes = head->getHtmlNodes();
    BOOST_CHECK(nodes[3]->getName() == "style");
    BOOST_CHECK(nodes[3]->getNodes().empty());
    BOOST_CHECK(body->getName() == "body");
    nodes = body->getHtmlNodes();
    BOOST_CHECK(nodes[4]->getName() == "script");
    BOOST_CHECK(nodes[4]->getNodes().empty());
    BOOST_CHECK(nodes.size() == 5);
    BOOST_CHECK(nodes[0]->getAttributeValue("id") == "up");
    BOOST_CHECK(nodes[3]->getAttributeValue("id") == "right");
    BOOST_CHECK(html->getAllText() == "Webcam Strem Preview UP DOWN LEFT RIGHT ");
    BOOST_CHECK(html->getImmediateText() == "");
}

BOOST_AUTO_TEST_CASE(random_page) {
    SourceFromFile src("./data/random_file.html");
    HtmlParser parser(src);
    std::shared_ptr<HtmlElement> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    auto nodes = dom->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 1);
    nodes = nodes[0]->getHtmlNodes();
    auto body = nodes[1];
    nodes = body->getHtmlNodes();
    BOOST_CHECK(nodes.size() == 6);
    BOOST_CHECK(body->getImmediateText() == "some t3xt -> test . code @ 9th line żółć ");
    BOOST_CHECK(body->getAllText() == "some t3xt -> test . code @ 9th line żółć Google \n& Ą Ą ");
    BOOST_CHECK(nodes[0]->getAttributeValue("href") == "https://google.com");
    BOOST_CHECK(nodes[1]->getAttributeValue("data-attr") == "yes");
    BOOST_CHECK(nodes[1]->getAttributeValue("data-attr2") == "no");
    BOOST_CHECK(nodes[5]->getAttributeValue("style") == "color: #fff;");
}

BOOST_AUTO_TEST_SUITE_END()
