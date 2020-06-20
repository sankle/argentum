#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#define EMPTY_ITEM_ID 0

#include <array>
#include <map>
#include <exception>
#include <cstdio>

#include "../../../Common/includes/game_config.h"

#include "Item.h"
#include "Level.h"
#include "Slot.h"

/*
 * El Inventario es un contenedor que guarda Items,
 * que pueden ser armas, hechizos o pociones.
 * Se accede a el mediante indices. 
 * Puede contener hasta INVENTORY_MAX_ITEMS elementos.
 */
class Inventory {
    private:
        std::array<Slot, N_INVENTORY_SLOTS> slots;
        std::map<Id, uint8_t> id_slot_map;
        uint8_t occupied_slots;

        unsigned int safe_gold, excess_gold;
        unsigned int max_safe_gold, max_excess_gold;
        Level& character_level;

        /*
         * Devuelve la el indice del menor slot libre del
         * container del inventario.
         */
        const uint8_t _getNextFreeSlot() const;

        /*
         * Agrega la cantidad de oro especificada en slot_gold,
         * sin superar el slot_max_gold.
         * Si la cantidad sobrepasa slot_max_gold, se guarda lo
         * que se puede, hasta llegar al limite.
         * 
         * Retorna la cantidad de oro que no se pudo guardar.
         */
        unsigned int _addGold(const uint32_t amount, 
                              unsigned int& slot_gold,
                              unsigned int& slot_max_gold);

        /*
         * Retira la cantidad de oro especificada de slot_gold,
         * en caso de haber suficiente.
         * Si no hay suficiente, retira todo lo que hay.
         * 
         * Retorna la cantidad de oro que se pudo retirar.
         */
        unsigned int _gatherGold(const uint32_t amount,
                                 unsigned int& slot_gold);

    public:
        Inventory(Level& level);
        ~Inventory();

        Inventory(const Inventory&) = delete;
        Inventory& operator=(const Inventory&) = delete;
        Inventory(Inventory&&) = delete;
        Inventory& operator=(Inventory&&) = delete;

        /*
         * Obtiene el item en la posicion especificada.
         * En caso de no haber, retorna nullptr.
         * 
         * Lanza InvalidPositionException si la posicion
         * especificada es invalida (fuera de rango).
         */
        Item* gatherItem(const uint8_t n_slot);

        /*
         * Obtiene amount de gold del inventario.
         * 
         * Lanza InsufficientGoldException si la cantidad de oro
         * presente en el inventario es menor a amount.
         */
        void gatherGold(const uint32_t amount);

        /*
         * Agrega un item al inventario.
         * Retorna la posicion en la que se agrego.
         * 
         * Lanza FullInventoryException si el inventario esta
         * lleno y no se puede agregar.
         */
        const uint8_t addItem(Item* item);

        /*
         * Agrega amount de gold al inventario, llenando primero
         * el oro seguro y luego el oro en exceso, hasta llegar
         * a sus respectivos limites.
         * 
         * Lanza GoldMaximumCapacityReachedException si no se pudo
         * guardar todo el oro por llegar al limite de capacidad.
         */
        void addGold(const uint32_t amount);

        void updateMaxAmountsOfGold();

        void debug() const; // Para testear.
};

class FullInventoryException: public std::exception {
    public:
        virtual const char* what() const noexcept;
};

class InvalidPositionException: public std::exception {
    public:
        virtual const char* what() const noexcept;
};

class InsufficientGoldException: public std::exception {
    public:
        virtual const char* what() const noexcept;
};

class GoldMaximumCapacityReachedException: public std::exception {
    public:
        virtual const char* what() const noexcept;
};

#endif