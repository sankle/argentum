#include "../../includes/View/Console.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Console::Console(const Renderer* renderer) : HUDComponent(renderer) {}

void Console::init(const json& config) {
    if (initialized) {
        throw Exception("Console already initialized.");
    }

    offset_x = config["offset"]["x"];
    offset_y = config["offset"]["y"];
    w = config["w"];
    h = config["h"];

    initialized = true;
}

void Console::loadMedia() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    // Cargar media
}

void Console::render() const {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    // Renderizar

    /*
    SDL_Rect render_quad = {offset_x, offset_y, w, h};
    g_renderer->render(texture.getTexture(), &render_quad);
    */
}

Console::~Console() {}

//-----------------------------------------------------------------------------
