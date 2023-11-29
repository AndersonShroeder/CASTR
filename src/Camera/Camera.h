//
// Created by Anderson on 11/28/2023.
//
#include "../Entities/Entity.h"

#ifndef CASTR_CAMERA_H
#define CASTR_CAMERA_H


class Camera {
    PositionInfo positionInfo;
    Entity subscribed;

public:
    void subscribe(Entity &entity);
    void visitSubscribed();
};


#endif //CASTR_CAMERA_H
