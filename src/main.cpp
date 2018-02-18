#include "parser.hpp"
#include "rating.hpp"
#include "predictor.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Read arguments
    char *filename = argv[1];
    size_t K = atoi(argv[2]);
    double alpha = atof(argv[3]);
    double beta = atof(argv[4]);
    size_t steps = atoi(argv[5]);

    // Parser initialization
    Parser *parser = new Parser(filename, ',');

    // Read CSV file
    parser->readCsv();

    // Split train test ratings
    parser->splitTrainTestRatings(0.7);

    // Ratings matrix
    Predictor *predictor = new Predictor(
                            parser->ratingsMatrix(),
                            parser->getUserNb(),
                            parser->getMovieNb()
                            );

    // Prediction matrix
    predictor->predictionMatrix(K, alpha, beta, steps);

    // Test predictions
    for (Rating *rating: parser->getTestRatings()) {
        std::cout << "Rating: " << rating->toString()
                  << "\t real: " << rating->getMark()
                  << "\t predicted: " << predictor->predict(
                             rating->getUser() - 1, rating->getMovie() - 1)
                  << std::endl;
    }

    // Free memory
    delete parser;
    delete predictor;

    // Exit success
    return 0;
}
