#include "base_mode.h"

    // Cellstates
    // 0b00000000
    // 0b01000000 mainplayer owner
    // 0b00100000 active
    // 0b00010000 no owner
    // 0b00000100 R
    // 0b00000010 G
    // 0b00000001 B

struct  TicTacToeMode : BaseMode {
    GameInformation init() override {
        GameInformation game_infomation;

            game_infomation.board_state[8][8] = {0b00010010};

            // tic tac toe borderpattern
            game_infomation.board_state[1][1] = 0b0;
            game_infomation.board_state[1][3] = 0b0;
            game_infomation.board_state[1][5] = 0b0;

            game_infomation.board_state[3][1] = 0b0;
            game_infomation.board_state[3][3] = 0b0;
            game_infomation.board_state[3][5] = 0b0;

            game_infomation.board_state[5][1] = 0b0;
            game_infomation.board_state[5][3] = 0b0;
            game_infomation.board_state[5][5] = 0b0;

            return game_infomation;
    };
    ProcessedGameInformation applyGamelogic(GameInformation game_information, TouchMatrixCell cells[8][8], bool mainplayer) override {
        ProcessedGameInformation processed_game_information;
        int row;
        int col;

        // get playerinput position
        for (int i = 0; i <= 7; ++i) {
            for (int j = 0; j <= 7; ++j) {
                if (cells[i][j].active) {
                    row = i;
                    col = j;
                }
            }
        }

        // check if input is invalid
        if (game_information.board_state[row][col] & 0b00010000 || game_information.board_state[row][col] & 0b00100000){
            processed_game_information.processed_game_information.instructions = 0b10000000;
            return processed_game_information;
        }


        if (mainplayer){
            processed_game_information.processed_game_information.board_state[row][col] = 0b01100100;
            processed_game_information.display_information[row][col].red = 255;
        }
        else{
            processed_game_information.processed_game_information.board_state[row][col] = 0b00100001;
            processed_game_information.display_information[row][col].blue = 255;
        }
    };
};