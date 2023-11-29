//
// Created by Anderson on 11/28/2023.
//

#include "Player.h"

bool e_key = false;
bool q_key = false;
bool w_key = false;
bool a_key = false;
bool s_key = false;
bool d_key = false;
bool shift_key = false;

void Player::readInput() {
    if (keys.e_key) {
        this->positionInfo.dir = rotate2D(this->positionInfo.dir, -1 * rotationSpeed);
        this->positionInfo.plane = rotate2D(this->positionInfo.plane, -1 * rotationSpeed);
    }
    if (keys.q_key) {
        this->positionInfo.dir = rotate2D(this->positionInfo.dir, rotationSpeed);
        this->positionInfo.plane = rotate2D(this->positionInfo.plane, rotationSpeed);
    }
    if (keys.w_key) this->positionInfo.pos += (this->positionInfo.dir) * this->moveSpeed;
//    if (keys.a_key) this->positionInfo.pos += (this->positionInfo.dir) * this->getMoveSpeed();
    if (keys.s_key) this->positionInfo.pos -= (this->positionInfo.dir) * this->moveSpeed;
//    if (keys.d_key) this->positionInfo.pos += (this->positionInfo.dir) * this->getMoveSpeed();
    if (keys.shift_key) ;
}
