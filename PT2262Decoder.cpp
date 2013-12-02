#include "PT2262Decoder.hpp"
#include <iostream>

using std::chrono::microseconds;
using std::cout;
using std::endl;

void PT2262Decoder::edgeOccured(microseconds time) {
    cout << "edgeOccured(" << time.count() << ")" << endl;
}
