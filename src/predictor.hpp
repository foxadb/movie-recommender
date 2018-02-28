#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <iostream>

class Predictor {

public:
    Predictor(double** ratings, size_t ratingNb, size_t userNb, size_t movieNb);
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

private:
    double** ratings;
    double** predictions;

    size_t ratingNb;
    size_t userNb;
    size_t movieNb;
};

#endif
