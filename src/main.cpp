#include "parser.hpp"
#include "rating.hpp"
#include "predictor.hpp"

#include <iostream>
#include <ctime>

int main(int argc, char *argv[]) {
    char *filename = argv[1];
    size_t K = std::atoi(argv[2]);
    double eta;
    double lambda;

    // Read arguments
    if (argc > 3) {
        eta = std::atof(argv[3]);
        lambda = std::atof(argv[4]);
    }

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

    // Fully specified argument prediction matrix
    if (argc > 3) {
        std::cout << "====== Matrix factorization ======" << std::endl;
        std::cout << "K = " << K
                  << ", eta = " << eta
                  << ", lambda = " << lambda
                  << std::endl;
        std::clock_t start = clock();
        predictor->predictionMatrix(K, eta, lambda);
        double duration = (double)(clock() - start) / CLOCKS_PER_SEC;

        // Test predictions
        std::cout << "======== Test predictions ========" << std::endl;
        parser->writeResultsFile("results.txt", predictor);

        // Factorization duration
        std::cout << "==================================" << std::endl;
        std::cout << "Factorization duration: " << duration << " s" << std::endl;

    // Prediction matrix Cross-Validation
    } else {
        double etaArr[] = { 1e-4, 1e-3, 1e-2 };
        double lambdaArr[] = { 1e-3, 1e-2, 1e-1, 1, 1e1, 1e2 };

        // Mean of MAE
        double mae = 0;

        std::cout << "======== Matrix factorizations ========" << std::endl;
        for (double eta: etaArr) {
            for (double lambda: lambdaArr) {
                for (int l = 0; l < 5; ++l) {
                    // Compute prediction matrix
                    predictor->predictionMatrix(K, eta, lambda);

                    // Compute MAE
                    mae += parser->meanAbsoluteError(predictor);
                }

                // Normalize MAE
                mae /= 5;
                std::cout << "K = " << K
                          << ", eta = " << eta
                          << ", lambda = " << lambda
                          << ", MAE = " << mae
                          << std::endl;
            }
        }
    }

    // Free memory
    delete parser;
    delete predictor;

    // Exit success
    return 0;
}
