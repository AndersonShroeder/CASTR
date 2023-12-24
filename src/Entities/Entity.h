// Created by Anderson on 11/28/2023.

/**
 * @file Entity2D.h
 * @brief Declaration of the Entity2D class and related structures for representing entities in a 2D space.
 */

#ifndef CASTR_ENTITY_H
#define CASTR_ENTITY_H

#include "../Util//Vector.h"

namespace Entities {

    /**
     * @struct PositionInfo
     * @brief Structure to store positional information of a 2D entity, including position, direction, and orientation.
     */
    struct PositionInfo {
        vDouble3d pos, dir, plane;
        int angle{0}, upDown{0};
    };

    /**
     * @class Entity
     * @brief Base class for 2D entities providing methods to access and update positional information.
     */
    class Entity {
    public:
        /**
         * @brief Gets the PositionInfo of the entity.
         * @return Reference to the PositionInfo structure.
         */
        PositionInfo& getPositionInfo();

        /**
         * @brief Updates the PositionInfo of the entity.
         * @param data The new PositionInfo data to set.
         */
        void updatePositionInfo(PositionInfo data);

    protected:
        PositionInfo positionInfo; ///< Positional information of the entity.
        double moveSpeed{.02};      ///< Speed of movement.
        double rotationSpeed{.02};  ///< Speed of rotation.
    };
} // namespace Entities

#endif //CASTR_ENTITY_H
