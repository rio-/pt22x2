#include "GPIOPin.hpp"
#include <functional>
#include <chrono>
#include <cassert>
#include <wiringPi.h>

using std::function;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::steady_clock;

GPIOPin::GPIOPin(unsigned int number) : number(number)
{
    wiringPiSetupGpio();
}

GPIOPin *GPIOPin::instance = nullptr;

void GPIOPin::setPullDown() {
    pullUpDnControl(number, PUD_DOWN);
}

void GPIOPin::setOutput() {
    pinMode(number, OUTPUT);
}

void GPIOPin::setHigh() {
    digitalWrite(number, HIGH);
}

void GPIOPin::setLow() {
    digitalWrite(number, LOW);
}

void GPIOPin::ISR() {
    if (instance) {
        instance->interruptHandler();
    }
}

void GPIOPin::interruptHandler() {
    if (this->edgeHandler) {
        this->edgeHandler(duration_cast<microseconds>(steady_clock::now().time_since_epoch()));
    }
}

void GPIOPin::setEdgeHandler(function<void(microseconds)> edgeHandler)
{
    assert(instance == nullptr);
    instance = this;
    this->edgeHandler = edgeHandler;
    wiringPiISR(number, INT_EDGE_BOTH, &GPIOPin::ISR);
}
