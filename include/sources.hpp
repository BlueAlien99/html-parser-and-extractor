#ifndef SOURCES_HPP
#define SOURCES_HPP

#include <fstream>
#include <string>

class AbstractSource {
public:
    virtual char getChar(int offset = 0) const = 0;

    char advance(int step = 1);
    char peek(int offset = 1) const;
    unsigned int getPosition() const;

protected:
    AbstractSource();

    unsigned int position_;
};

class SourceFromString : public AbstractSource {
public:
    SourceFromString(std::string str);
    char getChar(int offset = 0) const;

private:
    std::string buffer_;
};

class SourceFromFile : public AbstractSource {
public:
    SourceFromFile(std::string path);
    ~SourceFromFile();
    char getChar(int offset = 0) const;

private:
    mutable std::ifstream file_;
};

class SourceFromUrl : public AbstractSource {
public:
    static size_t writeCallback(char *content, size_t size, size_t nmemb, void *userdata);

    SourceFromUrl(std::string url);
    char getChar(int offset = 0) const;

private:
    std::string buffer_;
};

#endif