#include "parser.hpp"
#include "rating.hpp"
#include "predictor.hpp"

#include <iostream>
#include <ctime>

int main(int argc, char *argv[]) {
    // Read arguments
    char *filename = argv[1];
    size_t K = std::atoi(argv[2]);
    double eta = std::atof(argv[3]);
    double lambda = std::atof(argv[4]);
    size_t steps = std::atoi(argv[5]);

    // Parser initialization
    Parser *parser = new Parser(filename, ',');

    // Read CSV file
    std::cout << "Parsing data file: " << filename << std::endl;
    parser->readCsv();

    // Split train test ratings
    std::cout << "Split training/testing datasets" << std::endl;
    parser->splitTrainTestRatings(0.8);

    // Ratings matrix
    Predictor *predictor = new Predictor(
                            parser->ratingsMatrix(),
                            parser->getTrainRatingsNb(),
                            parser->getUserNb(),
                            parser->getMovieNb()
                            );

    // Prediction matrix
    std::cout << "====== Matrix factorization ======" << std::endl;
    std::cout << "K = " << K
              << ", eta = " << eta
              << ", lambda = " << lambda
              << std::endl;
    std::clock_t start = clock();
    predictor->predictionMatrix(K, eta, lambda, steps);
    double duration = (double)(clock() - start) / CLOCKS_PER_SEC;

    // Test predictions
    std::cout << "======== Test predictions ========" << std::endl;
    parser->writeResultsFile("results.txt", predictor);

    // Factorization duration
    std::cout << "==================================" << std::endl;
    std::cout << "Factorization duration: " << duration << " s" << std::endl;

    // Free memory
    delete parser;
    delete predictor;

    // Exit success
    return 0;
}
