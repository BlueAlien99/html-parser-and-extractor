#ifndef CONF_OBJECT_HPP
#define CONF_OBJECT_HPP

#include <memory>
#include <string>
#include <vector>

class ConfObject {
public:
    using VecStr = std::vector<std::string>;
    using VecPairStr = std::vector<std::pair<std::string, std::string> >;
    using VecPairInt = std::vector<std::pair<int, int> >;

    ConfObject(std::string tag = "") : tag_(tag), next_(nullptr) {}

    void setTag(std::string tag) { tag_ = tag; }
    void addClass(std::string class_name) { classes_.push_back(class_name); }
    void addId(std::string id) { ids_.push_back(id); }
    void addAttribute(std::string attribute_name) { attributes_.push_back(attribute_name); }
    void addAttributeValue(std::string name, std::string value) {
        attribute_values_.push_back({name, value});
    }
    void addRange(int from, int to) { ranges_.push_back({from, to}); }

    std::string getTag() const { return tag_; }
    VecStr getClasses() const { return classes_; }
    VecStr getIds() const { return ids_; }
    VecStr getAttributes() const { return attributes_; }
    VecPairStr getAttributeValues() const { return attribute_values_; }
    VecPairInt getRanges() const { return ranges_; }

    std::shared_ptr<ConfObject> createNextConf();
    std::shared_ptr<ConfObject> getNextConf() const;

    std::string getHumanReadableDescription() const;
    bool isValid() const;

private:
    std::string tag_;
    VecStr classes_;
    VecStr ids_;
    VecStr attributes_;
    VecPairStr attribute_values_;
    VecPairInt ranges_;

    std::shared_ptr<ConfObject> next_;
};

#endif