// Created by Anderson on 11/28/2023.

/**
 * @file Entity.h
 * @brief Declaration of the Entity class and related structures for representing entities in a 2D space.
 */

#ifndef CASTR_ENTITY_H
#define CASTR_ENTITY_H

#include "../Util//Vector.h"

namespace Entities {
/**
 * @brief Structure representing position information in 2D space, including position, direction, and plane vectors.
 */
    struct PositionInfo2D {
        vDouble2d pos; /**< Position vector in 2D space. */
        vDouble2d dir; /**< Direction vector in 2D space. */
        vDouble2d plane; /**< Plane vector in 2D space. */
    };

    struct PositionInfo3D {
        vDouble3d pos; /**< Position vector in 2D space. */
        double angle;
        int upDown;
    };

/**
 * @brief Class representing an entity in a 2D space with position and movement information.
 */
    class Entity {
    protected:
        PositionInfo2D positionInfo; /**< Position information of the entity in 2D space. */
        double moveSpeed = 0.02; /**< Movement speed of the entity. */
        double rotationSpeed = 0.02; /**< Rotation speed of the entity. */

    public:
        /**
         * @brief Getter for the position information of the entity.
         * @return A reference to the PositionInfo2D structure containing position, direction, and plane vectors.
         */
        PositionInfo2D &getPositionInfo();

        /**
         * @brief Updates the position information of the entity.
         * @param positionInfo The new position information to set.
         */
        void updatePositionInfo(PositionInfo2D positionInfo);
    };

    class Entity3D {
    protected:
        PositionInfo3D positionInfo3D;

    public:
        PositionInfo3D &getPositionInfo();
        void updatePositionInfo(PositionInfo3D positionInfo);
    };
}
#endif //CASTR_ENTITY_H
