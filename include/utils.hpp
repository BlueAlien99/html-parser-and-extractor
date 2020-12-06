#ifndef UTILS_HPP
#define UTILS_HPP

bool isSpace(char c) { return (c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\r'); }

bool isAlpha(char c) {
    bool uppercase = (c >= 'A' && c <= 'Z');
    bool lowercase = (c >= 'a' && c <= 'z');
    bool utf8 = ((signed char)c < 0);
    return (uppercase || lowercase || utf8);
}

bool isNum(char c) { return (c >= '0' && c <= '9'); }

bool isAlphanum(char c) { return (isNum(c) || isAlpha(c)); }

bool isSymbol(char c) { return (!isAlphanum(c) && !isSpace(c) && c >= 33 && c < 127); }

#endif