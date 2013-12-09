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

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "usage: send <system_code> <channel> on|off" << endl;
        return -1;
    }

    unsigned int system_code = stoi(argv[1]);
    unsigned int channel     = stoi(argv[2]);
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

    GPIOPin pin(24);
    pin.setOutput();
    PT22x2::Encoder encoder(pin);

    Command cmd(system_code, channel, state);
    auto codeword = cmd.getCodeword();
    cout << cmd << endl;
    cout << codeword << endl;
    for (int i=0; i<5; i++) {
        encoder.send(codeword);
    }

    return 0;
}
