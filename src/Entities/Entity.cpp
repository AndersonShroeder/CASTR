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

    void Entity3D::updatePositionInfo(PositionInfo3D positionInfo) {
        this->positionInfo3D = positionInfo;
    }

    PositionInfo3D &Entity3D::getPositionInfo() {
        return this->positionInfo3D;
    }
}