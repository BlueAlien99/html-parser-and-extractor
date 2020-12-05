#include "sources.hpp"

#include <curl/curl.h>

#include <fstream>
#include <string>

SourceFromString::SourceFromString(std::string str) : str_(str), position_(0) {}

char SourceFromString::getNextChar() {
    if (position_ < str_.length()) {
        return str_[position_++];
    }
    return '\0';
}

SourceFromFile::SourceFromFile(std::string path) { file_.open(path); }

SourceFromFile::~SourceFromFile() { file_.close(); }

char SourceFromFile::getNextChar() {
    char c;
    if (file_.get(c)) {
        return c;
    }
    return '\0';
}

size_t SourceFromUrl::writeCallback(char *content, size_t size, size_t nmemb, void *userdata) {
    char *content_ptr = static_cast<char *>(content);
    std::string *userdata_ptr = static_cast<std::string *>(userdata);
    userdata_ptr->append(content_ptr, size * nmemb);
    return size * nmemb;
}

SourceFromUrl::SourceFromUrl(std::string url) : position_(0) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer_);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
}

char SourceFromUrl::getNextChar() {
    if (position_ < buffer_.length()) {
        return buffer_[position_++];
    }
    return '\0';
}