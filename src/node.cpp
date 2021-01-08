#include "node.hpp"

void Element::addAttribute(const std::string& name, const std::string& value) {
    attributes_.insert_or_assign(name, value);
}

void Element::insertNode(std::shared_ptr<Node> node) { nodes_.push_back(node); }

std::string Element::getName() const { return name_; }

std::unordered_map<std::string, std::string> Element::getAttributes() const { return attributes_; }

std::vector<std::shared_ptr<Node> > Element::getNodes() const { return nodes_; }

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
    std::string text;
    for (std::shared_ptr<Node> node : nodes_) {
        auto textContent = std::dynamic_pointer_cast<TextContent>(node);
        if (textContent) {
            text += textContent->getText();
        } else if (all) {
            auto element = std::dynamic_pointer_cast<Element>(node);
            if (element) {
                text += element->getAllText();
            }
        }
    }
    return text;
}