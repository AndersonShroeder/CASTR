//
// Created by Anderson on 11/29/2023.
//

#include "Vector.h"

vDouble2d rotate2D(vDouble2d &v, double degrees) {
    return vDouble2d{v[0] * cos(degrees) - v[1] * sin(degrees), v[0] * sin(degrees) + v[1] * cos(degrees)};
}