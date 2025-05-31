#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include "simple_validator.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // You can use here to test, feel free to modify/add the test cases here.
    // You can also use other ways to test if you want.
    vector<tuple<string, bool>> test_cases = {
        {"<Design><Code>hello world</Code></Design>", true},                   // normal case
        {"<Design><Code>hello world</Code></Design><People>", false},          // no closing tag for "People"
        {"<People><Design><Code>hello world</People></Code></Design>", false}, // "/Code" should come before "/People"
        {"<People age=\"1\">hello world</People>", false},                       // there is no closing tag for "People age="1"" and no opening tag for "/People"
        {"<tutorial date=\"01/01/2000\">XML</tutorial date=\"01/01/2000\">", true}, // opening and closing tags with attributes
        {"<tutorial date=\"01/01/2000\">XML</tutorial date=\"01/01/20\">", false}, // invalid XML with mismatched attribute values
        {"<Design><Code>hello world</Code></Design><People></People>", false},  // An element is outside the root element.
        {"test<Design><Code>hello world</Code></Design>", false},               // invalid XML with text before opening tag
        {"<Design><Code>hello world</Code></Design>test", false},               // invalid XML with text after closing tag
        {"<Design><Code>hello world</Code>modern</Design>", true}, // valid XML with content and an element in the tag
        {"<Design>modern<Code>hello world</Code></Design>", true}, // valid XML with content and an element in the tag
        {"<Design>modern<Code>hello world</Code><People age=\"1\">Sean</People age=\"1\"></Design>", true}, // valid XML with content and elements in the tag
        {"<Design>modern<Code>hello world</Code><People age=\"1\">Sean</People age=\"1\"></Design><date><plan></plan></date>", false}, // Invalid XML with elements outside the root element.
        {"<Design color=\"red\" shape=\"round\"><Code>hello world</Code></Design color=\"red\" shape=\"round\">", true}, // valid XML with multiple attributes
        {"<Design color=\"red\" shape=\"round\"><Code>hello world</Code></Design color=\"red\" >", false}, // invalid XML with mismatched attributes between opening and closing tags
        {"<Design color=\"\'red\'\" shape=\'\"round\"\'><Code>hello world</Code></Design color=\"\'red\'\" shape=\'\"round\"\'>", true}, // valid XML with attributes having single quotes and double quotes
        {"<Design color='red' shape=\"round\">Modern<Code>hello world</Code></Design color='red' shape=\"round\">", true}, // valid XML with attributes having single quotes and double quotes
        {"<Design color=\'red\"><Code>hello world</Code></Design color=\'red\">", false}, // invalid XML with mismatched quotes in attributes
        {"<Design color=  \"red\"     shape=\"round\"><Code>hello world</Code></Design color    =\"red\" shape=\"round\"    >", true}, // valid XML with attributes having extra spaces between the equal sign and the value
        {"<Design _Main-Color=\"red\" Shape_1.0=\"round\"><Code>hello world</Code></Design _Main-Color=\"red\" Shape_1.0=\"round\">", true}, // valid XML with attributes containing certain allowed special characters
        {"<_Design_1.0-2 color=\"red\" shape=\"round\">Apple</_Design_1.0-2 color=\"red\" shape=\"round\">", true}, // valid XML with tag names containing certain allowed special characters
        {"<Design><Code>hello world</Code>modern</Design>               ", true}, // valid XML with trailing spaces
        {"             <Design><Code>hello world</Code>modern</Design>", true}, // valid XML with leading spaces
        {"<Design attr=test><Code>hello world</Code></Design attr=test>", false}, // invalid XML with attributes not quoted
        {"<tutorial date=\"01/01/2000>XML</tutorial date=\"01/01/2000>", false}, // invalid XML with missing closing quote for attribute
        {"<tutorial date=01/01/2000\">XML</tutorial date=01/01/2000\">", false}, // invalid XML with missing opening quote for attributes
        {"<Design>\n \
            <Code>\n \
                hello world\n \
            </Code>\n \
          </Design>", true}, // valid XML with newlines and indentation
        {"<tutorial date=\"<01/01/2000>\">XML</tutorial date=\"<01/01/2000>\">", true}, // valid XML with angle brackets in attribute values (This is valid only for our simplified XML bnf grammar, not for standard XML)
        {"</Code>test<Code>", false},  // invalid XML with end tag before start tag
        {"<Design>modern>&$<Code>hello world</Code>sleek>^%>cool</Design>", true}, // valid XML with special characters in content
    };

    int failed_count = 0;
    for (auto &test_case : test_cases)
    {
        auto &input = get<0>(test_case);
        auto &expected = get<1>(test_case);

        auto result = DetermineXml(input);
        auto result_str = result ? "Valid" : "Invalid";

        if (result == expected)
        {
            cout << "OK ";
        }
        else
        {
            cout << "NG ";
            failed_count++;
        }
        cout << input << ": " << result_str << endl;
    }

    cout << "Result: " << (test_cases.size() - failed_count) << "/" << test_cases.size() << endl;

    return 0;
}