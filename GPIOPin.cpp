#include "GPIOPin.hpp"
#include <functional>
#include <wiringPi.h>

GPIOPin::GPIOPin(unsigned int number) : number(number)
{
    wiringPiSetupGpio();
    piHiPri(99);
    pullUpDnControl(number, PUD_DOWN);
    wiringPiISR(number, INT_EDGE_BOTH, &(this->interruptHandler));
}

void GPIOPin::interruptHandler() {
    if (this->edgeHandler) {
        this->edgeHandler(micros());
    }
}

void GPIOPin::setEdgeHandler(std::function<void()> edgeHandler)
{
    this->edgeHandler = edgeHandler;
}
