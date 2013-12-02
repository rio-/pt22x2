#ifndef GPIOPIN_HPP
#define GPIOPIN_HPP
#include <functional>
#include <chrono>

class GPIOPin {
    private:
        std::function<void(std::chrono::microseconds)> edgeHandler;
        unsigned int number;
        static GPIOPin *instance;
        static void ISR();        
        void interruptHandler();

    public:
        GPIOPin(unsigned int number);
        void setEdgeHandler(std::function<void(std::chrono::microseconds)> edgeHandler);
};

#endif // GPIOPIN_HPP
