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

BOOST_AUTO_TEST_SUITE_END()
