#include "predictor.hpp"

#include <omp.h>
#include <iostream>
#include <cmath>

Predictor::Predictor(const std::vector<Rating *>& training,
                     const std::vector<Rating *>& testing,
                     size_t userNb, size_t movieNb) {
    this->training = training;
    this->testing = testing;
    this->userNb = userNb;
    this->movieNb = movieNb;

    // Prediction matrix allocation
    this->predictions = new double*[this->userNb];
    for (size_t i = 0; i < this->userNb; ++i) {
        this->predictions[i] = new double[this->movieNb];
    }
}

Predictor::~Predictor() {
    for (int i = 0; i < this->userNb; ++i) {
        delete [] this->predictions[i];
    }
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
        double **U, double **M,
        size_t K, double eta, double lambda) {
    // Initialize MAE
    double mae = -1;
    double oldMae[3] = { 0, 0, 0 };

    // Iterate until MAE converge enough
    while (!this->convergeEnough(1e-5, mae, oldMae, 3)) {
        for (Rating* rating: this->training) {
            size_t i = rating->getUser() - 1;
            size_t j = rating->getMovie() - 1;

            double dotProd = 0;
            for (size_t k = 0; k < K; ++k) {
                dotProd += U[i][k] * M[k][j];
            }

            // ij term error
            double error = rating->getMark() - dotProd;

            // Stochastic gradient descent iteration
            for (size_t k = 0; k < K; ++k) {
                U[i][k] += eta * 2 * (error * M[k][j] - lambda * U[i][k]);
                M[k][j] += eta * 2 * (error * U[i][k] - lambda * M[k][j]);
            }
        }

        // Compute the new MAE
        mae = this->meanAbsoluteError(U, M, K);
    }
}

bool Predictor::convergeEnough(double tolerance, double mae, double *oldMae, int size) {
    // Avoid NAN
    if (mae != mae) {
        return true;
    }

    // Avoid divergence
    if (mae > oldMae[0] && oldMae[0] > 0) {
        return true;
    }

    double diff = 0;
    for (int i = 0; i < size; ++i) {
        diff += std::abs(oldMae[i] - mae);
    }

    if (diff < tolerance) {
        return true;
    } else {
        for (int i = size - 1; i > 0; --i) {
            oldMae[i] = oldMae[i-1];
        }
        oldMae[0] = mae;
        return false;
    }
}

double Predictor::meanAbsoluteError(double **U, double **M, size_t K) {
    double error = 0;

    for (Rating* rating: this->testing) {
        size_t i = rating->getUser() - 1;
        size_t j = rating->getMovie() - 1;

        double dotProd = 0;
        for (size_t k = 0; k < K; ++k) {
            dotProd += U[i][k] * M[k][j];
        }
        error += std::abs(rating->getMark() - dotProd);
    }

    // Normalization
    error /= this->testing.size();

    return error;
}

void Predictor::predictionMatrix(size_t K, double eta, double lambda) {
    // Initialize User and Movie matrix
    double **U = this->genRandomMatrix(this->userNb, K);
    double **M = this->genRandomMatrix(K, this->movieNb);

    // Matrix factorization
    this->matrixFactorization(U, M, K, eta, lambda);

    // Compute predictions
    for (size_t i = 0; i < this->userNb; ++i) {
        for (size_t j = 0; j < this->movieNb; ++j) {
            double dotProd = 0;
            for (size_t k = 0; k < K; ++k) {
                dotProd += U[i][k] * M[k][j];
            }
            this->predictions[i][j] = dotProd;
        }
    }

    // Free U memory
    for (size_t i = 0; i < this->userNb; ++i) {
        delete [] U[i];
    }
    delete [] U;

    // Free M memory
    for (size_t i = 0; i < K; ++i) {
        delete [] M[i];
    }
    delete [] M;
}

double Predictor::predict(size_t user, size_t movie) {
    return this->predictions[user][movie];
}

double Predictor::trainingMeanAbsoluteError() {
    double error = 0;

    for (Rating *rating: this->training) {
        size_t i = rating->getUser() - 1;
        size_t j = rating->getMovie() - 1;
        error += std::abs(rating->getMark() - this->predictions[i][j]);
    }

    // Normalization
    error /= this->training.size();

    return error;
}

double Predictor::testingMeanAbsoluteError() {
    double error = 0;

    for (Rating *rating: this->testing) {
        size_t i = rating->getUser() - 1;
        size_t j = rating->getMovie() - 1;
        error += std::abs(rating->getMark() - this->predictions[i][j]);
    }

    // Normalization
    error /= this->testing.size();

    return error;
}
