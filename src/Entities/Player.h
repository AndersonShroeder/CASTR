//
// Created by Anderson on 11/28/2023.
//

#ifndef CASTR_PLAYER_H
#define CASTR_PLAYER_H


#include "../Input/InputReader.h"
#include "Entity.h"

class Player: public InputReader, public Entity {
    void readInput();
};


#endif //CASTR_PLAYER_H
