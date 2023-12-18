// Implementation of the Entity class methods

#include "Entity.h"

namespace Entities {
    PositionInfo &Entity::getPositionInfo() {
        return this->positionInfo;
    }

    void Entity::updatePositionInfo(PositionInfo data) {
        this->positionInfo = data;
    }

} // namespace Entities
