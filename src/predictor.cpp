#include "predictor.hpp"

#include <iostream>
#include <cmath>

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

double** Predictor::genRandomMatrix(size_t n, size_t m) {
    // Matrix allocation
    double **mat = new double*[n];
    for (size_t i = 0; i < n; ++i) {
        mat[i] = new double[m];
    }

    // Fill matrix with random values
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            mat[i][j] = (double)rand() / RAND_MAX;
        }
    }

    // Return the matrix
    return mat;
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
                        P[i][k] += alpha * (2 * eij * Q[k][j] - beta * P[i][k]);
                        Q[k][j] += alpha * (2 * eij * P[i][k] - beta * Q[k][j]);
                    }
                }
            }
        }
        // Compute convergence error
        /*double error = this->convergenceError(P, Q, K, beta);
        if (error < 0.01) {
            break;
        }*/
    }

    // Display the convergence error
    std::cout << "Convergence error: "
              << this->convergenceError(P, Q, K, beta)
              << std::endl;
}

double Predictor::convergenceError(double **P, double **Q, size_t K, double beta) {
    // Error initialization
    double error = 0;

    for (size_t i = 0; i < this->userNb; ++i) {
        for (size_t j = 0; j < this->movieNb; ++j) {
            if (this->ratings[i][j]) {
                // error += |R - PQ|^2
                double dotProd = 0;
                for (size_t k = 0; k < K; ++k) {
                    dotProd += P[i][k] * Q[k][j];
                }
                error += std::pow(this->ratings[i][j] - dotProd, 2);

                // error += ||P||^2 + ||Q||^2
                for (size_t k = 0; k < K; ++k) {
                    error += beta / 2 * (std::pow(P[i][k], 2) + std::pow(Q[k][j], 2));
                }
            }
        }
    }

    // Return error value
    return error;
}

void Predictor::predictionMatrix(size_t K, double alpha, double beta, size_t steps) {
    // Initialize P and Q
    double **P = this->genRandomMatrix(this->userNb, K);
    double **Q = this->genRandomMatrix(K, this->movieNb);

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
