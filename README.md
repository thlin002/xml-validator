# SimpleValidator
## What issue this PR is related to.
This PR adds a new feature to validate a simplified version of XML that follows the EBNF (Extended Backus-Naur Form) grammar below.
\*The following EBNF grammar uses the notation from the section <6 Notation> in [Extensible Markup Language (XML) 1.0 (Fifth Edition)](https://www.w3.org/TR/xml/).
```
[1]    document            ::=       element
[2]    S                   ::=       ('/n' | ' ' | '/t')+
[3]    NameStartChar       ::=       [A-Z] | "_" | [a-z]
[3a]   NameChar            ::=       NameStartChar | "-" | "." | [0-9]
[4]    Name                ::=       NameStartChar (NameChar)*
[5]    AttValue            ::=       '"' [^"]* '"' |  "'" [^']* "'"
[6]    CharData            ::=       [^<]*
[7]    Eq                  ::=       S? '=' S?
[8]    element             ::=       STag content ETag 
[9]    STag                ::=       '<' Name (S Attribute)* S? '>'  
[10]   Attribute           ::=       Name Eq AttValue
[11]   ETag                ::=       '</' Name (S Attribute)* S? '>'
[12]   content             ::=       CharData? (element CharData?)*
```
\*Additional to the above EBNF grammar, the `Name`s and `Attribute`s in `STag` and `ETag` must match.

#### Simplified XML examples:
An opening tag and a closing tag should have matching names and attributes.
```xml
<tutorial date="01/01/2000">XML</tutorial date="01/01/2000">
```
Tag attributes value can be enclosed in single or double quotes.
```xml
<Design color='red' shape="round">Modern<Code>hello world</Code></Design color='red' shape="round">
```
There can be arbitrary whitespaces between the tag name and the attributes, as well as between the attributes themselves and their values.
```xml
<Design    color   = "green" shape  =   "oval">Test</Design    color =  "green"   shape =  "oval">
```
XML can contain '\n', ' ', and '\t' characters between tags and their content.
```xml
<Design>
    <Code>
        hello world
    </Code>
</Design>
```

#### Usages (how to run the validator):
```bash
# usage: ./validator <xml_string>
# for example:
./validator "<tutorial date=\"01/01/2000\">XML</tutorial date=\"01/01/2000\">"
# output: Valid
./validator "<People><Design><Code>hello world</People></Code></Design>"
# output: Invalid
```

## What change this PR adds.
This change adds a scanner that identifies the XML tokens--`STag`, `ETag`, `Attribute`, and `CharData`--based on the provided EBNF grammar. It also includes a parser(validator) that validates the structure of the XML document using the tokens returned by the scanner. The scanner is implemented as a state machine that processes the input string character by character, recognizing the tokens. The parser(validator) uses a stack-based approach to ensure that the XML tags are correctly nested and matched.

The scanner is the class `Scanner` defined in `scanner.hpp` and `scanner.cpp`. The parser(validator) is the `DetermineXml` function defined in `simple_validator.hpp` and `simple_validator.cpp`.

`defs.hpp` includes the definitions of the `Token` struct and the `TokenType` enum, which are used in the scanner and parser(validator) to represent the tokens identified in the XML string.

`checker.cpp` has been updated to include tests that cover the specified BNF grammar.

## How I tested this change.
This change has been tested by running the `checker.cpp` program, which includes test cases that cover various valid and invalid XML strings based on the provided EBNF grammar.
```bash
./checker # This will run the tests defined in checker.cpp
```

## Anything I want reviewers to scrutinize.
Please review the system structure of the `SimpleValidator` project to ensure future maintainability and extensibility, especially the organization of the scanner and parser(validator) components.

## Other information
The parser(validator) logic is essentially a pushdown automata (PDA), which is capable of recognizing context-free languages.