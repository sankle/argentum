#include "../../includes/Model/Position.h"

#include <cstdlib>

#include "../../../Common/includes/Exceptions/Exception.h"

#define DEFAULT_ORIENTATION DOWN_ORIENTATION

Position::Position(const Id map, const int init_x_coord, const int init_y_coord,
                   MapContainer& map_container)
    : map(map),
      x(init_x_coord),
      y(init_y_coord),
      orientation(DEFAULT_ORIENTATION),
      map_container(map_container),
      broadcast(false) {}

Position::~Position() {
    map_container[map].clearTileOccupant(x, y);
}

const int Position::getX() const {
    return this->x;
}

const int Position::getY() const {
    return this->y;
}

const Orientation Position::getOrientation() const {
    return this->orientation;
}

const Id Position::getMapId() const {
    return this->map;
}

const unsigned int Position::getDistance(const Position& other) const {
    return getDistance(other.x, other.y);
}

const unsigned int Position::getDistance(const int other_x,
                                         const int other_y) const {
    return abs(this->x - other_x) + abs(this->y - other_y);
}

const unsigned int Position::getRange(const Position& other) const {
    return std::max(abs(this->x - other.getX()), abs(this->y - other.getY()));
}

void Position::move(bool is_creature) {
    if (!this->map_container[this->map].moveOccupant(
            this->x, this->y, this->orientation, is_creature))
        throw CollisionWhileMovingException();

    switch (this->orientation) {
        case UP_ORIENTATION:
            this->y -= 1;
            break;

        case DOWN_ORIENTATION:
            this->y += 1;
            break;

        case RIGHT_ORIENTATION:
            this->x += 1;
            break;

        case LEFT_ORIENTATION:
            this->x -= 1;
            break;

        default:
            throw Exception("Position::move: unknown orientation.");
    }

    this->broadcast = true;
}

void Position::changeOrientation(Orientation orientation) {
    this->orientation = orientation;
}

void Position::changePosition(int x, int y) {
    try {
        this->map_container[this->map].getNearestFreeTile(x, y);
    } catch (const CouldNotFindFreeTileException& e) {
        // No cambio mi posición.
        return;
    }
    
    this->map_container[this->map].swapTileOcuppant(this->x, this->y, x, y);
    this->x = x;
    this->y = y;
}

const bool Position::isInSafeZone() const {
    return map_container[map].isSafeZone(x, y);
}

const bool Position::mustBeBroadcasted() const {
    return this->broadcast;
}

void Position::beBroadcasted() {
    this->broadcast = false;
}

void Position::fillBroadcastData(UnitData& data) const {
    data.map = this->map;
    data.x_tile = this->x;
    data.y_tile = this->y;
    data.orientation = this->orientation;
}

void Position::fillPersistenceData(CharacterCfg& data) const {
    data.map = this->map;
    data.x_tile = this->x;
    data.y_tile = this->y;
}

const char* CollisionWhileMovingException::what() const noexcept {
    return "No puedes moverte en esa dirección.";
}
