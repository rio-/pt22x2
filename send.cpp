#include <iostream>
#include <string>
#include "GPIOPin.hpp"
#include "PT22x2.hpp"
#include "Command.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::stoi;
using std::string;

void send(PT22x2::Encoder encoder, unsigned int system_code, unsigned int channel, Command::State state) {
    Command cmd(system_code, channel, state);
    auto codeword = cmd.getCodeword();
    for (int i=0; i<5; i++) {
        encoder.send(codeword);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "usage: send <system_code> <channel>|all on|off" << endl;
        return -1;
    }

    GPIOPin pin(24);
    pin.setOutput();
    PT22x2::Encoder encoder(pin);

    unsigned int system_code = stoi(argv[1]);

    Command::State state;
    if (argv[3] == string("on")) {
        state = Command::State::On;
    }
    else if (argv[3] == string("off")) {
        state = Command::State::Off;
    }
    else {
        cout << "state has to be 'on' or 'off'" << endl;
        return -1;
    }

    if (argv[2] == string("all")) {
        send(encoder, system_code, 1, state);
        send(encoder, system_code, 2, state);
        send(encoder, system_code, 4, state);
        send(encoder, system_code, 8, state);
    }
    else {
        unsigned int channel = stoi(argv[2]);
        send(encoder, system_code, channel, state);
    }
    return 0;
}
