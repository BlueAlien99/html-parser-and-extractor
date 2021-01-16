#include "conf_object.hpp"

ConfObject::ConfObject(const ConfObject& confObject) {
    tag_ = confObject.tag_;
    classes_ = confObject.classes_;
    ids_ = confObject.ids_;
    attributes_ = confObject.attributes_;
    attribute_values_ = confObject.attribute_values_;
    ranges_ = confObject.ranges_;
    next_ = nullptr;
    if (confObject.next_ != nullptr) {
        next_ = std::make_unique<ConfObject>(*confObject.next_);
    }
}

void ConfObject::setNextConf(std::unique_ptr<ConfObject> next) { next_ = std::move(next); }

std::unique_ptr<ConfObject> ConfObject::getNextConf() const {
    if (next_ != nullptr && next_->isValid()) {
        return std::make_unique<ConfObject>(*next_);
    }
    return nullptr;
}

std::string ConfObject::getHumanReadableDescription() const {
    std::string desc;
    if (tag_ != "") {
        desc += "\ntag: " + tag_;
    }
    if (!classes_.empty()) {
        desc += "\nclasses: ";
        for (const auto& c : classes_) {
            desc += c + " ";
        }
    }
    if (!ids_.empty()) {
        desc += "\nids: ";
        for (const auto& i : ids_) {
            desc += i + " ";
        }
    }
    if (!attributes_.empty()) {
        desc += "\nattributes: ";
        for (const auto& a : attributes_) {
            desc += a + " ";
        }
    }
    if (!attribute_values_.empty()) {
        desc += "\nattr values: ";
        for (const auto& p : attribute_values_) {
            desc += "{" + p.first + " = " + p.second + "} ";
        }
    }
    if (!ranges_.empty()) {
        desc += "\nranges: ";
        for (const auto& p : ranges_) {
            desc += "(" + std::to_string(p.first) + ", " + std::to_string(p.second) + ") ";
        }
    }
    if (getNextConf() != nullptr) {
        desc += "\n\n\tthen\n";
        desc += next_->getHumanReadableDescription();
    }
    return desc;
}

bool ConfObject::isValid() const {
    if (tag_ == "" && classes_.empty() && ids_.empty() && attributes_.empty() &&
        attribute_values_.empty() && ranges_.empty()) {
        return false;
    }
    return true;
}
