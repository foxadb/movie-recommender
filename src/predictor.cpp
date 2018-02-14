#include "predictor.hpp"

#include <iostream>

Predictor::Predictor(double **ratings, size_t userNb, size_t movieNb) {
    this->ratings = ratings;
    this->userNb = userNb;
    this->movieNb = movieNb;
}

Predictor::~Predictor() {
    for (int i = 0; i < this->userNb; ++i) {
        delete [] this->ratings[i];
        delete [] this->predictions[i];
    }
    delete [] this->ratings;
    delete [] this->predictions;
}

void Predictor::matrixFactorization(
        double **P, double **Q,
        size_t K, double alpha, double beta, size_t steps) {
    // Iterate throw steps
    for (size_t step = 0; step < steps; ++step) {
        for (size_t i = 0; i < this->userNb; ++i) {
            for (size_t j = 0; j < this->movieNb; ++j) {
                if (this->ratings[i][j] > 0) {
                    double dotProd = 0;
                    for (size_t k = 0; k < K; ++k) {
                        dotProd += P[i][k] * Q[k][j];
                    }
                    double eij = this->ratings[i][j] - dotProd;

                    for (size_t k = 0; k < K; ++k) {
                        P[i][k] = P[i][k] + alpha * (2 * eij * Q[k][j] - beta * P[i][k]);
                        Q[k][j] = Q[k][j] + alpha * (2 * eij * P[i][k] - beta * Q[k][j]);
                    }
                }
            }
        }
    }
}

void Predictor::predictionMatrix(size_t K, double alpha, double beta, size_t steps) {
    // Initialize P
    double **P = new double*[this->userNb];
    for (size_t i = 0; i < this->userNb; ++i) {
        P[i] = new double[K];
    }

    // Initialize Q
    double **Q = new double*[K];
    for (size_t i = 0; i < K; ++i) {
        Q[i] = new double[this->movieNb];
    }

    // Matrix factorization
    this->matrixFactorization(P, Q, K, alpha, beta, steps);

    // Initialize Predictions Matrix
    this->predictions = new double*[this->userNb];
    for (size_t i = 0; i < this->userNb; ++i) {
        this->predictions[i] = new double[this->movieNb];
    }

    // Compute predictions
    for (size_t i = 0; i < this->userNb; ++i) {
        for (size_t j = 0; j < this->movieNb; ++j) {
            double dotProd = 0;
            for (size_t k = 0; k < K; ++k) {
                dotProd += P[i][k] * Q[k][j];
            }
            this->predictions[i][j] = dotProd;
        }
    }

    // Free P memory
    for (size_t i = 0; i < this->userNb; ++i) {
        delete [] P[i];
    }
    delete [] P;

    // Free Q memory
    for (size_t i = 0; i < K; ++i) {
        delete [] Q[i];
    }
    delete [] Q;
}

double Predictor::predict(size_t user, size_t movie) {
    return this->predictions[user][movie];
}
