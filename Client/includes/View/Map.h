#ifndef __MAP_H__
#define __MAP_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <fstream>
#include <string>
#include <vector>

#include "../../../Common/includes/Exceptions/SDLException.h"
#include "Renderer.h"
#include "Texture.h"
#include "Tile.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Harcodeo el mapa. Esto se deberá procesar de algún archivo.

#define TILE_TEXTURES 3
#define MAP_WIDTH 1536
#define MAP_HEIGHT 1536
#define MAP_X_TILES (MAP_WIDTH) / (TILE_WIDTH)
#define MAP_Y_TILES (MAP_HEIGHT) / (TILE_HEIGHT)
#define MAP_TOTAL_TILES (MAP_X_TILES) * (MAP_Y_TILES)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapProxy {
   private:
    const Renderer* g_renderer;
    std::vector<Texture> tile_textures;
    std::vector<Tile> tiles;

    /* Crea las texturas */
    void _createTextures();

    /* Carga las texturas de los tiles */
    void _loadTextures();

    /* Crea los tiles con sus tipos (parsea el mapa) */
    void _loadTiles();

   public:
    /* Constructor */
    MapProxy(const Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    MapProxy(const MapProxy&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    MapProxy& operator=(const MapProxy&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    MapProxy(MapProxy&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    MapProxy& operator=(MapProxy&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga los archivos necesarios */
    void loadMedia();

    /* Renderiza el mapa dentro de la cámara */
    void render() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapProxy();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_H__