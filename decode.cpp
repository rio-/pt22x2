#include <iostream>
#include <chrono>
#include "GPIOPin.hpp"
#include "PT2262Decoder.hpp"

using std::chrono::microseconds;
using std::cout;
using std::endl;

int main() {
    PT2262Decoder decoder;
    GPIOPin pin(25);
    
    pin.setEdgeHandler([&decoder] (microseconds time) { decoder.edgeOccured(time); });

    cout << "Receiving signals ..." << endl
         << "(press return to stop)" << endl;
    std::cin.get();
    return 0;
}
