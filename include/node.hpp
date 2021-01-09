#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Node {
public:
    enum class NodeType { HTML_ELEMENT, TEXT_CONTENT };
    NodeType getType() { return type_; }

    virtual void addAttribute(const std::string&, const std::string&) {}
    virtual void insertNode(std::shared_ptr<Node>) {}

    virtual std::string getName() const { return ""; }
    virtual std::unordered_map<std::string, std::string> getAttributes() const { return {}; }
    virtual std::vector<std::shared_ptr<Node> > getNodes() const { return {}; }
    virtual std::vector<std::shared_ptr<Node> > getTextNodes() const { return {}; }
    virtual std::vector<std::shared_ptr<Node> > getHtmlNodes() const { return {}; }

    virtual std::string getAttributeValue(const std::string&) const { return ""; }
    virtual bool doesAttributeExist(const std::string&) const { return false; }

    virtual std::string getImmediateText() const = 0;
    virtual std::string getAllText() const = 0;

protected:
    Node(NodeType type) : type_(type) {}
    virtual ~Node() = default;

private:
    NodeType type_;
};

class Element : public Node {
public:
    Element(std::string name) : Node(NodeType::HTML_ELEMENT), name_(std::move(name)) {}

    void addAttribute(const std::string& name, const std::string& value) override;
    void insertNode(std::shared_ptr<Node> node) override;

    std::string getName() const override;
    std::unordered_map<std::string, std::string> getAttributes() const override;
    std::vector<std::shared_ptr<Node> > getNodes() const override;
    std::vector<std::shared_ptr<Node> > getTextNodes() const override;
    std::vector<std::shared_ptr<Node> > getHtmlNodes() const override;

    std::string getAttributeValue(const std::string& name) const override;
    bool doesAttributeExist(const std::string& name) const override;

    std::string getImmediateText() const override;
    std::string getAllText() const override;

private:
    std::string getText(bool all) const;
    std::vector<std::shared_ptr<Node> > filterNodes(NodeType type) const;

    std::string name_;
    std::unordered_map<std::string, std::string> attributes_;
    std::vector<std::shared_ptr<Node> > nodes_;
};

class TextContent : public Node {
public:
    TextContent(std::string text) : Node(NodeType::TEXT_CONTENT), text_(std::move(text)) {}
    std::string getImmediateText() const override { return text_; }
    std::string getAllText() const override { return text_; }

private:
    std::string text_;
};

#endif