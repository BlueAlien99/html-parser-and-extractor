#include "node_iterator.hpp"

#include <memory>
#include <vector>

#include "node.hpp"

NodeIterator::NodeIterator(std::unique_ptr<Node> node) {
    if (node != nullptr) {
        nodes_.push_back(std::move(node));
        ++(*this);
    }
}

bool NodeIterator::operator!=(NodeIterator rhs) {
    if (nodes_.empty() && *rhs == nullptr) {
        return false;
    }
    return true;
}

std::unique_ptr<Node> NodeIterator::operator*() {
    if (!nodes_.empty()) {
        return nodes_.front()->clone();
    }
    return nullptr;
}

void NodeIterator::operator++() {
    std::vector<std::unique_ptr<Node> > to_push = nodes_.front()->getHtmlNodes();
    for (unsigned int i = 0; i < to_push.size(); ++i) {
        nodes_.push_back(std::move(to_push[i]));
    }
    nodes_.pop_front();
}
