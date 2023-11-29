//
// Created by Anderson on 11/28/2023.
//

#include "Entity.h"

void Entity::updatePositionInfo(PositionInfo positionInfo) {
    this->positionInfo = positionInfo;
}

PositionInfo &Entity::getPositionInfo() {
    return this->positionInfo;
}

