#ifndef __CHARACTER_H__
#define __CHARACTER_H__

//-----------------------------------------------------------------------------
#include <cstdint>

#include "../../../Common/includes/Exceptions/Exception.h"
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Character : public Unit {
   private:
    // Cuerpo y vestimenta
    Id head_id, body_id;
    Id helmet_id, armour_id, shield_id, weapon_id;

    /* Copia la data desde el paquete recibido */
    void _copyData(const CharacterData& init_data);

   public:
    /* Constructor */
    Character(Renderer* renderer, UnitSpriteContainer* sprites,
              const int tile_w, const int tile_h,
              const float tile_movement_time);

    /* Deshabilitamos el constructor por copia. */
    Character(const Character&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Character& operator=(const Character&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Character(Character&& other);

    /* Habilitamos el operador= para movimiento. */
    Character& operator=(Character&& other);

    //-------------------------------------------------------------------------

    /* Se inicializa con el primer paquete del server */
    void init(const CharacterData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const CharacterData& updated_data);

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Character();
};

//-----------------------------------------------------------------------------

#endif  // __CHARACTER_H__
