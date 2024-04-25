#include <Adafruit_NeoPixel.h>
#include "ledmatrix.h"

LedMatrix::LedMatrix(int PIN){
    num_rows = 8;
    num_col = 8;
    num_pixels = num_rows * num_col;
    strip = Adafruit_NeoPixel(num_pixels, PIN, NEO_GRB + NEO_KHZ800);
};

void LedMatrix::update_LED_strip_with_matrix(LEDMatrixCell matrix[8][8]){
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_col; col++) {
            if (row % 2 == 0) {
                strip.setPixelColor(row * num_col + col, strip.Color(matrix[col][row].red,
                                                                     matrix[col][row].green,
                                                                     matrix[col][row].blue));
            } else {
                int inverted_col = num_col - 1 - col;
                strip.setPixelColor(row * num_col + col, strip.Color(matrix[col][row].red,
                                                                     matrix[col][row].green,
                                                                     matrix[col][row].blue));
            }

        }
    }
};