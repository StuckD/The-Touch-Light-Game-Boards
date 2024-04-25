#include <Adafruit_NeoPixel.h>


#ifndef LED_MATRIX_CELL_H
#define LED_MATRIX_CELL_H

struct LEDMatrixCell {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class LedMatrix{
    public:
        int num_pixels;
        int num_rows;
        int num_col;
        Adafruit_NeoPixel strip;

        LedMatrix(int PIN);
        void update_LED_strip_with_matrix(LEDMatrixCell matrix[8][8]);
};

#endif // LED_MATRIX_CELL_H