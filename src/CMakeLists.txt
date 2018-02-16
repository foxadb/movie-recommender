file(GLOB LIB_HEADERS *.hpp)
file(GLOB LIB_SOURCES *.cpp)
add_library(movie-recommender ${LIB_HEADERS} ${LIB_SOURCES})

add_executable(recommender main.cpp)
target_link_libraries(recommender movie-recommender)
