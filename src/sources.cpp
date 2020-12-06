#include "sources.hpp"

#include <curl/curl.h>

#include <fstream>
#include <string>

//
// AbstractSource
//
AbstractSource::AbstractSource() : position_(0) {}

char AbstractSource::advance(int step) {
    position_ += step;
    return getChar();
}

char AbstractSource::peek(int offset) const { return getChar(offset); }

unsigned int AbstractSource::getPosition() const { return position_; }

//
// SourceFromString
//
SourceFromString::SourceFromString(std::string str) : buffer_(str) {}

char SourceFromString::getChar(int offset) const {
    unsigned int pos = position_ + offset;
    if (pos < buffer_.length()) {
        return buffer_[pos];
    }
    return '\0';
}

//
// SourceFromFile
//
SourceFromFile::SourceFromFile(std::string path) { file_.open(path, std::ios::in); }

SourceFromFile::~SourceFromFile() { file_.close(); }

char SourceFromFile::getChar(int offset) const {
    unsigned int pos = position_ + offset;
    file_.seekg(pos, std::ios::beg);
    char c = '\0';
    if (file_.get(c)) {
        return c;
    }
    return '\0';
}

//
// SourceFromUrl
//
SourceFromUrl::SourceFromUrl(std::string url) {
    CURL *curl;
    // CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer_);
        // res = curl_easy_perform(curl);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}

size_t SourceFromUrl::writeCallback(char *content, size_t size, size_t nmemb, void *userdata) {
    char *content_ptr = static_cast<char *>(content);
    std::string *userdata_ptr = static_cast<std::string *>(userdata);
    userdata_ptr->append(content_ptr, size * nmemb);
    return size * nmemb;
}

char SourceFromUrl::getChar(int offset) const {
    unsigned int pos = position_ + offset;
    if (pos < buffer_.length()) {
        return buffer_[pos];
    }
    return '\0';
}
