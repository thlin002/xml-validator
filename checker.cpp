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
        {"<People age=”1”>hello world</People>", false},                       // there is no closing tag for "People age=”1”" and no opening tag for "/People"
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