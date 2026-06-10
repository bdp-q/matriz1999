#include "Hitbox.h"

int hitbox_collide( hitbox *a, hitbox *b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    //pra sempre ser positivo
    if (dx < 0) 
        dx = -dx;
    if (dy < 0) 
        dy = -dy;
    //estao colidindo se a distancia deles for menor que a soma das suas metades (dado que x e y sao o centro do bloco)
    return (dx < a->hw + b->hw) && (dy < a->hh + b->hh);
}

