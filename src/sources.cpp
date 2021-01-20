#include "sources.hpp"

#include <curl/curl.h>

#include <fstream>
#include <istream>
#include <memory>
#include <sstream>
#include <string>

#include "token.hpp"

//
// AbstractSource
//
AbstractSource::AbstractSource() : current_char_(0), position_(Position(0, 0, 1, 0)) {}

char AbstractSource::advance(unsigned int step) {
    if (step == 0) {
        step = 1;
    }
    while (--step) {
        updatePosition(stream_->get());
    }
    current_char_ = stream_->get();
    if (current_char_ == EOF) {
        current_char_ = 0;
    }
    updatePosition(current_char_);
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

Position AbstractSource::getPosition() const { return position_; }

std::string AbstractSource::getTextAtPosition(const Position &pos) {
    const int PAST_POS = 8;
    stream_->clear();
    stream_->seekg(0, std::ios_base::end);
    int max_pos = stream_->tellg();
    int diff = max_pos - (pos.src_pos + PAST_POS);
    int len = pos.src_pos - pos.src_line_pos + PAST_POS;
    if (diff < 0) {
        len += diff;
    }
    char *buffer = new char[len + 1];
    buffer[len] = 0;
    stream_->clear();
    stream_->seekg(pos.src_line_pos);
    stream_->read(buffer, len);
    std::string text(buffer);
    delete[] buffer;
    stream_->clear();
    stream_->seekg(position_.src_pos);
    return text;
}

void AbstractSource::updatePosition(char c) {
    if (c == 0 || c == EOF) {
        return;
    }
    position_.src_pos = stream_->tellg();
    if (c == '\n') {
        position_.src_line_pos = position_.src_pos;
        ++position_.line;
        position_.column = 0;
    } else {
        ++position_.column;
    }
}

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
