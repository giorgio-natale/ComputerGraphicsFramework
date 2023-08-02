//
// Created by drgio on 02/08/2023.
//



#include "demo/SimpleCube.h"

int main() {
    SimpleCube app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
