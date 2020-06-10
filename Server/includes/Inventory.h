#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#define N_INVENTORY_ITEMS 10

#include <array>
#include <exception>

#include "Item.h"

class FullInventoryException: public std::exception {
    public:
        virtual const char *what() const noexcept;
};

class InvalidPositionException: public std::exception {
    public:
        virtual const char *what() const noexcept;
};

class InsufficientGoldException: public std::exception {
    public:
        virtual const char *what() const noexcept;
};

/*
 * El Inventario es un contenedor que guarda Items,
 * que pueden ser armas, hechizos o pociones.
 * Se accede a el mediante indices. 
 * Puede contener hasta INVENTORY_MAX_ITEMS elementos.
 */
class Inventory {
    private:
        std::array<Item*, N_INVENTORY_ITEMS> container;
        unsigned int items_quantity;
        unsigned int gold_quantity;

        /*
         * Devuelve la el indice del menor slot libre del
         * container del inventario.
         */
        const unsigned int getNextFreeSlot() const;

    public:
        Inventory();
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
        Item* gatherItem(const unsigned int position);

        /*
         * Obtiene amount de gold del inventario.
         * 
         * Lanza InsufficientGoldException si la cantidad de oro
         * presente en el inventario es menor a amount.
         */
        void gatherGold(const unsigned int amount);

        /*
         * Agrega un item al inventario.
         * Retorna la posicion en la que se agrego.
         * 
         * Lanza FullInventoryException si el inventario esta
         * lleno y no se puede agregar.
         */
        const unsigned int addItem(Item* item);

        /*
         * Agrega amount de gold al inventario.
         */
        void addGold(const unsigned int amount);

        void debug() const; // Para testear.
};

#endif