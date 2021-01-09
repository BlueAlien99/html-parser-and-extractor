#include "node.hpp"

#include "utils.hpp"

void Element::addAttribute(const std::string& name, const std::string& value) {
    attributes_.insert_or_assign(name, value);
}

void Element::insertNode(std::shared_ptr<Node> node) { nodes_.push_back(node); }

std::string Element::getName() const { return name_; }

std::unordered_map<std::string, std::string> Element::getAttributes() const { return attributes_; }

std::vector<std::shared_ptr<Node> > Element::getNodes() const { return nodes_; }

std::vector<std::shared_ptr<Node> > Element::getTextNodes() const {
    return filterNodes(NodeType::TEXT_CONTENT);
}

std::vector<std::shared_ptr<Node> > Element::getHtmlNodes() const {
    return filterNodes(NodeType::HTML_ELEMENT);
}

std::string Element::getAttributeValue(const std::string& name) const {
    return attributes_.at(name);
}

bool Element::doesAttributeExist(const std::string& name) const {
    auto attr = attributes_.find(name);
    if (attr != attributes_.end()) {
        return true;
    }
    return false;
}

std::string Element::getImmediateText() const { return getText(false); }

std::string Element::getAllText() const { return getText(true); }

std::string Element::getText(bool all) const {
    if (name_ == "br") {
        return "\n";
    }
    std::string text;
    for (std::shared_ptr<Node> node : nodes_) {
        if (node->getType() == NodeType::TEXT_CONTENT || all) {
            std::string node_text = node->getAllText();
            if (!utils::isReplaceableTagName(name_)) {
                if ((text.empty() || (text.size() && (text[text.size() - 1] == ' ' ||
                                                      text[text.size() - 1] == '\n'))) &&
                    node_text.size() && node_text[0] == ' ') {
                    node_text = node_text.substr(1);
                }
            }
            text += node_text;
        }
    }
    return text;
}

std::vector<std::shared_ptr<Node> > Element::filterNodes(NodeType type) const {
    std::vector<std::shared_ptr<Node> > ret;
    for (std::shared_ptr<Node> node : nodes_) {
        if (node->getType() == type) {
            ret.push_back(node);
        }
    }
    return ret;
}