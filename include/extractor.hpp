#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP

#include <memory>

#include "node.hpp"
#include "sources.hpp"

class Extractor {
public:
    static std::unique_ptr<Node> extract(std::unique_ptr<Node> node, AbstractSource& conf_source,
                                         bool safe);

private:
    static bool find(const std::vector<std::string>& to_find, std::string to_split);
    static std::unique_ptr<HtmlElement> filterDuplicates(std::unique_ptr<HtmlElement> filter);
};

#endif