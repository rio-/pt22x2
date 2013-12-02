#include "Command.hpp"
#include "PT22x2Decoder.hpp"
#include <exception>

using std::runtime_error;

Command::Command(PT22x2Decoder::Codeword codeword) :
    system_code(0), channel(0), state(State::Off)
{
    for (int i=0; i<5; i++) {
        if (codeword[i] == PT22x2Decoder::CodeBit::Zero) {
            system_code |= 1 << i;
        }
        else if (codeword[i] == PT22x2Decoder::CodeBit::One) {
            throw runtime_error("malformed command");
        }
    }

    for (int i=0; i<4; i++) {
        if (codeword[i+5] == PT22x2Decoder::CodeBit::Zero) {
            channel |= 1 << i;
        }
        else if (codeword[i+5] == PT22x2Decoder::CodeBit::One) {
            throw runtime_error("malformed command");
        }
    }

    if (channel == 0) {
        throw runtime_error("malformed command");
    }

    if (codeword[10] == PT22x2Decoder::CodeBit::Floating && codeword[11] == PT22x2Decoder::CodeBit::Zero) {
        state = State::Off;
    }
    else if (codeword[10] == PT22x2Decoder::CodeBit::Zero && codeword[11] == PT22x2Decoder::CodeBit::Floating) {
        state = State::On;
    }
    else {
        throw runtime_error("malformed command");
    }
}

std::ostream& operator<<(std::ostream &strm, const Command &cmd) {
    strm << "<Command: system_code=" << cmd.system_code << ", "
         << "channel=" << cmd.channel << ", state=";
    switch (cmd.state) {
        case Command::State::Off:
            strm << "off";
            break;
        case Command::State::On:
            strm << "on";
            break;
    }
    strm << ">";
    return strm;
}
