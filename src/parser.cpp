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
    for (Rating *rating: this->ratings) {
        delete rating;
    }
    this->trainRatings.clear();

    this->ratings.clear();
    this->trainRatings.clear();
    this->testRatings.clear();
}

size_t Parser::getUserNb() {
    return this->userNb;
}

size_t Parser::getMovieNb() {
    return this->movieNb;
}

std::vector<Rating*> Parser::getRatings() {
    return this->ratings;
}

std::vector<Rating*> Parser::getTrainRatings() {
    return this->trainRatings;
}

std::vector<Rating*> Parser::getTestRatings() {
    return this->testRatings;
}

std::string Parser::toString() {
    std::string result;

    // Ratings to strings
    for (Rating* rating: this->ratings) {
        result += rating->toString();
        result += "\n";
    }

    // User and movie counters
    result += "Users: " + std::to_string(this->userNb)
            + "\nMovies: " + std::to_string(this->movieNb);

    // Return result string
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

    while (std::getline(file, line)) {
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
            ++this->userNb;
        }

        // Compute max movie id
        if (this->movieNb < rating->getMovie()) {
            ++this->movieNb;
        }

        // Push rating to data
        this->ratings.push_back(rating);
    }

    // Close the input file
    file.close();
}

void Parser::splitTrainTestRatings(double ratio) {
    // Count ratings per user
    size_t *userRatingsNb = new size_t[this->userNb]();
    for (Rating *rating: this->ratings) {
        ++userRatingsNb[rating->getUser() - 1];
    }

    // Clear old rating vectors
    this->trainRatings.clear();
    this->testRatings.clear();

    // Current user id
    size_t currentUser = 1;

    // Rating per user counter
    size_t ratingCounter = 0;

    for (Rating *rating: this->ratings) {
        // Next user id
        if (currentUser < rating->getUser()) {
            ++currentUser;
            ratingCounter = 0;
        }

        // Split between training and testing datasets
        if (ratingCounter < userRatingsNb[currentUser - 1] * ratio) {
            this->trainRatings.push_back(rating);
        } else {
            this->testRatings.push_back(rating);
        }

        // Increase rating per user counter
        ++ratingCounter;
    }

    // Free memory
    delete userRatingsNb;
}

double** Parser::ratingsMatrix() {
    double **ratings = new double*[this->userNb];
    for (size_t i = 0; i < this->userNb; ++i) {
        ratings[i] = new double[this->movieNb]();
    }

    // Populate the matrix with ratings
    for (Rating *r: this->trainRatings) {
        ratings[r->getUser() - 1][r->getMovie() - 1] = r->getMark();
    }

    return ratings;
}
