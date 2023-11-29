//
// Created by Anderson on 11/28/2023.
//

#ifndef CASTR_ENTITY_H
#define CASTR_ENTITY_H

#include "../Vector/Vector.h"

struct PositionInfo{
    vDouble2d pos;
    vDouble2d dir;
    vDouble2d plane;
};

class Entity {
    PositionInfo positionInfo;
    double moveSpeed = 1;
    double rotationSpeed = 1;

public:
    PositionInfo getPositionInfo();
    void updatePositionInfo(PositionInfo positionInfo);
    void updateMoveSpeed(double val);
    void updateRotationSpeed(double val);
};


#endif //CASTR_ENTITY_H
