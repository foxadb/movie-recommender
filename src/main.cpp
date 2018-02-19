#include "parser.hpp"
#include "rating.hpp"
#include "predictor.hpp"

#include <iostream>
#include <ctime>

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
    std::cout << "Parsing data file: " << filename << std::endl;
    parser->readCsv();

    // Split train test ratings
    std::cout << "Split training/testing datasets" << std::endl;
    parser->splitTrainTestRatings(0.7);

    // Ratings matrix
    Predictor *predictor = new Predictor(
                            parser->ratingsMatrix(),
                            parser->getUserNb(),
                            parser->getMovieNb()
                            );

    // Prediction matrix
    std::cout << "Matrix factorization" << std::endl;

    std::clock_t start = clock();
    predictor->predictionMatrix(K, alpha, beta, steps);
    double duration = (double)(clock() - start) / CLOCKS_PER_SEC;

    // Test predictions
    std::cout << "===== Test predictions =====" << std::endl;
    size_t totalResults = 0;
    size_t goodResults = 0;
    double disparity = 0;
    for (Rating *rating: parser->getTestRatings()) {
        // Compute prediction
        double expected = rating->getMark();
        double predicted = predictor->predict(
                    rating->getUser() - 1, rating->getMovie() - 1);

        // Increase counters
        ++totalResults;
        if (std::abs(predicted - expected) < 0.25) {
            ++goodResults;
        }

        // Prediction disparity
        disparity += std::abs(predicted - expected);

        // Print rating result
        std::cout << "Rating: " << rating->toString()
                  << "\t expected: " << expected
                  << "\t predicted: " << predicted
                  << std::endl;
    }

    // Average prediction disparity
    disparity /= totalResults;

    // Print good results ratio
    std::cout << "Total results: " << totalResults << std::endl
              << "Good results (+- 0.25): " << goodResults
              << " (" << (double)(goodResults) / totalResults * 100 << " %)" << std::endl
              << "Average disparity: " << disparity
              << std::endl;

    std::cout << "============================" << std::endl;
    std::cout << "Factorization duration: " << duration << " s" << std::endl;

    // Free memory
    delete parser;
    delete predictor;

    // Exit success
    return 0;
}
