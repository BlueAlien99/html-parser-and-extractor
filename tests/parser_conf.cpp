#include "exceptions.hpp"
#include "conf_parser.hpp"
#include "lexer.hpp"
#include "conf_object.hpp"
#include "sources.hpp"
#include "token.hpp"

#define BOOST_TEST_MODULE parser_conf
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(script) {
    // SourceFromFile src("./data/parser_html/script");
    // ConfParser parser(src);
    // std::shared_ptr<ConfObject> dom = parser.parseSafe(src);
    // BOOST_REQUIRE(dom != nullptr);
    // BOOST_CHECK(parser.getOpenNodes().back()->getName() == "_dom_");
    // auto nodes = dom->getHtmlNodes();
    // BOOST_CHECK(nodes.size() == 2);
}