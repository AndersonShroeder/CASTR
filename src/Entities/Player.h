// Created by Anderson on 11/28/2023.

/**
 * @file Player.h
 * @brief Declaration of the Player class, derived from InputReader and Entity2D, for representing a player entity.
 */

#ifndef CASTR_PLAYER_H
#define CASTR_PLAYER_H

#include "../GameState/InputReader.h"
#include "Entity.h"

namespace Entities {

    /**
     * @class Player
     * @brief Represents a player entity with input reading capabilities.
     *
     * The Player class is derived from both InputReader and Entity, allowing it to read input
     * and have a 2D entity representation in the game.
     */
    class Player : public GameState::InputReader, public Entity {};

    /**
     * @class Player2D
     * @brief Represents a 2D player entity with input reading capabilities.
     *
     * The Player2D class is a specialization of the Player class, providing specific input reading
     * functionality for 2D entities.
     */
    class Player2D : public Player {
    public:
        /**
         * @brief Reads input for the 2D player entity.
         */
        void readInput() override;
    };

    /**
     * @class Player3D
     * @brief Represents a 3D player entity with input reading capabilities.
     *
     * The Player3D class is a specialization of the Player class, providing specific input reading
     * functionality for 3D entities.
     */
    class Player3D : public Player {
    public:
        /**
         * @brief Reads input for the 3D player entity.
         */
        void readInput() override;
    };

} // namespace Entities

#endif //CASTR_PLAYER_H
