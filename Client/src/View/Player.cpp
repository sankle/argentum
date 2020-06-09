#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Player::_setScaleFactor() {
    int body_w = g_sprites->get(data.body_id).clip_w;
    if (body_w > tile_w) {
        scale_factor = tile_w / body_w;
    } else {
        scale_factor = 1.0;
    }
}

void Player::_render(const Sprite& sprite) const {
    /** Pasos para renderizar cada componente:
     *
     * 1. Obtener (x, y) centrado en el tile aplicando scale_factor.
     * 2. Agregar el offset (x, y) del player.
     * 3. Renderizar aplicando scale_factor.
     */

    int x, y;

    // paso 1
    x = (tile_w - (sprite.clip_w * scale_factor)) / 2;
    y = (tile_h * (0.8)) - (sprite.clip_h * scale_factor);

    // paso 2
    x += this->x;
    y += this->y;

    // paso 3
    SDL_Rect render_quad, render_clip;
    SDL_Texture* texture;
    render_quad = {x, y, (int)(sprite.clip_w * scale_factor),
                   (int)(sprite.clip_h * scale_factor)};
    render_clip = {0, 0, sprite.clip_w, sprite.clip_h};
    texture = sprite.texture.getTexture();
    g_renderer->renderIfVisible(texture, &render_quad, &render_clip);
}

void Player::_startMovementIfNeeded() {
    /* si el x no concuerda con data.x_tile * tile_w, o lo mismo con y, mover */
}

// OLD API --------------------------------------------------------------------

/*
void Player::_centerOnTile() {
    // x = (data.x_tile * tile_w) + ((tile_w - w) / 2);
    // y = (data.y_tile * tile_h) + (tile_h * (0.8)) - h;
}

int Player::_xValueToReach() const {
    return (data.x_tile * tile_w) + ((tile_w - box.w) / 2);
}

int Player::_yValueToReach() const {
    return (data.y_tile * tile_h) + (tile_h * (0.8)) - box.h;
}
*/

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Player::Player(Renderer* renderer, UnitSpriteContainer* sprites)
    : g_renderer(renderer), g_sprites(sprites) {}

Player::Player(Player&& other) {
    /* Nos apropiamos de los punteros */
    g_renderer = other.g_renderer;
    g_sprites = other.g_sprites;
    other.g_renderer = NULL;
    other.g_sprites = NULL;

    /* Copiamos el resto de atributos */
    data = other.data;
    x = other.x;
    y = other.y;
    scale_factor = other.scale_factor;
}

Player& Player::operator=(Player&& other) {
    /* Nos apropiamos de los punteros */
    g_renderer = other.g_renderer;
    g_sprites = other.g_sprites;
    other.g_renderer = NULL;
    other.g_sprites = NULL;

    /* Copiamos el resto de atributos */
    data = other.data;
    x = other.x;
    y = other.y;
    scale_factor = other.scale_factor;

    return *this;
}

void Player::init(const PlayerData& init_data) {
    data = init_data;
    json map_data = JSON::loadJsonFile(MAPS_FILEPATH);
    tile_w = map_data["tilewidth"];
    tile_h = map_data["tileheight"];
    x = tile_w * data.x_tile;
    y = tile_h * data.y_tile;
    _setScaleFactor();
}

void Player::update(const PlayerData& updated_data) {
    data = updated_data;
    _setScaleFactor();
    _startMovementIfNeeded();
}

void Player::act() {}

void Player::render() const {
    fprintf(stderr, "x_tile = %i, y_tile = %i, x = %i, y = %i\n", data.x_tile,
            data.y_tile, x, y);

    // Cuerpo
    _render(g_sprites->get(data.body_id));

    // Cabeza
    _render(g_sprites->get(data.head_id));
}

Player::~Player() {}

//-----------------------------------------------------------------------------
