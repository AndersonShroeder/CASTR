// Created by Anderson on 11/28/2023.

/**
 * @file Camera.h
 * @brief Declaration of the Camera class for representing a camera in a 2D space.
 */

#include "Entity.h"

#ifndef CASTR_CAMERA_H
#define CASTR_CAMERA_H

namespace Entities {
/**
 * @brief Class representing a camera in a 2D space.
 */
    class Camera {
        PositionInfo2D *positionInfo; /**< Pointer to the position information of the camera. */

    public:
        /**
         * @brief Subscribes the camera to an entity, allowing it to track the entity's position information.
         * @param entity Reference to the entity to which the camera subscribes.
         */
        void subscribe(Entity &entity);

        /**
         * @brief Getter for the position information of the camera.
         * @return PositionInfo2D structure containing position, direction, and plane vectors.
         */
        PositionInfo2D getPositionInfo();
    };
}
#endif //CASTR_CAMERA_H
