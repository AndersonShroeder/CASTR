// Created by Anderson on 11/28/2023.

/**
 * @file Player.h
 * @brief Declaration of the Player class, derived from InputReader and Entity, for representing a player entity.
 */

#ifndef CASTR_PLAYER_H
#define CASTR_PLAYER_H

#include "../GameState/InputReader.h"
#include "Entity.h"
namespace Entities {
/**
 * @brief Class representing a player entity, derived from InputReader and Entity.
 */
    class Player : public GameState::InputReader, public Entity {
    public:
        /**
         * @brief Implementation of the readInput function for handling player input.
         */
        void readInput() override;
    };
}
#endif //CASTR_PLAYER_H
