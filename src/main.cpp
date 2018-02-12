#include "parser.hpp"
#include "rating.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    // Parser initialization
    Parser *parser = new Parser(argv[1], ',');

    // Read CSV file
    parser->readCsv();

    std::cout << parser->toString() << std::endl;

    // Free memory
    delete parser;

    // Exit success
    return 0;
}
