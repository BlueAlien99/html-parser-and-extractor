#include "conf_parser.hpp"

#include <boost/test/unit_test.hpp>

#include "conf_object.hpp"
#include "exceptions.hpp"
#include "lexer.hpp"
#include "sources.hpp"
#include "token.hpp"

BOOST_AUTO_TEST_SUITE(conf_parser)

BOOST_AUTO_TEST_CASE(basic) {
    SourceFromString src("span.hl.long#unique(5)");
    ConfParser parser(src);
    std::shared_ptr<ConfObject> config = parser.parseSafe(src);
    BOOST_REQUIRE(config != nullptr);
    BOOST_CHECK(config->getTag() == "span");
    std::vector<std::string> classes = {"hl", "long"};
    BOOST_CHECK(config->getClasses() == classes);
    std::vector<std::string> ids = {"unique"};
    BOOST_CHECK(config->getIds() == ids);
    ConfObject::VecPairInt ranges = {{5, 5}};
    BOOST_CHECK(config->getRanges() == ranges);
}

BOOST_AUTO_TEST_CASE(ranges) {
    SourceFromString src("a(1)(5:)(:3)(-2:6)(:)(10)()");
    ConfParser parser(src);
    std::shared_ptr<ConfObject> config = parser.parseSafe(src);
    BOOST_REQUIRE(config != nullptr);
    ConfObject::VecPairInt ranges = {{1, 1}, {5, -1}, {0, 3}, {-2, 6}, {0, -1}, {10, 10}, {0, 0}};
    BOOST_CHECK(config->getRanges() == ranges);
}

BOOST_AUTO_TEST_CASE(nesting) {
    SourceFromString src("div.gr [data] (7:9).xyz#foo#bar a[href='google.com']");
    ConfParser parser(src);
    std::shared_ptr<ConfObject> config = parser.parseSafe(src);
    BOOST_REQUIRE(config != nullptr);
    BOOST_CHECK(config->getTag() == "div");
    std::vector<std::string> classes = {"gr"};
    BOOST_CHECK(config->getClasses() == classes);
    config = config->getNextConf();
    BOOST_CHECK(config != nullptr);
    std::vector<std::string> attr = {"data"};
    BOOST_CHECK(config->getAttributes() == attr);
    config = config->getNextConf();
    BOOST_CHECK(config != nullptr);
    classes = {"xyz"};
    BOOST_CHECK(config->getClasses() == classes);
    std::vector<std::string> ids = {"foo", "bar"};
    BOOST_CHECK(config->getIds() == ids);
    ConfObject::VecPairInt ranges = {{7, 9}};
    BOOST_CHECK(config->getRanges() == ranges);
    config = config->getNextConf();
    BOOST_CHECK(config != nullptr);
    ConfObject::VecPairStr attr_vals = {{"href", "google.com"}};
    BOOST_CHECK(config->getAttributeValues() == attr_vals);
    BOOST_CHECK(config->getNextConf() == nullptr);
}

BOOST_AUTO_TEST_CASE(attributes) {
    SourceFromString src(
        "[id = \"div>' x'!--\"][class='--> \"/html>'][__--__] "
        "[no_val-da<sh][unq='__--__'][tag=\"li/>'\"]");
    ConfParser parser(src);
    std::shared_ptr<ConfObject> config = parser.parseSafe(src);
    BOOST_REQUIRE(config != nullptr);
    BOOST_CHECK(config->getTag() == "");
    std::vector<std::string> attr = {"__--__"};
    ConfObject::VecPairStr attr_vals = {{"id", "div>' x'!--"}, {"class", "--> \"/html>"}};
    BOOST_CHECK(config->getAttributes() == attr);
    BOOST_CHECK(config->getAttributeValues() == attr_vals);
    config = config->getNextConf();
    BOOST_CHECK(config != nullptr);
    attr = {"no_val-da<sh"};
    attr_vals = {{"unq", "__--__"}, {"tag", "li/>'"}};
    BOOST_CHECK(config->getAttributes() == attr);
    BOOST_CHECK(config->getAttributeValues() == attr_vals);
}

BOOST_AUTO_TEST_CASE(reorder) {
    SourceFromString src("[action]button div(6)img");
    ConfParser parser(src);
    std::shared_ptr<ConfObject> config = parser.parseSafe(src);
    BOOST_REQUIRE(config != nullptr);
    BOOST_CHECK(config->getTag() == "button");
    std::vector<std::string> attr = {"action"};
    BOOST_CHECK(config->getAttributes() == attr);
    config = config->getNextConf();
    BOOST_CHECK(config != nullptr);
    BOOST_CHECK(config->getTag() == "img");
    ConfObject::VecPairInt ranges = {{6, 6}};
    BOOST_CHECK(config->getRanges() == ranges);
}

BOOST_AUTO_TEST_CASE(non_standard_data) {
    SourceFromString src(".--__--#-_-_.____#__id__()img");
    ConfParser parser(src);
    std::shared_ptr<ConfObject> config = parser.parseSafe(src);
    BOOST_REQUIRE(config != nullptr);
    BOOST_CHECK(config->getTag() == "img");
    std::vector<std::string> classes = {"--__--", "____"};
    BOOST_CHECK(config->getClasses() == classes);
    std::vector<std::string> ids = {"-_-_", "__id__"};
    BOOST_CHECK(config->getIds() == ids);
    ConfObject::VecPairInt ranges = {{0, 0}};
    BOOST_CHECK(config->getRanges() == ranges);
}

BOOST_AUTO_TEST_SUITE_END()
