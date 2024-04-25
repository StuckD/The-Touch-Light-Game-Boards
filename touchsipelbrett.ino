// from DStuck

/*
------------------------------- Configuration ------------------------------------------
*/
#include <algorithm>
#include <vector>

#include "touchmatrix.h"
#include "ledmatrix.h"
#include "connection.h"

#include "base_mode.h"
#include "tictactoe.cpp"
#include "checker.cpp"
#include "connect.cpp"

#define _MULTPLEX_SIG 26

#define _MULITPLEX_SELECTOR0 17
#define _MULITPLEX_SELECTOR1 16
#define _MULITPLEX_SELECTOR2 4
#define _MULITPLEX_SELECTOR3 2

#define _LED_DATA_PIN 33

#define _StartMode TICTACTOE

#define _MainBoard true

#if _MainBoard
    const uint8_t reciever_mac_address[6] = {0xE4, 0x65, 0xB8, 0x11, 0x12, 0x28,};
#else
    const uint8_t reciever_mac_address[6] = {0xE4, 0x65, 0xB8, 0x0D, 0x12, 0x08,};
#endif


const int multiplex_s[4] = {_MULITPLEX_SELECTOR0,
                            _MULITPLEX_SELECTOR1,
                            _MULITPLEX_SELECTOR2,
                            _MULITPLEX_SELECTOR3,};

/*
------------------------------- Structs ------------------------------------------
*/

enum BoardMode {
    TICTACTOE,
    CONNECT,
    CHECKER
};

/*
------------------------------- Globals ------------------------------------------
*/
TouchMatrix touch_matrix(multiplex_s, _MULTPLEX_SIG );
LedMatrix led_matrix(_LED_DATA_PIN);

BaseMode* mode;

bool playerturn = _MainBoard ? true : false;

BoardMode boardmodes[] = {
    TICTACTOE,
    CONNECT,
    CHECKER
};

uint8_t game_selector = 0;

/*
------------------------------- Funktions ------------------------------------------
*/



/*!
    @brief Changes and initialises the board mode
*/
void selectBoardMode(BoardMode board_mode){
    if(mode)
     delete mode;
    switch (board_mode)
     {
        case TICTACTOE:
            mode = new TicTacToeMode();
            break;
        case CONNECT:
            mode = new ConnectMode();
            break;
        case CHECKER:
            mode = new CheckerMode();
            break;
    }
    GameInformation game_information = mode->init();
}

void printBin(byte aByte) {
    for (int8_t aBit = 7; aBit >= 0; aBit--)
        Serial.write(bitRead(aByte, aBit) ? '1' : '0');
    Serial.println();
}

void serialprintmatrix(TouchMatrix matrix){
    for (int i = 0; i <= 7; ++i) {
        for (int j = 0; j <= 7; ++j) {
            Serial.print(matrix.cells[j][i].active ? "1" : "0");
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println();
}

LEDMatrixCell convertBoardStateToLEDCell (byte board_state) {
    LEDMatrixCell display_information;
    if (board_state & 0b00000100)
        display_information.red = 255;
    if (board_state & 0b00000010)
        display_information.green = 255;
    if (board_state & 0b00000001)
        display_information.blue = 255;
    return display_information;
}

void controllBoardActions(){
    static uint32_t swap_game_timer = 0;
    bool swap_game = false;

    if (connection_data_buffer.id != last_connection_id && connection_data_buffer.instructions & 0b10000000)
        swap_game = true;

    if(touch_matrix.getActiveCellsCount() > 12){
        if((swap_game_timer - millis() > 5000)){
            swap_game = true;
            }
    }
    else {
        swap_game_timer = millis();
    }

    if (swap_game){
        game_selector = (game_selector < 3) ? game_selector++ : 0;
        ConnectionDataBuffer data_buffer;
        data_buffer.instructions = 0b10000000;
        sendData(reciever_mac_address, data_buffer);
    }
}

void managePlayerTurn(){
    if(playerturn){

        // gets the data from the connection buffer
        GameInformation data;
        data.instructions = connection_data_buffer.instructions;
        memcpy(data.board_state, connection_data_buffer.board_state, sizeof(data.board_state));

        LEDMatrixCell display_information[8][8];


        for (int col = 0; col < 8; col++) {
            for (int row = 0; row < 8; row++) {
                display_information[row][col] = convertBoardStateToLEDCell(data.board_state[row][col]);
            }
        }
        led_matrix.update_LED_strip_with_matrix(display_information);


        if (touch_matrix.getActiveCellsCount() != 1)
            return;

        //applies the game logic
        ProcessedGameInformation processed_data = mode->applyGamelogic(data, touch_matrix.cells, _MainBoard);

        // invalid move
        if (processed_data.processed_game_information.instructions & 0b10000000)
            return;

        // updates the led matrix
        led_matrix.update_LED_strip_with_matrix(processed_data.display_information);

        // send updated gamestate to opponment
        ConnectionDataBuffer connection_data;
        connection_data.id = last_connection_id++;
        memcpy(connection_data.board_state, processed_data.processed_game_information.board_state, sizeof(connection_data.board_state));
        connection_data.instructions = processed_data.processed_game_information.instructions;
        sendData(reciever_mac_address, connection_data);

        playerturn = false;
    }
    else {
        if(last_connection_id  != connection_data_buffer.id)
            playerturn = true;
    }
}

/*!
    @brief Debugging function to display Matrix in Serial
    @param currently_active touchmatrix to display
*/
void printBits(byte currently_active) {
    byte vert = currently_active & 0b1111;
    byte hori = (currently_active & 0b11110000)>> 4;

    byte v_mask = 0b1000;
    byte h_mask = 0b0001;

    byte active_in_line = 0b0000;

    for(int v = 0; v<= 3; v++){
        for(int h = 0; h <= 3; h++) {
            if (hori & h_mask && vert & v_mask)
                active_in_line = active_in_line | h_mask;
            h_mask = h_mask << 1;
        }
        printBin(active_in_line);
        Serial.println();
        v_mask = v_mask >> 1;
    }

    Serial.println();
}

/*
------------------------------- Main loop ------------------------------------------
*/

void setup() {
    pinMode(_MULTPLEX_SIG, INPUT);

    pinMode(_MULITPLEX_SELECTOR0, OUTPUT);
    pinMode(_MULITPLEX_SELECTOR1, OUTPUT);
    pinMode(_MULITPLEX_SELECTOR2, OUTPUT);
    pinMode(_MULITPLEX_SELECTOR3, OUTPUT);

    Serial.begin(115200);

    selectBoardMode(_StartMode);

    startConnection(reciever_mac_address);

    delay(1000);
}

void loop() {
    touch_matrix.clearcells();
    touch_matrix.update();
    controllBoardActions();
    managePlayerTurn();
}