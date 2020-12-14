#include "sources.hpp"

#include <curl/curl.h>

#include <fstream>
#include <istream>
#include <memory>
#include <sstream>
#include <string>

//
// AbstractSource
//
AbstractSource::AbstractSource() : current_char_(0) {}

char AbstractSource::advance(int offset) {
    stream_->clear();
    stream_->seekg(offset > 0 ? offset - 1 : offset, std::ios_base::cur);
    current_char_ = stream_->get();
    if (current_char_ == EOF) {
        current_char_ = 0;
    }
    return current_char_;
}

char AbstractSource::peek() const {
    char c = stream_->peek();
    if (c == EOF) {
        return 0;
    }
    return c;
}

char AbstractSource::getChar() const { return current_char_; }

unsigned int AbstractSource::getPosition() const { return stream_->tellg(); }

//
// SourceFromString
//
SourceFromString::SourceFromString(std::string str) {
    stringbuf_ = std::make_unique<std::stringbuf>(str);
    stream_ = std::make_unique<std::istream>(stringbuf_.get());
    advance();
}

//
// SourceFromFile
//
SourceFromFile::SourceFromFile(std::string path) {
    filebuf_ = std::make_unique<std::filebuf>();
    filebuf_->open(path, std::ios::in);
    stream_ = std::make_unique<std::istream>(filebuf_.get());
    advance();
}

SourceFromFile::~SourceFromFile() { filebuf_->close(); }

//
// SourceFromUrl
//
SourceFromUrl::SourceFromUrl(std::string url) {
    std::string buffer;
    CURL *curl;
    // CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        // res = curl_easy_perform(curl);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    stringbuf_ = std::make_unique<std::stringbuf>(buffer);
    stream_ = std::make_unique<std::istream>(stringbuf_.get());
    advance();
}

size_t SourceFromUrl::writeCallback(char *content, size_t size, size_t nmemb, void *userdata) {
    char *content_ptr = static_cast<char *>(content);
    std::string *userdata_ptr = static_cast<std::string *>(userdata);
    userdata_ptr->append(content_ptr, size * nmemb);
    return size * nmemb;
}
