#ifndef PT2262DECODER_HPP
#define PT2262DECODER_HPP
#include<chrono>

class PT2262Decoder {
    public:
        void edgeOccured(std::chrono::microseconds time);
};

#endif // PT2262DECODER_HPP
