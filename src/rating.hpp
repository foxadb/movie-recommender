#ifndef RATING_HPP
#define RATING_HPP

#include <string>

class Rating {

public:
    Rating();
    Rating(size_t user, size_t movie, double mark);
    ~Rating();

    size_t getUser();
    size_t getMovie();
    double getMark();

    void setUser(size_t user);
    void setMovie(size_t movie);
    void setMark(double mark);

    std::string toString();

private:
    size_t user;
    size_t movie;
    double mark;

};

#endif
