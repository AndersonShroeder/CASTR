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
protected:
    PositionInfo positionInfo;
    double moveSpeed = .001;
    double rotationSpeed = .001;

public:
    PositionInfo &getPositionInfo();
    void updatePositionInfo(PositionInfo positionInfo);

    double getMoveSpeed();
};


#endif //CASTR_ENTITY_H
