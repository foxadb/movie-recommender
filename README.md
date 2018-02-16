# movie-recommender
Movie recommendation engine written in C++ using the Netflix matrix factorization technique

## Documentation

[Matrix factorization techniques for recommender systems](https://datajobs.com/data-science-repo/Recommender-Systems-%5BNetflix%5D.pdf)

[MovieLens Datasets](https://grouplens.org/datasets/movielens)

## Instructions

A rating dataset from MovieLens containing 100004 records is imported in the project.
It is located in ``data/ratings.csv``

### Compilation

```
mkdir build
cd build
cmake ..
make
```

### Run

```
./src/recommender ../data/ratings.csv 2 0.0002 0.02 5000
```
