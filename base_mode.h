#include "touchmatrix.h"
#include "ledmatrix.h"

#ifndef GAMEINFORMATION_H
#define GAMEINFORMATION_H

#include <Arduino.h>

struct GameInformation {
    byte instructions;
    byte board_state[8][8];
};

struct ProcessedGameInformation {
    GameInformation processed_game_information;
    LEDMatrixCell display_information[8][8];
};

struct BaseMode {
    virtual GameInformation init();
    virtual ProcessedGameInformation applyGamelogic(GameInformation game_information, TouchMatrixCell cells[8][8], bool mainplayer);
};

#endif // GAMEINFORMATION_H