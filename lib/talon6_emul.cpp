#include <iostream>
#include <fstream>

using namespace std;
#include "talon6.h"



int main(int argc, char *argv[])
{
    Talon6::Parser parser;
    Talon6::Roof roof;
    Talon6::ResponseEncoder response_encoder(roof);

    Talon6::Commands cmd = Talon6::Commands::NO_COMMAND;
    const char *response;

    std::fstream serial_in, serial_out;
    char c;

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " path/to/pty" << endl;
        return 1;
    }

    serial_in.open(argv[1], fstream::in);
    serial_out.open(argv[1], fstream::out);

    while (1) {

        while (serial_in.get(c)) {
            cmd = parser.parse_char(c);
            if (cmd != Talon6::Commands::NO_COMMAND) {
                response = response_encoder.create_command_response(cmd);

                serial_out.write(response, Talon6::BUF_SIZE - 1);
                serial_out.flush();

                cmd = Talon6::Commands::NO_COMMAND;
            }
        }

    }

    serial_in.close();
    serial_out.close();
    return 0;
}
