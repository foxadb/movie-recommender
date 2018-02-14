#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <iostream>

class Predictor {

public:
    Predictor(double** ratings, size_t userNb, size_t movieNb);
    ~Predictor();

    void matrixFactorization(
            double **P, double **Q,
            size_t K, double alpha, double beta, size_t steps);

    void predictionMatrix(size_t K, double alpha, double beta, size_t steps);

    double predict(size_t user, size_t movie);

private:
    double** ratings;
    double** predictions;
    size_t userNb;
    size_t movieNb;
};

#endif
