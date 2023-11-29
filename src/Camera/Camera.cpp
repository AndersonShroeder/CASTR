//
// Created by Anderson on 11/28/2023.
//

#include "Camera.h"

void Camera::subscribe(Entity &entity) {
    this->subscribed = entity;
}

void Camera::visitSubscribed() {
    this->positionInfo = subscribed.getPositionInfo();
}

