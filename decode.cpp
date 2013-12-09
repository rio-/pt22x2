#include <iostream>
#include <chrono>
#include <exception>
#include "GPIOPin.hpp"
#include "PT22x2Decoder.hpp"
#include "Command.hpp"

using std::chrono::microseconds;
using std::cout;
using std::cin;
using std::endl;
using std::runtime_error;

void codewordHandler(PT22x2Decoder::Codeword codeword) {
    try {
        Command cmd(codeword);
        cout << cmd << endl;
    }
    catch (runtime_error const &e) {
    }
}

int main() {
    PT22x2Decoder decoder(&codewordHandler);
    GPIOPin pin(25);
    
    pin.setEdgeHandler([&decoder] (microseconds time) {
            decoder.edgeOccured(time);
    });

    cout << "Receiving signals ..." << endl
         << "(press return to stop)" << endl;
    cin.get();
    return 0;
}
