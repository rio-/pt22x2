#include <iostream>
#include "GPIOPin.hpp"
#include "PT2262Decoder.hpp"

int main() {
    PT2262Decoder decoder;
    GPIOPin pin(25);

    std::cout << "Receiving signals ..." << std::endl;
}
