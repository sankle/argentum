#ifndef __MAP_PARSER_H__
#define __MAP_PARSER_H__

//-----------------------------------------------------------------------------
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Librería de json a utilizar (https://github.com/nlohmann/json).
#include "json.hpp"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __ID__
#define __ID__
typedef int Id;
#endif  // __ID__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapParser {
   private:
    // metodos privados

   public:
    /* Constructor */
    MapParser();

    /* Deshabilitamos el constructor por copia. */
    MapParser(const MapParser&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    MapParser& operator=(const MapParser&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    MapParser(MapParser&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    MapParser& operator=(MapParser&& other) = delete;

    //-------------------------------------------------------------------------

    /* Metodo */
    void loadMaps() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapParser();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_PARSER_H__
