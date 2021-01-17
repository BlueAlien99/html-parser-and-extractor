#include "node_iterator.hpp"

#include <boost/test/unit_test.hpp>
#include <set>

#include "html_parser.hpp"
#include "node.hpp"
#include "sources.hpp"

BOOST_AUTO_TEST_SUITE(node_iterator)

BOOST_AUTO_TEST_CASE(basic) {
    SourceFromFile src("./data/node_iterator/basic");
    HtmlParser parser(src);
    std::unique_ptr<Node> dom = parser.parseSafe(src);
    BOOST_REQUIRE(dom != nullptr);
    BOOST_CHECK(dom->getId() == 0);
    std::set<unsigned int> ids;
    std::vector<std::string> tags = {"div", "p", "ul", "span", "li", "li", "li", "b"};
    unsigned int c = 0;
    for (NodeIterator i = dom->begin(); i != dom->end(); ++i) {
        BOOST_CHECK(c < tags.size());
        std::unique_ptr<Node> node = std::move(*i);
        BOOST_CHECK(node != nullptr);
        BOOST_CHECK(ids.count(node->getId()) == 0);
        ids.insert(node->getId());
        BOOST_CHECK(ids.count(node->getId()) == 1);
        BOOST_CHECK(node->getName() == tags[c]);
        ++c;
    }
    BOOST_CHECK(c == tags.size());
    BOOST_CHECK(ids.size() == 8);
}

BOOST_AUTO_TEST_SUITE_END()