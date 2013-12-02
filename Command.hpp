#ifndef COMMAND_HPP
#define COMMAND_HPP
#include "PT226xDecoder.hpp"
#include <ostream>

class Command {
    public:
        unsigned int system_code;
        unsigned int channel;
        enum class State { Off, On } state;

        Command(PT226xDecoder::Codeword codeword);
};

std::ostream& operator<<(std::ostream &strm, const Command &cmd);

#endif // COMMAND_HPP
