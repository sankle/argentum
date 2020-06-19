#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <array>
#include <fstream>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/paths.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player : public Unit {
   private:
    // Stats y atributos
    uint32_t health, mana, gold; /* stats */

    // Inventario y equipameniento
    std::array<InventorySlot, N_INVENTORY_SLOTS> inventory;
    std::array<Id, N_WEARABLE_ITEMS> equipment;

    // Cuerpo y vestimenta
    Id head_id, body_id;
    Id helmet_id, armour_id, shield_id, weapon_id;

    /* Copia la data desde el paquete recibido */
    void _copyData(const PlayerData& init_data);

   public:
    /* Constructor */
    Player(Renderer* renderer, UnitSpriteContainer* sprites);

    /* Deshabilitamos el constructor por copia. */
    Player(const Player&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Player& operator=(const Player&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Player(Player&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Player& operator=(Player&& other) = delete;

    //-------------------------------------------------------------------------

    /* Se inicializa con el primer paquete del server */
    void init(const PlayerData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const PlayerData& updated_data);

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const override;

    /* Obtiene la posición en tiles */
    SDL_Point getPos() const;

    /* Obtiene la posición y dimensiones en pixeles */
    SDL_Rect getBox() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
