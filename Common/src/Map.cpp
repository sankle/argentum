#include "../includes/Map.h"

#include "../includes/RandomNumberGenerator.h"

#define MAX_FREE_ITEM_TILE_SEARCHING_STEP 30
#define SAFE_ZONE_MAX_SEARCHING_STEP 100
//-----------------------------------------------------------------------------
// Métodos privados

void Map::_checkIfValid(const json& map) const {
    if ((TILE_WIDTH != map["tilewidth"]) ||
        (TILE_HEIGHT != map["tileheight"])) {
        throw Exception("Invalid map file. Tiles must be %i x %i.", TILE_WIDTH,
                        TILE_HEIGHT);
    }

    if (map["layers"].size() != EXPECTED_LAYERS) {
        throw Exception(
            "Invalid map file. File must contain these map layers: ground1, "
            "ground2, decoration, roof, safezones, collision, indoor, and "
            "portal layer.");
    }

    for (int i = 0; i < EXPECTED_LAYERS; i++) {
        if ((map["layers"][i]["width"] != w) ||
            (map["layers"][i]["height"] != h)) {
            throw Exception(
                "Invalid map file. All layers must have the same dimensions.");
        }
    }
}

void Map::_initLayers() {
    // Reservamos espacio para la grilla de tiles lógicos
    tiles.reserve(w * h);

    // Reservamos el espacio para nuestros objetos estáticos
    decoration_ly.reserve(h);
    roof_ly.reserve(h);
    npc_ly.reserve(h);

    // Iniciamos cada capa
    for (int i = 0; i < h; i++) {
        decoration_ly.emplace_back();
        roof_ly.emplace_back();
        npc_ly.emplace_back();
    }
}

void Map::_fillTiles(const json& map, const json& tilesets) {
    Tile tile;
    size_t i = 0;
    for (size_t row = 0; row < h; row++) {
        for (size_t col = 0; col < w; col++) {
            // Calculamos el índice actual
            i = (row * w) + col;

            // Ids gráficos (tiles gráficos)
            tile.ground_1_id = map["layers"][GROUND1_LAYER]["data"][i];
            tile.ground_2_id = map["layers"][GROUND2_LAYER]["data"][i];

            tile.decoration_id = map["layers"][DECORATION_LAYER]["data"][i];
            if (tile.decoration_id) {
                decoration_ly[row].emplace_back(col, tile.decoration_id);
            }

            tile.roof_id = map["layers"][ROOF_LAYER]["data"][i];
            if (tile.roof_id) {
                roof_ly[row].emplace_back(col, tile.roof_id);
            }

            tile.npc_id = map["layers"][NPC_LAYER]["data"][i];
            if (tile.npc_id) {
                npc_ly[row].emplace_back(col, tile.npc_id);
            }

            // Metadata (tiles lógicos)
            int safe_zone = map["layers"][SAFEZONE_LAYER]["data"][i];
            tile.safe_zone = (bool)safe_zone;
            int collision = map["layers"][COLLISION_LAYER]["data"][i];
            tile.collision = (bool)collision;
            int indoor = map["layers"][INDOOR_LAYER]["data"][i];
            tile.indoor = (bool)indoor;
            int portal = map["layers"][PORTAL_LAYER]["data"][i];
            tile.portal = (bool)portal;

            if (tile.npc_id > 0) {
                /* Debemos calcular el id global del npc */
                size_t npc_gid = tile.npc_id;
                size_t tileset_first_gid = tilesets["npcs"]["first_gid"];
                size_t npc_lid = npc_gid - tileset_first_gid;
                std::string tileset_filepath = tilesets["npcs"]["filepath"];
                tileset_filepath = paths::asset(tileset_filepath.c_str());
                json tileset = JSON::loadJsonFile(tileset_filepath);
                tile.npc = tileset["tiles"][npc_lid]["properties"][0]["value"];
            } else {
                tile.npc = 0;
            }

            // Ocupantes del tile
            tile.occupant_id = 0;
            tile.item_id = 0;
            tile.item_amount = 0;

            tiles.push_back(tile);
        }
    }
}

Tile& Map::_getTile(const int x, const int y) {
    if (!_isValid(x, y)) {
        throw Exception("Invalid map coordinates. x = %i, y = %i\n", x, y);
    }

    return tiles[TILENUMBER(x, y)];
}

Tile& Map::_getTileWithoutChecks(const int x, const int y) {
    return tiles[TILENUMBER(x, y)];
}

const bool Map::_moveOccupant(Tile& from_tile, Tile& to_tile,
                              bool is_creature) {
    if (to_tile.collision || to_tile.occupant_id || to_tile.npc_id) {
        // El tile está ocupado / hay colisión.
        return false;
    }
    if (is_creature && to_tile.safe_zone) {
        return false;
    }

    // Se puede mover.
    to_tile.occupant_id = from_tile.occupant_id;
    from_tile.occupant_id = 0;

    return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Map::Map() {}

void Map::init(const json& map, const json& tilesets,
               std::vector<Id>& creatures, std::string& name) {
    w = map["width"];
    h = map["height"];
    this->creatures = creatures;
    this->name = name;

    _checkIfValid(map);
    _initLayers();
    _fillTiles(map, tilesets);
}

//-----------------------------------------------------------------------------
// Escritura

TileId& Map::getNPC(const int x, const int y) {
    Tile& tile = _getTile(x, y);
    return tile.npc_id;
}

TileId& Map::getPortal(int x, int y) {
    // Hay que llegar a la celda inferior izquierda del portal
    // Primero descendemos
    while (_getTile(x, y).portal) {
        y++;
    }
    y--;

    // Ahora vamos a la izquierda
    while (_getTile(x, y).portal) {
        x--;
    }
    x++;

    return _getTile(x, y).decoration_id;
}

const bool Map::moveOccupant(const int x, const int y,
                             const Orientation& orientation, bool is_creature) {
    Tile& from_tile = _getTile(x, y);

    if (orientation == UP_ORIENTATION) {
        if (y == 0) {
            // Limite superior alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x, y - 1);
        return _moveOccupant(from_tile, to_tile, is_creature);
    }

    if (orientation == DOWN_ORIENTATION) {
        if (y + 1 == this->h) {
            // Limite inferior alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x, y + 1);
        return _moveOccupant(from_tile, to_tile, is_creature);
    }

    if (orientation == LEFT_ORIENTATION) {
        if (x == 0) {
            // Limite izquierdo alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x - 1, y);
        return _moveOccupant(from_tile, to_tile, is_creature);
    }

    if (x + 1 == this->w) {
        // Limite derecho alcanzado.
        return false;
    }
    Tile& to_tile = _getTile(x + 1, y);
    return _moveOccupant(from_tile, to_tile, is_creature);
}

void Map::establishEntitySpawningPosition(int& x, int& y, bool is_creature) {
    bool valid_position = false;
    RandomNumberGenerator gen;
    int step = 0;
    while (!valid_position) {
        x = gen(0, this->w - 1);
        y = gen(0, this->h - 1);

        const Tile& tile = this->getTile(x, y);

        if (!tile.collision && !tile.npc_id && !tile.occupant_id) {
            if (is_creature && tile.safe_zone &&
                step < SAFE_ZONE_MAX_SEARCHING_STEP) {
                ++step;
                continue;
            }
            if (!is_creature && !tile.safe_zone &&
                step < SAFE_ZONE_MAX_SEARCHING_STEP) {
                ++step;
                continue;
            }

            valid_position = true;
        }

        ++step;
    }
}

void Map::occupyTile(InstanceId id, const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.occupant_id = id;
}

void Map::setItem(const Id item_id, const uint32_t amount, const int x,
                  const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.item_id = item_id;
    tile.item_amount = amount;
}

void Map::addItem(const Id item_id, const uint32_t amount, int& x, int& y) {
    this->getNearestFreeTile(x, y, true);

    Tile& tile = this->_getTile(x, y);
    tile.item_id = item_id;
    tile.item_amount = amount;
}

const bool Map::isSafeZone(const int x, const int y) const {
    return this->getTile(x, y).safe_zone;
}

void Map::clearTileOccupant(const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.occupant_id = 0;
}

void Map::clearTileItem(const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.item_id = 0;
    tile.item_amount = 0;
}

void Map::clear() {
    for (size_t row = 0; row < (size_t)h; row++) {
        for (size_t col = 0; col < (size_t)w; col++) {
            Tile& tile = _getTileWithoutChecks(col, row);
            tile.item_amount = 0;
            tile.item_id = 0;
            tile.occupant_id = 0;
        }
    }
}

//-----------------------------------------------------------------------------
// Lectura

int Map::getWidthTiles() const {
    return w;
}

int Map::getHeightTiles() const {
    return h;
}

bool Map::_isValid(const int x, const int y) const {
    if ((x >= w) || (x < 0) || (y >= h) || (y < 0)) {
        return false;
    } else {
        return true;
    }
}

const Tile& Map::getTile(const int x, const int y) const {
    if (!_isValid(x, y)) {
        throw Exception("Invalid map coordinates. x = %i, y = %i\n", x, y);
    }

    return tiles[TILENUMBER(x, y)];
}

const Tile& Map::getTileWithoutChecks(const int x, const int y) const {
    return tiles[TILENUMBER(x, y)];
}

bool Map::isPositionValidForCreature(const int x, const int y) const {
    const Tile& tile = getTile(x, y);
    if (tile.collision || tile.occupant_id || tile.npc_id || tile.safe_zone) {
        return false;
    }
    return true;
}

void Map::getNearestFreeTile(int& x, int& y, const bool is_for_item) const {
    // Primero nos fijamos si está libre el mismo tile en (x, y)
    const Tile& current_tile = this->getTile(x, y);
    if (is_for_item) {
        if (!current_tile.item_id && !current_tile.collision &&
            !current_tile.npc_id) {
            return;
        }
    } else {
        if (!current_tile.item_id && !current_tile.collision &&
            !current_tile.npc_id && !current_tile.occupant_id) {
            return;
        }
    }

    bool empty_tile_found = false;

    int step = 1;
    int _x = 0;
    int _y = 0;

    while (!empty_tile_found) {
        for (int x_inc = -1; x_inc <= 1; ++x_inc) {
            _x = (x + x_inc * step);

            for (int y_inc = -1; y_inc <= 1; ++y_inc) {
                _y = (y + y_inc * step);

                if (_x < 0 || _y < 0 || _x > this->w - 1 || _y > this->h - 1)
                    continue;

                const Tile& tile = this->getTile(_x, _y);

                if (is_for_item) {
                    if (!tile.item_id && !tile.collision && !tile.npc_id) {
                        empty_tile_found = true;
                        x = _x;
                        y = _y;
                        return;
                    }
                } else {
                    if (!tile.item_id && !tile.collision && !tile.npc_id &&
                        !tile.occupant_id) {
                        empty_tile_found = true;
                        x = _x;
                        y = _y;
                        return;
                    }
                }
            }
        }
        ++step;
        if (step == MAX_FREE_ITEM_TILE_SEARCHING_STEP)
            throw(CouldNotFindFreeTileException());
    }
}

const std::vector<Id>& Map::getCreatures() const {
    return creatures;
}

const std::string& Map::getMapName() const {
    return name;
}

//-----------------------------------------------------------------------------

Map::~Map() {}

const char* CouldNotFindFreeTileException::what() const noexcept {
    return "No se pudieron dropear algunos items.";
}

//-----------------------------------------------------------------------------
