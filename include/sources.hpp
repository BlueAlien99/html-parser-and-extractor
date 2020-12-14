#ifndef SOURCES_HPP
#define SOURCES_HPP

#include <fstream>
#include <istream>
#include <memory>
#include <sstream>

class AbstractSource {
public:
    char advance(int offset = 0);
    char peek() const;
    char getChar() const;
    unsigned int getPosition() const;

protected:
    AbstractSource();

    char current_char_;
    std::unique_ptr<std::istream> stream_;
};

class SourceFromString : public AbstractSource {
public:
    SourceFromString(std::string str);

private:
    std::unique_ptr<std::stringbuf> stringbuf_;
};

class SourceFromFile : public AbstractSource {
public:
    SourceFromFile(std::string path);
    ~SourceFromFile();

private:
    std::unique_ptr<std::filebuf> filebuf_;
};

class SourceFromUrl : public AbstractSource {
public:
    static size_t writeCallback(char *content, size_t size, size_t nmemb, void *userdata);

    SourceFromUrl(std::string url);

private:
    std::unique_ptr<std::stringbuf> stringbuf_;
};

#endif