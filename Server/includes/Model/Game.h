#ifndef __GAME_H__
#define __GAME_H__
//-----------------------------------------------------------------------------
#include <unordered_map>
#include <cstdint>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "../Control/NotificationReply.h"
//-----------------------------------------------------------------------------
#include "Config.h"
#include "config_structs.h"
#include "Character.h"
#include "ItemsContainer.h"
//-----------------------------------------------------------------------------
class Game {
    private:
        //-----------------------------------------------------------------------------
        // Game components configuration files:
        //-----------------------------------------------------------------------------
        Config<RaceCfg> races;
        Config<KindCfg> kinds;
        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        // Game entities
        //-----------------------------------------------------------------------------
        MapContainer map_container;
        ItemsContainer items;
        std::unordered_map<InstanceId, Character> characters;
        // std::unordered_map<int, Creatures> creatures; Falta implementar
        //-----------------------------------------------------------------------------
        InstanceId next_instance_id;

    public:
        //-----------------------------------------------------------------------------
        Game();
        ~Game();

        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;
        Game(Game&& other) = delete;
        Game& operator=(Game&& other) = delete;
        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        // DEFINIR COMO VIENE EL PLAYERDATA SI ES NUEVO!.
        /*
         * Recibe un struct CharacterCfg con toda la información persistida
         * del character, o bien la información necesaria para crear un nuevo
         * character.
         * 
         * Retorna el id único de instancia de dicho character, mediante el cual
         * se interactuará con el mismo.
         * 
         * Lanza Exception si alguno de los id no mapea a ninguna raza/clase.
         */
        const int newCharacter(CharacterCfg& init_data);

        /*
        * Llamar a este metodo ante la desconexión de un character.
        * 
        * Recibe el id de instancia del character a eliminar.
        * Lo persiste, y luego lo elimina del juego.
        * 
        * Lanza Exception si el id especificado no corresponde a ningún
        * character en el juego.
        */
        void deleteCharacter(const InstanceId id);
        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        // Actualización del loop
        //-----------------------------------------------------------------------------
        
        NotificationReply* actCharacters(const int it);

        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        NotificationReply* startMovingUp(const Id caller);
        NotificationReply* startMovingDown(const Id caller);
        NotificationReply* startMovingLeft(const Id caller);
        NotificationReply* startMovingRight(const Id caller);

        NotificationReply* stopMoving(const Id caller);

        NotificationReply* useWeapon(const Id caller, 
                       const uint32_t x_coord, const uint32_t y_coord);

        NotificationReply* equip(const Id caller, const uint8_t n_slot);

        NotificationReply* meditate(const Id caller);

        NotificationReply* resurrect(const Id caller);

        NotificationReply* list(const Id caller, const uint32_t x_coord, const uint32_t y_coord);

        NotificationReply* depositItemOnBank(const Id caller, 
                               const uint32_t x_coord, const uint32_t y_coord,
                               const uint8_t n_slot, uint32_t amount);
        NotificationReply* withdrawItemFromBank(const Id caller, 
                                  const uint32_t x_coord, const uint32_t y_coord,
                                  const uint32_t item_id, const uint32_t amount);

        NotificationReply* depositGoldOnBank(const Id caller, 
                               const uint32_t x_coord, const uint32_t y_coord,
                               const uint32_t amount);
        NotificationReply* withdrawGoldFromBank(const Id caller, 
                                  const uint32_t x_coord, const uint32_t y_coord,
                                  const uint32_t amount);
        
        NotificationReply* buyItem(const Id caller, 
                     const uint32_t x_coord, const uint32_t y_coord,
                     const uint32_t item_id, const uint32_t amount);
        NotificationReply* sellItem(const Id caller, 
                      const uint32_t x_coord, const uint32_t y_coord,
                      const uint8_t n_slot, const uint32_t amount);
        
        NotificationReply* take(const Id caller);
        NotificationReply* drop(const Id caller, const uint8_t n_slot, const uint32_t amount);

        NotificationReply* listConnectedPlayers(const Id caller);
};
//-----------------------------------------------------------------------------
#endif  // __GAME_H__