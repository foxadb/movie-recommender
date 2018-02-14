#ifndef PARSER_HPP
#define PARSER_HPP

#include "rating.hpp"

#include <string>
#include <vector>

class Parser {

public:
    Parser(char *filename, char delimiter);
    ~Parser();

    size_t getUserNb();
    size_t getMovieNb();
    std::vector<Rating*> getRatings();
    std::string toString();

    void readCsv();
    double** ratingsMatrix();

private:
    char *filename;
    char delimiter;

    std::vector<Rating*> ratings;
    size_t userNb;
    size_t movieNb;
};

#endif
