#include "parser.hpp"
#include "rating.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Parser initialization
    Parser *parser = new Parser(argv[1], ',');

    // Read CSV file
    parser->readCsv();

    // User and Movie numbers
    size_t userNb = parser->getUserNb();
    size_t movieNb = parser->getMovieNb();

    // Print CSV
    std::cout << parser->toString() << std::endl;

    // Ratings matrix
    double** ratings = parser->ratingsMatrix();

    for (size_t i = 0; i < userNb; ++i) {
        for (size_t j = 0; j < movieNb; ++j) {
            std::cout << std::to_string(ratings[i][j]) + " ";
        }
        std::cout << std::endl;
    }

    // Free memory
    delete parser;

    for (int i = 0; i < userNb; ++i) {
        delete [] ratings[i];
    }
    delete [] ratings;

    // Exit success
    return 0;
}
