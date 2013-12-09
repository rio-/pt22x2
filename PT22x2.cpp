#include "PT22x2.hpp"
#include <exception>
#include <iostream>
#include <thread>

using std::chrono::microseconds;
using std::array;
using std::runtime_error;
using std::this_thread::sleep_for;

namespace PT22x2 {
    Decoder::Decoder(function<void(Codeword)> codewordHandler) :
        lasttime(microseconds(0)),
        receiving(false),
        position(0),
        codewordHandler(codewordHandler)
    {
    }

    void Decoder::edgeOccured(microseconds time)
    {
        auto now = time;
        auto delta = now - lasttime;
        lasttime = now;

        if (delta >= pulsewidth_sync - pulsewidth_tolerance) {
            // Sync received, start new codeword
            receiving = true;
            position  = 0;
            return;
        }

        if (receiving) {
            if (pulsewidth_short - pulsewidth_tolerance <= delta &&
                pulsewidth_short + pulsewidth_tolerance >= delta) {
                pulses[position++] = Pulse::Short;
            }
            else if (pulsewidth_long - pulsewidth_tolerance <= delta &&
                     pulsewidth_long + pulsewidth_tolerance >= delta) {
                pulses[position++] = Pulse::Long;
            }
            else {
                // Noise, stop receiving
                receiving = false;
                position = 0;
                return;
            }

            if (position == 4*codeword_length) {
                // Codeword received, stop receiving and print
                receiving = false;
                position = 0;

                bool got_codeword = false;
                Codeword codeword;
                try {
                    codeword = codewordFromPulses(pulses);
                    got_codeword = true;
                }
                catch (runtime_error const &e) {
                }

                if (got_codeword && codewordHandler) {
                    codewordHandler(codeword);
                }
            }

        }
    }

    Codeword Decoder::codewordFromPulses(CodewordPulses pulses) {
        Codeword codeword;
        for (int i=0; i < codeword_length; i++) {
            if (pulses[4*i]   == Pulse::Short &&
                pulses[4*i+1] == Pulse::Long &&
                pulses[4*i+2] == Pulse::Short &&
                pulses[4*i+3] == Pulse::Long) {
                codeword[i] = CodeBit::Zero;
                continue;
            }
            if (pulses[4*i]   == Pulse::Long &&
                pulses[4*i+1] == Pulse::Short &&
                pulses[4*i+2] == Pulse::Long &&
                pulses[4*i+3] == Pulse::Short) {
                codeword[i] = CodeBit::One;
                continue;
            }
            if (pulses[4*i]   == Pulse::Short &&
                pulses[4*i+1] == Pulse::Long &&
                pulses[4*i+2] == Pulse::Long &&
                pulses[4*i+3] == Pulse::Short) {
                codeword[i] = CodeBit::Floating;
                continue;
            }
            throw runtime_error("malformed pulses");
        }
        return codeword;
    }

    Encoder::Encoder(GPIOPin pin) : pin(pin) {}

    CodewordPulses Encoder::pulsesFromCodeword(Codeword codeword) {
        CodewordPulses pulses;
        for (int i=0; i < codeword_length; i++) {
            switch (codeword[i]) {
                case CodeBit::Zero:
                    pulses[4*i]   = Pulse::Short;
                    pulses[4*i+1] = Pulse::Long;
                    pulses[4*i+2] = Pulse::Short;
                    pulses[4*i+3] = Pulse::Long;
                    break;
                case CodeBit::One:
                    pulses[4*i]   = Pulse::Long;
                    pulses[4*i+1] = Pulse::Short;
                    pulses[4*i+2] = Pulse::Long;
                    pulses[4*i+3] = Pulse::Short;
                    break;
                case CodeBit::Floating:
                    pulses[4*i]   = Pulse::Short;
                    pulses[4*i+1] = Pulse::Long;
                    pulses[4*i+2] = Pulse::Long;
                    pulses[4*i+3] = Pulse::Short;
                    break;
            }
        }
        return pulses;
    }

    void Encoder::send(Codeword codeword) {
        auto pulses = pulsesFromCodeword(codeword);
        bool nextPulseHigh = true;
        for (int i=0; i < codeword_length * 4; i++) {
            if (nextPulseHigh) {
                pin.setHigh();
                nextPulseHigh = false;
            }
            else {
                pin.setLow();
                nextPulseHigh = true;
            }
            
            if (pulses[i] == Pulse::Short) {
                sleep_for(pulsewidth_short);
            }
            else {
                sleep_for(pulsewidth_long);
            }
        }

        // SYNC bit
        pin.setHigh();
        sleep_for(pulsewidth_short);
        pin.setLow();
        sleep_for(pulsewidth_sync);
    }
}

std::ostream& operator<<(std::ostream &strm, const PT22x2::Codeword &codeword) {
    strm << "<Codeword: ";
    for (int i=0; i < PT22x2::codeword_length; i++) {
        switch (codeword[i]) {
            case PT22x2::CodeBit::Zero:
                strm << '0';
                break;
            case PT22x2::CodeBit::One:
                strm << '1';
                break;
            case PT22x2::CodeBit::Floating:
                strm << 'f';
                break;
        }
    }
    strm << ">";
    return strm;
}
