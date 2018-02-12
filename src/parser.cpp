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
    result += "Users: " + std::to_string(this->userNb)
            + "\nMovies: " + std::to_string(this->movieNb);
    return result;
}

void Parser::readCsv() {
    // Open CSV file
    std::ifstream file(this->filename);
    std::string line;

    // Initialize user and movie counters
    this->userNb = 0;
    this->movieNb = 0;

    // Skip first line (header)
    std::getline(file, line);

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;

        Rating *rating = new Rating();

        // User id
        std::getline(ss, value, this->delimiter);
        rating->setUser(atoi(value.c_str()));

        // Movie id
        std::getline(ss, value, this->delimiter);
        rating->setMovie(atoi(value.c_str()));

        // Mark
        std::getline(ss, value, this->delimiter);
        rating->setMark(atof(value.c_str()));

        // Compute max user id
        if (this->userNb < rating->getUser()) {
            this->userNb++;
        }

        // Compute max movie id
        if (this->movieNb < rating->getMovie()) {
            this->movieNb++;
        }

        // Push rating to data
        this->ratings.push_back(rating);
    }

    // Close the input file
    file.close();
}
