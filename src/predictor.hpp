#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include "rating.hpp"

#include <iostream>
#include <vector>

class Predictor {

public:
    Predictor(const std::vector<Rating*>& training,
              const std::vector<Rating*>& testing,
              size_t userNb, size_t movieNb);
    ~Predictor();

    double** genRandomMatrix(size_t n, size_t m);

    void matrixFactorization(
            double **U, double **V,
            size_t K, double eta, double lambda);

    double meanAbsoluteError(double **U, double **V, size_t K);

    bool convergeEnough(double tolerance, double mae, double *oldMae, int size);

    void predictionMatrix(size_t K, double eta, double lambda);

    double predict(size_t user, size_t movie);

    double trainingMeanAbsoluteError();

    double testingMeanAbsoluteError();

private:
    double** ratings;
    double** predictions;

    size_t ratingNb;
    size_t userNb;
    size_t movieNb;

    std::vector<Rating*> training;
    std::vector<Rating*> testing;
};

#endif
