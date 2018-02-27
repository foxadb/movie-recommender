#include "predictor.hpp"

#include <omp.h>
#include <iostream>
#include <cmath>

Predictor::Predictor(double **ratings, size_t ratingNb, size_t userNb, size_t movieNb) {
    this->ratings = ratings;
    this->ratingNb = ratingNb;
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
            mat[i][j] = (double)rand() / RAND_MAX * 5;
        }
    }

    // Return the matrix
    return mat;
}

void Predictor::matrixFactorization(
        double **U, double **V,
        size_t K, double eta, double lambda) {
    // Initialize MAE
    double mae = 1;
    double newMae = 0;

    // Iterate until MAE converge enough
    while (std::abs(mae - newMae) > 1e-5) {
        mae = newMae;
        for (size_t i = 0; i < this->userNb; ++i) {
            for (size_t j = 0; j < this->movieNb; ++j) {
                if (this->ratings[i][j] > 0) {
                    double dotProd = 0;
                    for (size_t k = 0; k < K; ++k) {
                        dotProd += U[i][k] * V[k][j];
                    }
                    double eij = this->ratings[i][j] - dotProd;

                    for (size_t k = 0; k < K; ++k) {
                        U[i][k] += eta * 2 * (eij * V[k][j] - lambda * U[i][k]);
                        V[k][j] += eta * 2 * (eij * U[i][k] - lambda * V[k][j]);
                    }
                }
            }
        }

        // Compute the new MAE
        newMae = this->meanAbsoluteError(U, V, K);
    }

    std::cout << "Training MAE = " << newMae << std::endl;
}

double Predictor::meanAbsoluteError(double **U, double **V, size_t K) {
    double error = 0;

    for (size_t i = 0; i < this->userNb; ++i) {
        for (size_t j = 0; j < this->movieNb; ++j) {
            if (this->ratings[i][j]) {
                double dotProd = 0;
                for (size_t k = 0; k < K; ++k) {
                    dotProd += U[i][k] * V[k][j];
                }
                error += std::abs(this->ratings[i][j] - dotProd);
            }
        }
    }

    // Normalization
    error /= this->ratingNb;

    return error;
}

void Predictor::predictionMatrix(size_t K, double eta, double lambda) {
    // Initialize P and Q
    double **U = this->genRandomMatrix(this->userNb, K);
    double **V = this->genRandomMatrix(K, this->movieNb);

    // Matrix factorization
    this->matrixFactorization(U, V, K, eta, lambda);

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
                dotProd += U[i][k] * V[k][j];
            }
            this->predictions[i][j] = dotProd;
        }
    }

    // Free P memory
    for (size_t i = 0; i < this->userNb; ++i) {
        delete [] U[i];
    }
    delete [] U;

    // Free Q memory
    for (size_t i = 0; i < K; ++i) {
        delete [] V[i];
    }
    delete [] V;
}

double Predictor::predict(size_t user, size_t movie) {
    return this->predictions[user][movie];
}
