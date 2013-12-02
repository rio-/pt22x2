#ifndef PT226XDECODER_HPP
#define PT226XDECODER_HPP
#include<chrono>
#include<array>
#include<functional>

using std::chrono::microseconds;
using std::array;
using std::function;

class PT22x2Decoder {
    public:
        static constexpr microseconds pulsewidth_short = microseconds(322);
        static constexpr microseconds pulsewidth_long  = microseconds(967);
        static constexpr microseconds pulsewidth_sync  = microseconds(9995);
        static constexpr microseconds pulsewidth_tolerance = microseconds(200);
        static constexpr unsigned int codeword_length = 12;
        enum class Pulse { Short, Long };
        enum class CodeBit { Zero, One, Floating };
        typedef array<Pulse, 4*codeword_length> CodewordPulses;
        typedef array<CodeBit, codeword_length> Codeword;

    private:
        microseconds lasttime;
        bool receiving;
        unsigned int position;
        CodewordPulses pulses;
        function<void(Codeword)> codewordHandler;
        
    public:
        PT22x2Decoder(function<void(Codeword)> codewordHandler);
        void edgeOccured(microseconds time);
        Codeword codewordFromPulses(CodewordPulses pulses);
};


#endif // PT226XDECODER_HPP
