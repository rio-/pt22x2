#ifndef COMMAND_HPP
#define COMMAND_HPP
#include "PT22x2.hpp"
#include <ostream>

class Command {
    public:
        unsigned int system_code;
        unsigned int channel;
        enum class State { Off, On } state;

        Command(PT22x2::Codeword codeword);
        Command(unsigned int system_code, unsigned int channel, State state);
        PT22x2::Codeword getCodeword();
};

std::ostream& operator<<(std::ostream &strm, const Command &cmd);

#endif // COMMAND_HPP
