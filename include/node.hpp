#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Node {
protected:
    Node() = default;
    virtual ~Node() = default;
};

class Element : public Node {
public:
    Element(std::string name) : name_(std::move(name)) {}

    void addAttribute(const std::string& name, const std::string& value);
    void insertNode(std::shared_ptr<Node> node);

    std::string getName() const;
    std::unordered_map<std::string, std::string> getAttributes() const;
    std::vector<std::shared_ptr<Node> > getNodes() const;

    std::string getAttributeValue(const std::string& name) const;
    bool doesAttributeExist(const std::string& name) const;

    std::string getImmediateText() const;
    std::string getAllText() const;

private:
    std::string getText(bool all) const;

    std::string name_;
    std::unordered_map<std::string, std::string> attributes_;
    std::vector<std::shared_ptr<Node> > nodes_;
};

class TextContent : public Node {
public:
    TextContent(std::string text) : text_(std::move(text)) {}
    std::string getText() const { return text_; }

private:
    std::string text_;
};

#endif