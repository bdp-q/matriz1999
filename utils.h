#ifndef __UTILS__
#define __UTILS__

//funcao que recebe o x o y e o tamanho de 2 objetos e ve se tao se tocando
//1 se tocar 0 se nao colidir
unsigned char detect_colision(unsigned short x1, unsigned short y1, unsigned char side1, unsigned short x2, unsigned short y2, unsigned short side2);

#endif