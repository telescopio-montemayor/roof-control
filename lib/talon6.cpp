/*
 *  Talon6 compatible protocol implementation
 *
 *  Copyright (C) 2020 Adrián Pardini <github@tangopardo.com.ar> - Universidad Nacional de La Plata
 *
 */

#include <string.h>

#include "talon6.h"

using namespace Talon6;


Parser::Parser()
{

}


Commands Parser::parse_char(const char c)
{
    switch (state) {
        case IDLE:
            if (c == '&') {
                state = PARSING;
                idx = 0;
            }
            break;

        case PARSING:
            if (c == '#') {
                state = IDLE;
                last_command = parse_buffer();
                return last_command;
            }

            if (idx < BUF_SIZE) {
               buf[idx] = c;
               idx++;
            } else {
                state = IDLE;
            }

            break;
    }

    return Commands::NO_COMMAND;
}

Commands Parser::get_last_command()
{
    return last_command;
}

const char * Parser::get_last_command_parameters()
{
    return parameters;
}

Commands Parser::parse_buffer()
{
    memset(parameters, 0, BUF_SIZE);

    switch(buf[0]) {
        case 'G':
            return Commands::GET_STATUS;
        case 'V':
            return Commands::GET_FIRMWARE_VERSION;
        case 'C':
            return Commands::PARK;
        case 'P':
            return Commands::PARK_SAFE;
        case 'O':
            return Commands::UNPARK;
        case 'S':
            return Commands::ABORT_MOTION;
        case 'A':
            // XXX FIXME: parse position
            return Commands::GOTO;
    }

    return Commands::UNKNOWN;
}



Roof::Roof()
{

}



const char * ResponseEncoder::create_command_response(Commands command)
{
    memset(buf, 0, BUF_SIZE);
    char *response = &buf[2];


    switch (command) {

        case Commands::GET_FIRMWARE_VERSION:
            buf[0] = '&';
            buf[1] = 'V';
            memcpy(response, _roof.firmware_version, 5);
            buf[6] = '\n';
            break;

        case Commands::GET_STATUS:
            buf[0] = '&';
            buf[1] = 'G';
            buf[BUF_SIZE - 1] = '\n';

            buf[2] = 0x7F & (((int)_roof.status << 4) | (int)_roof.last_action);

            buf[3] = (_roof.current_position >> 14) & 0x7F;
            buf[4] = (_roof.current_position >> 7)  & 0x7F;
            buf[5] = _roof.current_position & 0x7F;

            buf[6] = (_roof.voltage >> 7) & 0x07;
            buf[7] = _roof.voltage & 0x7F;

            buf[8]  = (_roof.closing_timer >> 14) & 0x7F;
            buf[9]  = (_roof.closing_timer >> 7)  & 0x7F;
            buf[10] = _roof.closing_timer & 0x7F;

            buf[11] = (_roof.power_lost_timer >> 7) & 0x07;
            buf[12] = _roof.power_lost_timer & 0x7F;

            buf[13] = (_roof.weather_condition_timer >> 7) & 0x07;
            buf[14] = _roof.weather_condition_timer & 0x7F;

            // XXX FIXME: These next three seem to be wrong interpreted on the INDI driver.
            if (_roof.close_switch) {
                buf[15] |= 0x01;
            }

            if (_roof.direct_command_switch) {
                buf[15] |= 0x02;
            }

            if (_roof.management_switch) {
                buf[15] |= 0x04;
            }

            if (_roof.power_lost) {
                buf[16] |= 0x01;
            }

            if (_roof.weather_conditions) {
                buf[16] |= 0x02;
            }

            if (_roof.mount_parked) {
                buf[16] |= 0x04;
            }

            if (_roof.open) {
                buf[16] |= 0x08;
            }

            if (_roof.closed) {
                buf[16] |= 0x10;
            }

            if (_roof.open_switch) {
                buf[16] |= 0x20;
            }

            if (_roof.stop_switch) {
                buf[16] |= 0x40;
            }

            break;

        default:
            break;
    }

    return buf;
}
