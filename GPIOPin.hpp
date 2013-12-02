#ifndef GPIOPIN_HPP
#define GPIOPIN_HPP
#include <functional>

class GPIOPin {
    private:
        std::function<void()> edgeHandler;
        unsigned int number;

        void interruptHandler();
    public:
        GPIOPin(unsigned int number);
        void setEdgeHandler(std::function<void()> edgeHandler);
};

#endif // GPIOPIN_HPP
