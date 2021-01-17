#ifndef NODE_ITERATOR_HPP
#define NODE_ITERATOR_HPP

#include <deque>
#include <memory>

class Node;

class NodeIterator {
public:
    NodeIterator(std::unique_ptr<Node> node);
    bool operator!=(NodeIterator rhs);
    std::unique_ptr<Node> operator*();
    void operator++();

private:
    std::deque<std::unique_ptr<Node> > nodes_;
};

#endif