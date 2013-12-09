#ifndef PT226X_HPP
#define PT226X_HPP
#include <chrono>
#include <array>
#include <functional>
#include <iostream>
#include "GPIOPin.hpp"

using std::chrono::microseconds;
using std::array;
using std::function;

namespace PT22x2 {

        static constexpr microseconds pulsewidth_short = microseconds(322);
        static constexpr microseconds pulsewidth_long  = microseconds(967);
        static constexpr microseconds pulsewidth_sync  = microseconds(9995);
        static constexpr microseconds pulsewidth_tolerance = microseconds(200);
        static constexpr unsigned int codeword_length = 12;

        enum class Pulse { Short, Long };
        enum class CodeBit { Zero, One, Floating };

        typedef array<Pulse, 4*codeword_length> CodewordPulses;
        typedef array<CodeBit, codeword_length> Codeword;

    class Decoder {
        private:
            microseconds lasttime;
            bool receiving;
            unsigned int position;
            CodewordPulses pulses;
            function<void(Codeword)> codewordHandler;

            Codeword codewordFromPulses(CodewordPulses pulses);
            
        public:
            Decoder(function<void(Codeword)> codewordHandler);
            void edgeOccured(microseconds time);
    };

    class Encoder {
        private:
            GPIOPin pin;
            CodewordPulses pulsesFromCodeword(Codeword codeword);

        public:
            Encoder(GPIOPin pin);
            void send(Codeword codeword);
    };
}

std::ostream& operator<<(std::ostream &strm, const PT22x2::Codeword &codeword);

#endif // PT226X_HPP
