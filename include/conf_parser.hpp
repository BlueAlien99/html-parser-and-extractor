#ifndef CONF_PARSER_HPP
#define CONF_PARSER_HPP

#include <memory>
#include <vector>

#include "conf_object.hpp"
#include "lexer.hpp"
#include "sources.hpp"
#include "token.hpp"

class ConfParser {
public:
    ConfParser(AbstractSource& source);
    std::shared_ptr<ConfObject> parse();
    std::shared_ptr<ConfObject> parseSafe(AbstractSource& source);

private:
    void buildClassOrId();
    void buildAttribute();
    void buildRange();

    std::pair<std::string, int> buildRangeBoundary();

    std::unique_ptr<ConfLexer> lexer_;
    std::shared_ptr<ConfObject> conf_;
    std::shared_ptr<ConfObject> current_conf_;
};

#endif