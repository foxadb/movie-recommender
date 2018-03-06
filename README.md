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

#### Command line

```
./src/recommender csvfile k eta lambda [iteration number]
```

#### Example

Run the recommender on the ratings dataset:
```
./src/recommender ../data/ratings.csv 2 0.001 0.01
```

For fast test use ``small.csv`` file:
```
./src/recommender ../data/small.csv 2 0.04 0.0001 100000
```

Results are written into the ``results.txt`` text file.
