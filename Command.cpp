#include "Command.hpp"
#include "PT22x2.hpp"
#include <exception>

using std::runtime_error;

Command::Command(PT22x2::Codeword codeword) :
    system_code(0), channel(0), state(State::Off)
{
    for (int i=0; i<5; i++) {
        if (codeword[i] == PT22x2::CodeBit::Zero) {
            system_code |= 1 << i;
        }
        else if (codeword[i] == PT22x2::CodeBit::One) {
            throw runtime_error("malformed command");
        }
    }

    for (int i=0; i<4; i++) {
        if (codeword[i+5] == PT22x2::CodeBit::Zero) {
            channel |= 1 << i;
        }
        else if (codeword[i+5] == PT22x2::CodeBit::One) {
            throw runtime_error("malformed command");
        }
    }

    if (channel == 0) {
        throw runtime_error("malformed command");
    }

    if (codeword[10] == PT22x2::CodeBit::Floating && codeword[11] == PT22x2::CodeBit::Zero) {
        state = State::Off;
    }
    else if (codeword[10] == PT22x2::CodeBit::Zero && codeword[11] == PT22x2::CodeBit::Floating) {
        state = State::On;
    }
    else {
        throw runtime_error("malformed command");
    }
}

Command::Command(unsigned int system_code, unsigned int channel, State state) :
    system_code(system_code), channel(channel), state(state)
{}

PT22x2::Codeword Command::getCodeword() {
    PT22x2::Codeword codeword;

    for (int i=0; i<5; i++) {
        if ((system_code >> i) & 1) {
            codeword[i] = PT22x2::CodeBit::Zero;
        }
        else {
            codeword[i] = PT22x2::CodeBit::Floating;
        }
    }

    for (int i=0; i<4; i++) {
        if ((channel >> i) & 1) {
            codeword[i+5] = PT22x2::CodeBit::Zero;
        }
        else {
            codeword[i+5] = PT22x2::CodeBit::Floating;
        }
    }

    codeword[9] = PT22x2::CodeBit::Floating;

    if (state == State::Off) {
        codeword[10] = PT22x2::CodeBit::Floating;
        codeword[11] = PT22x2::CodeBit::Zero;
    }
    else {
        codeword[10] = PT22x2::CodeBit::Zero;
        codeword[11] = PT22x2::CodeBit::Floating;
    }

    return codeword;
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
