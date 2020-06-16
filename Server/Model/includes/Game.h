#ifndef __GAME_H__
#define __GAME_H__
//-----------------------------------------------------------------------------
#include <unordered_map>
#include <cstdint>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
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
        std::unordered_map<int, Character> characters;
        // std::unordered_map<int, Creatures> creatures; Falta implementar
        //-----------------------------------------------------------------------------
        int next_instance_id;

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
         * Recibe un struct CharacterPersistenceCfg con toda la información persistida
         * del character, o bien la información necesaria para crear un nuevo
         * character.
         * 
         * Retorna el id único de instancia de dicho character, mediante el cual
         * se interactuará con el mismo.
         * 
         * Lanza Exception si alguno de los id no mapea a ninguna raza/clase.
         */
        const int newCharacter(CharacterPersistenceCfg& init_data);

        /*
        * Llamar a este metodo ante la desconexión de un character.
        * 
        * Recibe el id de instancia del character a eliminar.
        * Lo persiste, y luego lo elimina del juego.
        * 
        * Lanza Exception si el id especificado no corresponde a ningún
        * character en el juego.
        */
        void deleteCharacter(const int id);
        //-----------------------------------------------------------------------------
        void startMovingUp();
        void startMovingDown();
        void startMovingLeft();
        void startMovingRight();

        void stopMoving();

        void useWeapon(const uint32_t x_coord, const uint32_t y_coord);

        void equip(const uint8_t n_slot);

        void meditate();

        void resurrect();

        void list(const uint32_t x_coord, const uint32_t y_coord);

        void depositItemOnBank(const uint32_t x_coord, const uint32_t y_coord,
                               const uint8_t n_slot, uint32_t amount);
        void withdrawItemFromBank(const uint32_t x_coord, const uint32_t y_coord,
                                  const uint32_t item_id, const uint32_t amount);

        void depositGoldOnBank(const uint32_t x_coord, const uint32_t y_coord,
                               const uint32_t amount);
        void withdrawGoldFromBank(const uint32_t x_coord, const uint32_t y_coord,
                                  const uint32_t amount);
        
        void buyItem(const uint32_t x_coord, const uint32_t y_coord,
                     const uint32_t item_id, const uint32_t amount);
        void sellItem(const uint32_t x_coord, const uint32_t y_coord,
                      const uint8_t n_slot, const uint32_t amount);
        
        void take();
        void drop(const uint8_t n_slot, const uint32_t amount);

        void listConnectedPlayers();
        
        
        
};
//-----------------------------------------------------------------------------
#endif  // __GAME_H__
