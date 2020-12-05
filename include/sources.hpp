#ifndef SOURCES_HPP
#define SOURCES_HPP

#include <string>

class AbstractSource {
public:
    char getNextChar();
    char getLastChar() const;

protected:
    AbstractSource();

    std::string buffer_;
    int position_;
};

class SourceFromString : public AbstractSource {
public:
    SourceFromString(std::string str);
};

class SourceFromFile : public AbstractSource {
public:
    SourceFromFile(std::string path);
};

class SourceFromUrl : public AbstractSource {
public:
    static size_t writeCallback(char *content, size_t size, size_t nmemb, void *userdata);

    SourceFromUrl(std::string url);
};

#endif