// Created by Anderson on 11/28/2023.

/**
 * @file Player.h
 * @brief Declaration of the Player class, derived from InputReader and Entity, for representing a player entity.
 */

#ifndef CASTR_PLAYER_H
#define CASTR_PLAYER_H

#include "../Input/InputReader.h"
#include "Entity.h"

/**
 * @brief Class representing a player entity, derived from InputReader and Entity.
 */
class Player : public InputReader, public Entity {
public:
    /**
     * @brief Implementation of the readInput function for handling player input.
     */
    void readInput() override;
};

#endif //CASTR_PLAYER_H
