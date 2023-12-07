//
// Created by Anderson on 11/28/2023.
//

#include "Player.h"

namespace Entities {
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
        if (keys.shift_key);
    }

    void Player3D::readInput() {
        double dx = sin(this->positionInfo3D.angle * M_PI / 180.0);
        double dy = cos(this->positionInfo3D.angle * M_PI / 180.0);

        if (keys.e_key) {
            this->positionInfo3D.angle += 1;
            if (this->positionInfo3D.angle >= 360) this->positionInfo3D.angle -= 360;
        }

        if (keys.q_key) {
            this->positionInfo3D.angle -= 1;
            if (this->positionInfo3D.angle < 0) this->positionInfo3D.angle += 360;
        }

        if (keys.w_key) {
            this->positionInfo3D.pos[0] += dx;
            this->positionInfo3D.pos[1] += dy;
        }

        if (keys.s_key) {
            this->positionInfo3D.pos[0] -= dx;
            this->positionInfo3D.pos[1] -= dy;
        }

        if (keys.a_key) {
            this->positionInfo3D.pos[0] -= dy;
            this->positionInfo3D.pos[1] += dx;
        }

        if (keys.d_key) {
            this->positionInfo3D.pos[0] += dy;
            this->positionInfo3D.pos[1] -= dx;
        }
    }
}