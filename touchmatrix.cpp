#include "touchmatrix.h"
#include <Arduino.h>

/*!
    @brief Sets the active state on all cells to false
*/
void TouchMatrix::clearcells() {
    for (int h = 0; h<=7; h++){
        for(int v = 0; v<=7; v++){
            TouchMatrixCell cell;
            cell.active = false;
            cells[h][v] = cell;
        }
    }
}

TouchMatrix::TouchMatrix(const int m_s[4], int m_si) {
    std::copy(m_s, m_s + 4, multiplex_selectors);
    multiplex_signal = m_si;

    clearcells();
}

/*!
    @brief Reads the Mutiplexer and updates the cells attribute
*/
void TouchMatrix::update() {
    bool active_h[8] = {false, false, false, false, false, false, false, false};

    for(int multiplex_index = 0; multiplex_index<=15; multiplex_index++){

        for (int selector_index = 0; selector_index < 4; selector_index++) {
            digitalWrite(multiplex_selectors[selector_index],
                         ((byte)multiplex_index & (1 << selector_index)) ? HIGH : LOW);
        }

        delay(35); // mulitplexer is too slow or bug in my software idk

        if(multiplex_index <= 7){
            if(digitalRead(multiplex_signal)){
                active_h[multiplex_index] = true;
            }
        }
        else {
            if(digitalRead(multiplex_signal)){
                for(int group_index = 0; group_index <= 7; group_index++){
                    if(active_h[group_index]){
                        cells[group_index][multiplex_index-8].active = true;
                    }
                }
            }
        }
    }

};

/*!
    @brief Returns the amount of active cells
*/
int TouchMatrix::getActiveCellsCount(){
    int counter = 0;
    for(int i = 0; i <= 8; i++){
        for(int j = 0; j <= 8; j++){
            if(cells[j][i].active)
                counter++;
        }
    }
    return counter;
};