


unsigned char detect_colision(unsigned short x1, unsigned short y1, unsigned char side1, unsigned short x2, unsigned short y2, unsigned short side2){
    if ((x1 < x2 + side2) || (x1 + side1 > x2) || (y1 < y2 + side2) || (y1 + side1 > y2))
        return 1;
    return 0;
}
