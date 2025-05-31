#include "simple_validator.hpp"
#include "scanner.hpp"
#include <stack>

bool DetermineXml(const std::string &input)
{
    std::stack<Token> tagStack; // Stack to keep track of open tags
    // Check if the input is empty. Empty input is not valid XML
    if (input.empty()) {
        return false;
    }
    Scanner scanner = Scanner(input);
    bool firstToken = true; // Flag to check if we are processing the first token
    while(true) {
        Token token = Token();
        // Scan the next token from the input
        if (!scanner.scan(token)) {
            break; // Error found in XML structure
        }

        // Check if we have a element or content outside the root element
        if (firstToken == false && tagStack.empty() && token.type != TokenType::TOK_EOF) {
            return false;
        }

        // Process the scanned token
        if (token.type == TokenType::TOK_START_TAG) {
            // Push start tag onto the stack
            tagStack.push(token);
        } else if (token.type == TokenType::TOK_END_TAG) {
            // Check if the end tag matches the last start tag
            if (tagStack.empty() || (tagStack.top().tagType != token.tagType || tagStack.top().attributeStringList != token.attributeStringList)) {
                return false; // Mismatched end tag
            }
            tagStack.pop(); // Pop the matching start tag from the stack
        } else if (token.type == TokenType::TOK_CONTENT) {
            // Content is valid, do nothing
        } else if (token.type == TokenType::TOK_EOF) { // EOF Token indicates end of input
            // If we reach EOF, check if all tags are closed, i.e., the stack should be empty
            return tagStack.empty();
        }

        if (firstToken) {
            firstToken = false; // Set the flag to false after processing the first token
        }
    }
    return false;
}