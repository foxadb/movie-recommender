#ifndef PARSER_HPP
#define PARSER_HPP

#include "rating.hpp"

#include <string>
#include <vector>

class Parser {

public:
    Parser(char *filename, char delimiter);
    ~Parser();

    std::string toString();
    void readCsv();

private:
    char *filename;
    char delimiter;
    std::vector<Rating*> ratings;
};

#endif
