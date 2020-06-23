#ifndef __GAME_H__
#define __GAME_H__
//-----------------------------------------------------------------------------
#include <cstdint>
#include <unordered_map>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "../../includes/Control/Notification.h"
//-----------------------------------------------------------------------------
#include "Character.h"
#include "Config.h"
#include "ItemsContainer.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

// Forward declaration
class ActiveClients;

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

    ActiveClients& active_clients;

   public:
    //-----------------------------------------------------------------------------
    Game(ActiveClients& active_clients);
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&& other) = delete;
    Game& operator=(Game&& other) = delete;
    //-----------------------------------------------------------------------------

    Notification* _buildBroadcast(InstanceId id, BroadcastType broadcast_type,
                          EntityType entity_type);

    void _pushCharacterDifferentialBroadcast(InstanceId id, BroadcastType type,
                                             bool send_to_caller);
    void _pushFullBroadcast(InstanceId receiver, bool is_new_connection);

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

    /* método provisorio */
    void broadcastNewCharacter(InstanceId id);

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

    void actCharacters(const int it);

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    void startMovingUp(const Id caller);
    void startMovingDown(const Id caller);
    void startMovingLeft(const Id caller);
    void startMovingRight(const Id caller);

    void stopMoving(const Id caller);

    void useWeapon(const Id caller, const uint32_t x_coord,
                   const uint32_t y_coord);

    void equip(const Id caller, const uint8_t n_slot);

    void meditate(const Id caller);

    void resurrect(const Id caller);

    void list(const Id caller, const uint32_t x_coord, const uint32_t y_coord);

    void depositItemOnBank(const Id caller, const uint32_t x_coord,
                           const uint32_t y_coord, const uint8_t n_slot,
                           uint32_t amount);
    void withdrawItemFromBank(const Id caller, const uint32_t x_coord,
                              const uint32_t y_coord, const uint32_t item_id,
                              const uint32_t amount);

    void depositGoldOnBank(const Id caller, const uint32_t x_coord,
                           const uint32_t y_coord, const uint32_t amount);
    void withdrawGoldFromBank(const Id caller, const uint32_t x_coord,
                              const uint32_t y_coord, const uint32_t amount);

    void buyItem(const Id caller, const uint32_t x_coord,
                 const uint32_t y_coord, const uint32_t item_id,
                 const uint32_t amount);
    void sellItem(const Id caller, const uint32_t x_coord,
                  const uint32_t y_coord, const uint8_t n_slot,
                  const uint32_t amount);

    void take(const Id caller);
    void drop(const Id caller, const uint8_t n_slot, const uint32_t amount);

    void listConnectedPlayers(const Id caller);

    //-------------------------------------------------------------------------
    const Id getMapId(const InstanceId caller);
};
//-----------------------------------------------------------------------------
#endif  // __GAME_H__
