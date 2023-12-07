//
// Created by Anderson on 11/28/2023.
//

#include "Entity.h"

namespace Entities {
    void Entity::updatePositionInfo(PositionInfo2D positionInfo) {
        this->positionInfo = positionInfo;
    }

    PositionInfo2D &Entity::getPositionInfo() {
        return this->positionInfo;
    }

}