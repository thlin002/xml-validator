#ifndef DEFS_H
#define DEFS_H

#include <string>
#include <vector>

enum TokenType {
    TOK_EOF,
    TOK_START_TAG,
    TOK_END_TAG,
    TOK_CONTENT,
};

struct Token {
    TokenType type;
    std::string tagType;
    std::vector<std::string> attributeStringList; // List of attributes in the tag
                                                  // Attributes are stored as strings in the format: key="value"
    std::string contentString; // Content between tags, if applicable
    Token() : type(TokenType::TOK_EOF), tagType(""), attributeStringList(std::vector<std::string>()), contentString("") {}
    Token(TokenType type, const std::string& tagType, const std::vector<std::string>& attr, const std::string& content = "")
        : type(type), tagType(tagType), attributeStringList(attr), contentString(content) {}
};

#endif