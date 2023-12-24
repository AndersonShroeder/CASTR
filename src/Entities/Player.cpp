//
// Created by Anderson on 11/28/2023.
//

#include "Player.h"

namespace Entities {
    void Player2D::readInput() {
        double tempMoveSpeed = moveSpeed;
        if (keys.shift_key) {
            tempMoveSpeed *= 2;
        }

        vDouble3d v = (this->positionInfo.dir) * tempMoveSpeed;
        if (keys.e_key) {
            this->positionInfo.dir = rotate2D(this->positionInfo.dir, -1 * rotationSpeed);
            this->positionInfo.plane = rotate2D(this->positionInfo.plane, -1 * rotationSpeed);
        }
        if (keys.q_key) {
            this->positionInfo.dir = rotate2D(this->positionInfo.dir, rotationSpeed);
            this->positionInfo.plane = rotate2D(this->positionInfo.plane, rotationSpeed);
        }
        if (keys.w_key) {
            this->positionInfo.pos[0] += v[0];
            this->positionInfo.pos[1] += v[1];
        }

        if (keys.a_key) {
            this->positionInfo.pos[0] -= v[1];
            this->positionInfo.pos[1] += v[0];
        }

        if (keys.s_key) {
            this->positionInfo.pos[0] -= v[0];
            this->positionInfo.pos[1] -= v[1];
        }
        if (keys.d_key) {
            this->positionInfo.pos[0] += v[1];
            this->positionInfo.pos[1] -= v[0];
        }
    }

    void Player3D::readInput() {
        double dx = sin(this->positionInfo.angle * M_PI / 180.0) * .5;
        double dy = cos(this->positionInfo.angle * M_PI / 180.0) * .5;

        if (keys.shift_key) {
            if (keys.w_key) {
                positionInfo.pos[2] += 1;
            }
            if (keys.s_key) {
                positionInfo.pos[2] -= 1;
            }

            if (keys.a_key) {
                positionInfo.upDown -= 1;
            }
            if (keys.d_key) {
                positionInfo.upDown += 1;
            }
        }

        else {
            if (keys.e_key) {
                this->positionInfo.angle += 1;
                if (this->positionInfo.angle >= 360) this->positionInfo.angle -= 360;
            }

            if (keys.q_key) {
                this->positionInfo.angle -= 1;
                if (this->positionInfo.angle < 0) this->positionInfo.angle += 360;
            }

            if (keys.w_key) {
                this->positionInfo.pos[0] += dx;
                this->positionInfo.pos[1] += dy;
            }

            if (keys.s_key) {
                this->positionInfo.pos[0] -= dx;
                this->positionInfo.pos[1] -= dy;
            }

            if (keys.a_key) {
                this->positionInfo.pos[0] -= dy;
                this->positionInfo.pos[1] += dx;
            }

            if (keys.d_key) {
                this->positionInfo.pos[0] += dy;
                this->positionInfo.pos[1] -= dx;
            }
        }
    }
}