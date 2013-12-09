#include <iostream>
#include <chrono>
#include <exception>
#include "GPIOPin.hpp"
#include "PT22x2.hpp"
#include "Command.hpp"

using std::chrono::microseconds;
using std::cout;
using std::cin;
using std::endl;
using std::runtime_error;

void codewordHandler(PT22x2::Codeword codeword) {
    try {
        Command cmd(codeword);
        cout << cmd << endl;
        cout << codeword << endl;
    }
    catch (runtime_error const &e) {
        // malformed codeword, just ignore it
    }
}

int main() {
    PT22x2::Decoder decoder(&codewordHandler);

    GPIOPin pin(25);
    pin.setPullDown();
    pin.setEdgeHandler([&decoder] (microseconds time) {
            decoder.edgeOccured(time);
    });

    cout << "Receiving signals ..." << endl
         << "(press return to stop)" << endl;
    cin.get();
    return 0;
}
