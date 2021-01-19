#include "extractor.hpp"

#include <boost/test/unit_test.hpp>
#include <set>
#include <vector>

#include "conf_object.hpp"
#include "conf_parser.hpp"
#include "html_parser.hpp"
#include "node.hpp"
#include "node_iterator.hpp"
#include "sources.hpp"

BOOST_AUTO_TEST_SUITE(extractor)

BOOST_AUTO_TEST_CASE(tags_and_nesting) {
    SourceFromFile src("./data/extractor/basic");
    HtmlParser parser(src);
    std::unique_ptr<Node> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(dom->getId() == 0);

    SourceFromString c_src("div p");
    SourceFromString c2_src("div div p");

    auto f1 = Extractor::extract(dom->clone(), c_src, true);
    auto f2 = Extractor::extract(dom->clone(), c2_src, true);

    BOOST_REQUIRE(f1 != nullptr);
    auto f1_nodes = f1->getHtmlNodes();
    BOOST_CHECK(f1_nodes.size() == 7);
    for (unsigned int i = 0; i < f1_nodes.size(); ++i) {
        BOOST_CHECK(f1_nodes[i]->getName() == "p");
    }

    BOOST_REQUIRE(f2 != nullptr);
    auto f2_nodes = f2->getHtmlNodes();
    BOOST_CHECK(f2_nodes.size() == 6);
    for (unsigned int i = 0; i < f2_nodes.size(); ++i) {
        BOOST_CHECK(f2_nodes[i]->getName() == "p");
    }
}

BOOST_AUTO_TEST_CASE(classes) {
    SourceFromFile src("./data/extractor/basic");
    HtmlParser parser(src);
    std::unique_ptr<Node> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(dom->getId() == 0);

    SourceFromString c_src(".ipsum");
    SourceFromString c2_src(".ipsum.lorem");

    auto f1 = Extractor::extract(dom->clone(), c_src, true);
    auto f2 = Extractor::extract(dom->clone(), c2_src, true);

    std::vector<std::string> texts = {"one", "two", "four"};
    std::vector<std::string> tags = {"li", "li", "p"};

    BOOST_REQUIRE(f1 != nullptr);
    auto f1_nodes = f1->getHtmlNodes();
    BOOST_CHECK(f1_nodes.size() == 3);
    for (unsigned int i = 0; i < f1_nodes.size(); ++i) {
        BOOST_CHECK(f1_nodes[i]->getName() == tags[i]);
        BOOST_CHECK(f1_nodes[i]->getAllText() == texts[i]);
    }

    BOOST_REQUIRE(f2 != nullptr);
    auto f2_nodes = f2->getHtmlNodes();
    BOOST_CHECK(f2_nodes.size() == 2);
    for (unsigned int i = 0; i < f2_nodes.size(); ++i) {
        BOOST_CHECK(f2_nodes[i]->getName() == tags[i]);
        BOOST_CHECK(f2_nodes[i]->getAllText() == texts[i]);
    }
}

BOOST_AUTO_TEST_CASE(attributes) {
    SourceFromFile src("./data/extractor/basic");
    HtmlParser parser(src);
    std::unique_ptr<Node> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(dom->getId() == 0);

    SourceFromString c_src("[ok] [no='2']");
    SourceFromString c2_src("[ok] [no='2'] [class='']");

    auto f1 = Extractor::extract(dom->clone(), c_src, true);
    auto f2 = Extractor::extract(dom->clone(), c2_src, true);

    std::vector<std::string> texts = {"five", "six"};

    BOOST_REQUIRE(f1 != nullptr);
    auto f1_nodes = f1->getHtmlNodes();
    BOOST_CHECK(f1_nodes.size() == 1);
    BOOST_CHECK(f1_nodes[0]->getName() == "ul");
    BOOST_CHECK(f1_nodes[0]->getAllText() == "four five six ");

    BOOST_REQUIRE(f2 != nullptr);
    auto f2_nodes = f2->getHtmlNodes();
    BOOST_CHECK(f2_nodes.size() == 2);
    for (unsigned int i = 0; i < f2_nodes.size(); ++i) {
        BOOST_CHECK(f2_nodes[i]->getName() == "p");
        BOOST_CHECK(f2_nodes[i]->getAllText() == texts[i]);
    }
}

BOOST_AUTO_TEST_CASE(non_existent_class_or_attribute) {
    SourceFromFile src("./data/extractor/basic");
    HtmlParser parser(src);
    std::unique_ptr<Node> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(dom->getId() == 0);

    SourceFromString c_src(".non-existent");
    SourceFromString c2_src("[non-existent]");

    auto f1 = Extractor::extract(dom->clone(), c_src, true);
    auto f2 = Extractor::extract(dom->clone(), c2_src, true);

    BOOST_REQUIRE(f1 != nullptr);
    auto f1_nodes = f1->getHtmlNodes();
    BOOST_CHECK(f1_nodes.size() == 0);

    BOOST_REQUIRE(f2 != nullptr);
    auto f2_nodes = f2->getHtmlNodes();
    BOOST_CHECK(f2_nodes.size() == 0);
}

BOOST_AUTO_TEST_CASE(ranges) {
    SourceFromFile src("./data/extractor/basic");
    HtmlParser parser(src);
    std::unique_ptr<Node> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(dom->getId() == 0);

    SourceFromString c_src("li(3:4)");
    SourceFromString c2_src("li(3:10)");
    SourceFromString c3_src("li(:-10)");
    SourceFromString c4_src("li(10:)");
    SourceFromString c5_src("li(1)(4:5)");
    SourceFromString c6_src("li(1:)(4:5)");
    SourceFromString c7_src("li(3:) p(1)");
    SourceFromString c8_src("li(3:) (1)");

    auto f1 = Extractor::extract(dom->clone(), c_src, true);
    auto f2 = Extractor::extract(dom->clone(), c2_src, true);
    auto f3 = Extractor::extract(dom->clone(), c3_src, true);
    auto f4 = Extractor::extract(dom->clone(), c4_src, true);
    auto f5 = Extractor::extract(dom->clone(), c5_src, true);
    auto f6 = Extractor::extract(dom->clone(), c6_src, true);
    auto f7 = Extractor::extract(dom->clone(), c7_src, true);
    auto f8 = Extractor::extract(dom->clone(), c8_src, true);

    BOOST_REQUIRE(f1 != nullptr);
    BOOST_CHECK(f1->getHtmlNodes().size() == 2);
    BOOST_CHECK(f1->getAllText() == "fourfive");

    BOOST_REQUIRE(f2 != nullptr);
    BOOST_CHECK(f2->getHtmlNodes().size() == 3);
    BOOST_CHECK(f2->getAllText() == "fourfivesix");

    BOOST_REQUIRE(f3 != nullptr);
    BOOST_CHECK(f3->getHtmlNodes().size() == 0);
    BOOST_CHECK(f3->getAllText() == "");

    BOOST_REQUIRE(f4 != nullptr);
    BOOST_CHECK(f4->getHtmlNodes().size() == 0);
    BOOST_CHECK(f4->getAllText() == "");

    BOOST_REQUIRE(f5 != nullptr);
    BOOST_CHECK(f5->getHtmlNodes().size() == 3);
    BOOST_CHECK(f5->getAllText() == "twofivesix");

    BOOST_REQUIRE(f6 != nullptr);
    BOOST_CHECK(f6->getHtmlNodes().size() == 5);
    BOOST_CHECK(f6->getAllText() == "twothreefourfivesix");

    BOOST_REQUIRE(f7 != nullptr);
    BOOST_CHECK(f7->getHtmlNodes().size() == 1);
    BOOST_CHECK(f7->getHtmlNodes()[0]->getName() == "p");
    BOOST_CHECK(f7->getAllText() == "five");

    BOOST_REQUIRE(f8 != nullptr);
    BOOST_CHECK(f8->getHtmlNodes().size() == 1);
    BOOST_CHECK(f8->getHtmlNodes()[0]->getName() == "p");
    BOOST_CHECK(f8->getAllText() == "five");
}

BOOST_AUTO_TEST_SUITE_END()
