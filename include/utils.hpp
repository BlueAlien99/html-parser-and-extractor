#ifndef UTILS_HPP
#define UTILS_HPP

#include <set>
#include <string>

namespace utils {

bool isVoidElement(std::string name) {
    static const std::set<std::string> void_elems_ = {
        "area",  "base",   "br",   "col",  "command", "embed",  "hr",    "img",
        "input", "keygen", "link", "meta", "param",   "source", "track", "wbr"};
    if (void_elems_.find(name) != void_elems_.end()) {
        return true;
    }
    return false;
}

}  // namespace config_data

#endif