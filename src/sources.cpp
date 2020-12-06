#include "sources.hpp"

#include <curl/curl.h>

#include <fstream>
#include <string>

char AbstractSource::getNextChar() {
    ++position_;
    return getLastChar();
}

char AbstractSource::getLastChar() const {
    if (position_ >= 0 && (unsigned int)position_ < buffer_.length()) {
        return buffer_[position_];
    }
    return '\0';
}

void AbstractSource::setPosition(int pos) { position_ = pos; }

int AbstractSource::getPosition() const { return position_; }

void AbstractSource::rewind() { --position_; }

AbstractSource::AbstractSource() : position_(-1) {}

SourceFromString::SourceFromString(std::string str) { buffer_ = str; }

SourceFromFile::SourceFromFile(std::string path) {
    std::ifstream file(path);
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        buffer_ = std::string(size, '\0');
        file.seekg(0);
        file.read(&buffer_[0], size);
    }
    file.close();
}

size_t SourceFromUrl::writeCallback(char *content, size_t size, size_t nmemb, void *userdata) {
    char *content_ptr = static_cast<char *>(content);
    std::string *userdata_ptr = static_cast<std::string *>(userdata);
    userdata_ptr->append(content_ptr, size * nmemb);
    return size * nmemb;
}

SourceFromUrl::SourceFromUrl(std::string url) {
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
