#ifndef GPIOPIN_HPP
#define GPIOPIN_HPP
#include <functional>
#include <chrono>

using std::function;
using std::chrono::microseconds;

class GPIOPin {
    private:
        function<void(microseconds)> edgeHandler;
        unsigned int number;
        static GPIOPin *instance;
        static void ISR();        
        void interruptHandler();

    public:
        GPIOPin(unsigned int number);
        void setEdgeHandler(function<void(microseconds)> edgeHandler);
        void setPullDown();
        void setOutput();
        void setHigh();
        void setLow();
};

#endif // GPIOPIN_HPP
