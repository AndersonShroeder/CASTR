//
// Created by Anderson on 11/28/2023.
//

#include "Camera.h"

void Camera::subscribe(Entity &entity) {
    this->positionInfo = &(entity.getPositionInfo());
}

PositionInfo2D Camera::getPositionInfo() {
    return *(this->positionInfo);
}

