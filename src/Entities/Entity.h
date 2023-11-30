// Created by Anderson on 11/28/2023.

/**
 * @file Entity.h
 * @brief Declaration of the Entity class and related structures for representing entities in a 2D space.
 */

#ifndef CASTR_ENTITY_H
#define CASTR_ENTITY_H

#include "../Vector/Vector.h"

/**
 * @brief Structure representing position information in 2D space, including position, direction, and plane vectors.
 */
struct PositionInfo2D {
    vDouble2d pos; /**< Position vector in 2D space. */
    vDouble2d dir; /**< Direction vector in 2D space. */
    vDouble2d plane; /**< Plane vector in 2D space. */
};

/**
 * @brief Class representing an entity in a 2D space with position and movement information.
 */
class Entity {
protected:
    PositionInfo2D positionInfo; /**< Position information of the entity in 2D space. */
    double moveSpeed = 0.002; /**< Movement speed of the entity. */
    double rotationSpeed = 0.002; /**< Rotation speed of the entity. */

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

#endif //CASTR_ENTITY_H
