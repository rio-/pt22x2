#include "PT226xDecoder.hpp"
#include <exception>

using std::chrono::microseconds;
using std::array;
using std::runtime_error;

constexpr microseconds PT226xDecoder::pulsewidth_short;
constexpr microseconds PT226xDecoder::pulsewidth_long;
constexpr microseconds PT226xDecoder::pulsewidth_sync;
constexpr microseconds PT226xDecoder::pulsewidth_tolerance;

PT226xDecoder::PT226xDecoder(function<void(PT226xDecoder::Codeword)> codewordHandler) :
    lasttime(microseconds(0)),
    receiving(false),
    position(0),
    codewordHandler(codewordHandler)
{
}

void PT226xDecoder::edgeOccured(microseconds time)
{
    microseconds now = time;
    microseconds delta = now - lasttime;
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

            Codeword codeword = codewordFromPulses(pulses);
            if (codewordHandler) {
                codewordHandler(codeword);
            }
        }

    }
}

PT226xDecoder::Codeword PT226xDecoder::codewordFromPulses(PT226xDecoder::CodewordPulses pulses) {
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
