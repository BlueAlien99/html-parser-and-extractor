#ifndef SOURCES_HPP
#define SOURCES_HPP

#include <fstream>
#include <string>

class AbstractSource {
public:
    virtual char getNextChar() = 0;
};

class SourceFromString : public AbstractSource {
public:
    SourceFromString(std::string str);
    char getNextChar();

private:
    std::string str_;
    unsigned int position_;
};

class SourceFromFile : public AbstractSource {
public:
    SourceFromFile(std::string path);
    ~SourceFromFile();
    char getNextChar();

private:
    std::ifstream file_;
};

class SourceFromUrl : public AbstractSource {
public:
    static size_t writeCallback(char *content, size_t size, size_t nmemb, void *userdata);

    SourceFromUrl(std::string url);
    char getNextChar();

private:
    std::string buffer_;
    unsigned int position_;
};

#endif