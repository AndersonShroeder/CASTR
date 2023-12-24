//
// Created by Anderson on 11/29/2023.
//

#include "Vector.h"

vDouble3d rotate2D(vDouble3d &v, double degrees) {
    return vDouble3d{v[0] * cos(degrees) - v[1] * sin(degrees), v[0] * sin(degrees) + v[1] * cos(degrees), 0};
}

vDouble3d perp2D(vDouble3d &v) {
    return vDouble3d{-1*v[1], v[0], 0};
}