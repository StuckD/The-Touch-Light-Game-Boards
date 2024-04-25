#include "base_mode.h"

struct ConnectMode : public BaseMode {
    GameInformation init() override {

    };

    ProcessedGameInformation applyGamelogic(GameInformation game_information, TouchMatrixCell cells[8][8], bool mainplayer) override {

    };
};