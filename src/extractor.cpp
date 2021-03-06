#include "extractor.hpp"

#include <boost/algorithm/string.hpp>
#include <memory>
#include <set>
#include <vector>

#include "conf_object.hpp"
#include "conf_parser.hpp"
#include "node.hpp"
#include "sources.hpp"

std::unique_ptr<Node> Extractor::extract(std::unique_ptr<Node> node, AbstractSource& conf_source,
                                         bool safe) {
    auto filter = std::make_unique<HtmlElement>("_filter_", 0);
    filter->insertNode(std::move(node));

    std::unique_ptr<ConfObject> conf;
    ConfParser conf_parser(conf_source);
    if (safe) {
        conf = conf_parser.parseSafe(conf_source);
        if (conf == nullptr) {
            return nullptr;
        }
    } else {
        conf = conf_parser.parse();
    }

    while (conf != nullptr) {
        std::string tag = conf->getTag();
        auto classes = conf->getClasses();
        auto ids = conf->getIds();
        auto attrs = conf->getAttributes();
        auto attr_vals = conf->getAttributeValues();
        auto ranges = conf->getRanges();

        auto filtered_nodes = filter->getHtmlNodes();
        filter = std::make_unique<HtmlElement>("_filter_", 0);

        for (unsigned int i = 0; i < filtered_nodes.size(); ++i) {
            for (NodeIterator iter = filtered_nodes[i]->begin(); iter != filtered_nodes[i]->end();
                 ++iter) {
                std::unique_ptr<Node> node = std::move(*iter);

                if (!tag.empty() && node->getName() != tag) {
                    continue;
                }
                if (!classes.empty() && (!node->doesAttributeExist("class") ||
                                         !find(classes, node->getAttributeValue("class")))) {
                    continue;
                }
                if (!ids.empty() && (!node->doesAttributeExist("id") ||
                                     !find(ids, node->getAttributeValue("id")))) {
                    continue;
                }
                if (!findAttributes(node->clone(), attrs, attr_vals)) {
                    continue;
                }

                bool bad_value = false;
                for (const auto& attr : attr_vals) {
                    if (node->getAttributeValue(attr.first) != attr.second) {
                        bad_value = true;
                        break;
                    }
                }
                if (bad_value) {
                    continue;
                }

                filter->insertNode(std::move(node));
            }
        }
        filter = filterDuplicates(std::move(filter));
        filter = filterRanges(std::move(filter), ranges);
        conf = conf->getNextConf();
    }
    return filter;
}

bool Extractor::find(const std::vector<std::string>& to_find, std::string to_split) {
    std::vector<std::string> split;
    boost::split(split, to_split, boost::is_any_of("\t\n "));
    unsigned int count = 0;
    for (const std::string& f : to_find) {
        for (const std::string& s : split) {
            if (s == f) {
                ++count;
                break;
            }
        }
    }
    if (count == to_find.size()) {
        return true;
    }
    return false;
}

bool Extractor::findAttributes(std::unique_ptr<Node> node, const ConfObject::VecStr& attrs,
                               const ConfObject::VecPairStr& attr_vals) {
    for (const std::string& attr : attrs) {
        if (!node->doesAttributeExist(attr)) {
            return false;
        }
    }
    for (const auto& attr : attr_vals) {
        if (!node->doesAttributeExist(attr.first)) {
            return false;
        }
    }
    return true;
}

std::unique_ptr<HtmlElement> Extractor::filterDuplicates(std::unique_ptr<HtmlElement> filter) {
    auto filtered_nodes = filter->getHtmlNodes();
    filter = std::make_unique<HtmlElement>("_filter_", 0);
    std::set<unsigned int> uids;
    for (unsigned int i = 0; i < filtered_nodes.size(); ++i) {
        unsigned int id = filtered_nodes[i]->getId();
        if (uids.count(id) == 0) {
            uids.insert(id);
            filter->insertNode(std::move(filtered_nodes[i]));
        }
    }
    return filter;
}

std::unique_ptr<HtmlElement> Extractor::filterRanges(std::unique_ptr<HtmlElement> filter,
                                                     const ConfObject::VecPairInt& ranges) {
    if (ranges.empty()) {
        return filter;
    }
    auto filtered_nodes = filter->getHtmlNodes();
    filter = std::make_unique<HtmlElement>("_filter_", 0);
    int s = filtered_nodes.size();
    for (const std::pair<int, int>& range : ranges) {
        int i = range.first < 0 ? range.first + s : range.first;
        int e = range.second < 0 ? range.second + s : range.second;
        i = std::max(i, 0);
        e = std::min(e, s - 1);
        while (i <= e) {
            if (filtered_nodes[i] != nullptr) {
                filter->insertNode(std::move(filtered_nodes[i]));
            }
            ++i;
        }
    }
    return filterDuplicates(std::move(filter));
}
