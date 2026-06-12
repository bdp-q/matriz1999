#include "Anim.h"

void anim_update(anim *a) {
    a->timer++;
    if (a->timer >= a->velocity) {
        a->timer = 0;
        a->frame = (a->frame + 1) % a->frame_count; //se chegou no final, volta ao comeco
    }
}