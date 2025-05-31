#include <cctype>
#include "scanner.hpp"

Scanner::Scanner(const std::string& input) : input(input) {}

bool Scanner::scan(Token& token) {
    skip();

    // Return a EOF token if we reached the end of input
    if (currentChar() == '\0') {
        token = Token();
        return true;
    }
    // Check the current character in the input string and decide whether to scan a tag or content
    if (currentChar() == '<') {
        return scanTag(token);
    } else {
        return scanContent(token);
    }
}

char Scanner::currentChar() const {
    // Return the current character in the input string
    if (position < input.size()) {
        return input[position];
    }
    return '\0'; // Indicate end of input
}

void Scanner::skip() {
    // Skip whitespace characters in the input string
    while (position < input.size() && isspace(input[position])) {
        ++position;
    }
}

void Scanner::next() {
    // Move to the next character in the input string
    if (position < input.size()) {
        ++position;
    }
    return;
}

bool Scanner::isspace(char c) const {
    if (c == ' ' || c == '\t' || c == '\n') {
        return true; // Character is a whitespace character
    }
    return false; // Character is not a whitespace character
}

bool Scanner::isNameStartChar(char c) const {
    // Check if the character is a valid starting character for a tag or attribute name
    return std::isalpha(c) || c == '_';
}

bool Scanner::isNameChar(char c) const {
    // Check if the character is a valid character for a tag or attribute name
    return Scanner::isNameStartChar(c) || c == '-' || c == '.' || std::isdigit(c);
}

bool Scanner::scanAttribute(std::string& attributeString) {
    // Scan an attribute in the format: key="value" and return true if successful
    
    // Check if the current character is a valid starting character for an attribute key
    if (isNameStartChar(currentChar())) {
        attributeString += currentChar();
        next();
    } else {
        return false;
    }

    // Continue scanning the attribute key
    while (isNameChar(currentChar())) {
        attributeString += currentChar();
        next();
    }

    // Skip whitespace after the attribute key
    skip();

    // Check if the next character is '=' after the attribute key
    if (currentChar() == '=') {
        attributeString += '=';
        next();
    } else {
        return false;
    }

    // Skip whitespace after the '='
    skip();

    // Check if the next character is a quotation mark to start the attribute value
    char quotationMark;
    if (currentChar() == '"') {
        attributeString += '"';
        quotationMark = '"';
        next();
    } else if (currentChar() == '\'') {
        attributeString += '\'';
        quotationMark = '\'';
        next();
    } else {
        return false;
    }

    // Continue scanning the attribute value until we find the closing quotation mark
    while (currentChar() != quotationMark && currentChar() != '\0') {
        attributeString += currentChar();
        next();
    }

    // Check if we found the closing quotation mark for the attribute value
    if (currentChar() == quotationMark) {
        attributeString += quotationMark;
        next();
        return true; // Successfully scanned an attributeq
    } else {
        return false; // Expected a quotation mark to close attribute value
    }
}

bool Scanner::scanTag(Token& token) {
    // Scan a start tag and update the token
    if(currentChar() != '<') {
        return false; // Not a start tag
    }
    next();

    // Check if the next character is '/' which indicates an end tag
    if(currentChar() == '/') {
        token.type = TokenType::TOK_END_TAG;
        next();
    } else {
        token.type = TokenType::TOK_START_TAG;
    }

    // Check if the next character is a valid starting character for a tag name
    if (isNameStartChar(currentChar())) {
        token.tagType += currentChar();
        next();
    } else {
        // Not a valid tag name start character, reset token and return false
        token = Token();
        return false;
    }
    // Start scanning the tag name
    while (isNameChar(currentChar())) {
        token.tagType += input[position];
        next();
    }

    // If we reach here, we expect '>' or attributes to follow the tag name
    while (true) {
        skip(); // Skip whitespace after tag name

        // Check if the next character is '>' which indicates the end of the tag
        if (currentChar() == '>') {
            next();
            return true; // Successfully scanned a start tag
        }

        // Scan an attribute and add it to the token's attribute string list
        std::string attributeString = "";
        bool result = scanAttribute(attributeString);
        if (result) {
            token.attributeStringList.push_back(attributeString);
        } else {
            token = Token(); // Reset token
            return false; // Failed to scan a valid attribute
        }
    }
}

bool Scanner::scanContent(Token& token) {
    while (currentChar() != '<' && currentChar() != '\0') {
        // Scan text content until we reach a tag
        token.contentString += currentChar();
        next();
    }
    if (!token.contentString.empty()) {
        token.type = TokenType::TOK_CONTENT; // Set token type to content
        return true; // Successfully scanned content
    } else {
        return false; // Failed to scan content
    }
}