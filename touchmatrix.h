#ifndef TOUCHMATRIX_H
#define TOUCHMATRIX_H

struct TouchMatrixCell {
    bool active;
};
class TouchMatrix {
    private:
        int multiplex_selectors[4];
        int multiplex_signal;
    public:

        TouchMatrixCell cells[8][8];

        TouchMatrix(const int m_s[4], int m_si);
        void update();
        void clearcells();
        int getActiveCellsCount();
};


#endif // TOUCHMATRIX_H