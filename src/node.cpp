#include "node.hpp"

#include "utils.hpp"

void HtmlElement::addAttribute(const std::string& name, const std::string& value) {
    attributes_.insert_or_assign(name, value);
}

void HtmlElement::insertNode(std::shared_ptr<Node> node) { nodes_.push_back(node); }

std::string HtmlElement::getName() const { return name_; }

std::unordered_map<std::string, std::string> HtmlElement::getAttributes() const {
    return attributes_;
}

std::vector<std::shared_ptr<Node> > HtmlElement::getNodes() const { return nodes_; }

std::vector<std::shared_ptr<Node> > HtmlElement::getTextNodes() const {
    return filterNodes(NodeType::TEXT_CONTENT);
}

std::vector<std::shared_ptr<Node> > HtmlElement::getHtmlNodes() const {
    return filterNodes(NodeType::HTML_ELEMENT);
}

std::string HtmlElement::getAttributeValue(const std::string& name) const {
    return attributes_.at(name);
}

bool HtmlElement::doesAttributeExist(const std::string& name) const {
    auto attr = attributes_.find(name);
    if (attr != attributes_.end()) {
        return true;
    }
    return false;
}

std::string HtmlElement::getImmediateText() const { return getText(false); }

std::string HtmlElement::getAllText() const { return getText(true); }

std::string HtmlElement::getText(bool all) const {
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

std::vector<std::shared_ptr<Node> > HtmlElement::filterNodes(NodeType type) const {
    std::vector<std::shared_ptr<Node> > ret;
    for (std::shared_ptr<Node> node : nodes_) {
        if (node->getType() == type) {
            ret.push_back(node);
        }
    }
    return ret;
}