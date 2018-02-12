#include "parser.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

Parser::Parser(char *filename, char delimiter) {
    this->filename = filename;
    this->delimiter = delimiter;
}

Parser::~Parser() {
    this->ratings.clear();
}

std::string Parser::toString() {
    std::string result;
    for(Rating* rating: this->ratings) {
        result += rating->toString();
        result += "\n";
    }
    return result;
}

void Parser::readCsv() {
    std::ifstream file(this->filename);
    std::string line;

    // Skip first line
    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;

        Rating *rating = new Rating();

        // User ID (start from 0)
        std::getline(ss, value, this->delimiter);
        rating->setUser(atoi(value.c_str()) - 1);

        // Movie ID (start from 0)
        std::getline(ss, value, this->delimiter);
        rating->setMovie(atoi(value.c_str()) - 1);

        // Mark
        std::getline(ss, value, this->delimiter);
        rating->setMark(atof(value.c_str()));

        // Push rating to data
        this->ratings.push_back(rating);
    }

    // Close the input file
    file.close();
}
