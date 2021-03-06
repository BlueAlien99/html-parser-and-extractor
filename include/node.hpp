#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "node_iterator.hpp"

class Node {
public:
    enum class NodeType { HTML_ELEMENT, TEXT_CONTENT };

    virtual ~Node() = default;

    NodeType getType() { return type_; }
    unsigned int getId() { return id_; }

    virtual void addAttribute(const std::string&, const std::string&) {}
    virtual void insertNode(std::unique_ptr<Node>) {}

    virtual std::string getName() const { return ""; }
    virtual std::unordered_map<std::string, std::string> getAttributes() const { return {}; }
    virtual std::vector<std::unique_ptr<Node> > getNodes() const { return {}; }
    virtual std::vector<std::unique_ptr<Node> > getTextNodes() const { return {}; }
    virtual std::vector<std::unique_ptr<Node> > getHtmlNodes() const { return {}; }

    virtual std::string getAttributeValue(const std::string&) const { return ""; }
    virtual bool doesAttributeExist(const std::string&) const { return false; }

    virtual std::string getImmediateText() const = 0;
    virtual std::string getAllText() const = 0;

    virtual std::unique_ptr<Node> clone() const = 0;

    virtual NodeIterator begin() const { return NodeIterator(nullptr); }
    virtual NodeIterator end() const { return NodeIterator(nullptr); }

protected:
    Node(NodeType type, unsigned int id) : id_(id), type_(type) {}

    unsigned int id_;

private:
    NodeType type_;
};

class HtmlElement : public Node {
public:
    HtmlElement(std::string name, unsigned int id)
        : Node(NodeType::HTML_ELEMENT, id), name_(std::move(name)) {}

    void addAttribute(const std::string& name, const std::string& value) override;
    void insertNode(std::unique_ptr<Node> node) override;

    std::string getName() const override;
    std::unordered_map<std::string, std::string> getAttributes() const override;
    std::vector<std::unique_ptr<Node> > getNodes() const override;
    std::vector<std::unique_ptr<Node> > getTextNodes() const override;
    std::vector<std::unique_ptr<Node> > getHtmlNodes() const override;

    std::string getAttributeValue(const std::string& name) const override;
    bool doesAttributeExist(const std::string& name) const override;

    std::string getImmediateText() const override;
    std::string getAllText() const override;

    std::unique_ptr<Node> clone() const override;

    NodeIterator begin() const override;
    NodeIterator end() const override;

private:
    std::string getText(bool all) const;
    std::vector<std::unique_ptr<Node> > filterNodes(NodeType type) const;

    std::string name_;
    std::unordered_map<std::string, std::string> attributes_;
    std::vector<std::unique_ptr<Node> > nodes_;
};

class TextContent : public Node {
public:
    TextContent(std::string text, unsigned int id)
        : Node(NodeType::TEXT_CONTENT, id), text_(std::move(text)) {}
    std::string getImmediateText() const override { return text_; }
    std::string getAllText() const override { return text_; }

    std::unique_ptr<Node> clone() const override {
        return std::make_unique<TextContent>(text_, id_);
    }

private:
    std::string text_;
};

#endif