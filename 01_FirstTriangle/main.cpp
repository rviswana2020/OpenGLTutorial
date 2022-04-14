#include "FirstTriangle.h"

#include <iostream>        // console printing
#include <stdexcept>       // exception handling
#include <cstdlib>         // EXIT macro definition

int main() {
    FirstTriangle app;

    try {
        app.run();
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
