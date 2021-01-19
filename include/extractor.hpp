#ifndef EXTRACTOR_HPP
#define EXTRACTOR_HPP

#include <memory>

#include "conf_object.hpp"
#include "node.hpp"
#include "sources.hpp"

class Extractor {
public:
    static std::unique_ptr<Node> extract(std::unique_ptr<Node> node, AbstractSource& conf_source,
                                         bool safe);

private:
    static bool find(const std::vector<std::string>& to_find, std::string to_split);
    static bool findAttributes(std::unique_ptr<Node> node, const ConfObject::VecStr& attrs,
                               const ConfObject::VecPairStr& attr_vals);
    static std::unique_ptr<HtmlElement> filterDuplicates(std::unique_ptr<HtmlElement> filter);
    static std::unique_ptr<HtmlElement> filterRanges(std::unique_ptr<HtmlElement> filter,
                                                     const ConfObject::VecPairInt& ranges);
};

#endif