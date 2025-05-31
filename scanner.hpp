#ifndef SCANNER_H
#define SCANNER_H

#include "defs.hpp"

class Scanner {
public:
    const std::string input;
    Scanner(const std::string&);
    bool scan(Token&); // Scan the next token from the input string, updating the provided Token object

private:
    int position = 0; // Current position in the input string
    char currentChar() const; // Get the current character in the input string
    void next(); // Move to the next character in the input string
    void skip(); // Skip whitespace characters in the input string
    bool scanTag(Token&); // Scan a start or end tag
    bool scanAttribute(std::string&); // Scan an attribute in the tag in the format: key="value"
    bool scanContent(Token&); // Scan content between tags

    bool isspace(char c) const; // Check if a character is either a space, tab, or newline
    bool isNameStartChar(char c) const; // Check if a character is a valid starting character for a tag or attribute name
    bool isNameChar(char c) const; // Check if a character is a valid character for a tag or attribute name
};

#endif // SCANNER_H