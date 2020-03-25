/*
 *  Talon6 compatible protocol implementation
 *
 *  Copyright (C) 2020 Adrián Pardini <github@tangopardo.com.ar> - Universidad Nacional de La Plata
 *
 */

namespace Talon6 {
    static constexpr const int BUF_SIZE = 18;

    enum class Commands {
        GET_STATUS              = 1,
        GET_FIRMWARE_VERSION    = 2,
        PARK                    = 4,
        PARK_SAFE               = 8,
        UNPARK                  = 0x10,
        GOTO                    = 0x20,
        ABORT_MOTION            = 0x40,

        UNKNOWN                 = 128,
        NO_COMMAND              = 0,
    };

    enum class RoofStatus {
        OPEN    = 0,
        CLOSED  = 1,
        OPENING = 2,
        CLOSING = 3,
        ERROR   = 4,
    };

    enum class LastAction {
        NONE                            =   0,
        OPEN_BY_USER                    =   1,
        CLOSE_BY_USER                   =   2,
        NOT_USED                        =   3,
        GO_TO_BY_USER                   =   4,
        CALIBRATE_BY_USER               =   5,
        CLOSED_DUE_TO_RAIN_CLOUD        =   6,
        CLOSE_DUE_TO_POWER_DOWN         =   7,
        CLOSE_DUE_TO_COMMUNICATION_LOST =   8,
        CLOSE_DUE_TO_INTERNET_LOST      =   9,
        CLOSE_DUE_TO_TIMEOUT_EXPIRED    =   10,
        CLOSE_BY_MANAGEMENT             =   11,
        CLOSE_BY_AUTOMATION             =   12,
        STOP_MOTOR_STALLED              =   13,
        EMERGENCY_STOP                  =   14,
        ORDERED_THE_MOUNT_TO_PARK       =   15,
    };


    class Parser {
        public:
            Parser();
            Commands parse_char(const char c);
            Commands get_last_command();
            const char * get_last_command_parameters();

        private:
            char buf[BUF_SIZE];
            char parameters[BUF_SIZE];
            Commands last_command = Commands::NO_COMMAND;
            int idx = 0;
            enum PARSE_STATE {
                IDLE,
                PARSING,
            } state = IDLE;

            Commands parse_buffer();
    };


    class Roof {
        public:
            Roof();

            long EncoderTicks = 60;

            RoofStatus status = RoofStatus::CLOSED;

            char firmware_version[5] = "roof";

            LastAction last_action = LastAction::NONE;
            long current_position = 0;
            int voltage = 0;                        // In units of (15/1024) Volts
            int closing_timer = 0;
            int power_lost_timer = 0;
            int weather_condition_timer = 0;

            // Sensors:
            bool power_lost = false;
            bool weather_conditions = false;
            bool mount_parked = false;
            bool closed = true;
            bool open = false;

            //Switches
            bool open_switch = false;
            bool close_switch = false;
            bool stop_switch = false;
            bool direct_command_switch = false;
            bool management_switch = false;
    };


    class ResponseEncoder {
        public:
            ResponseEncoder(Roof& roof): _roof(roof) {};

            const char *create_command_response(Commands command);

        private:
            Roof &_roof;
            char buf[BUF_SIZE] = { '\n' };
    };
}
