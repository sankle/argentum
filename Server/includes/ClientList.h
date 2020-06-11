#ifndef __ClientList_H__
#define __ClientList_H__

#include "ClientConnection.h"
#include <unordered_map>
#include <mutex>

class ClientList {
private:
    // atributos del ClientConnection
    std::unordered_map<int, ClientConnection> connection_map;
    std::mutex m;

public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: id del player que usa la connexion
     */
    ClientList();

    /* Deshabilitamos el constructor por copia. */
    ClientList(const ClientList&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientList& operator=(const ClientList&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientList(ClientList&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientList& operator=(ClientList&& other) = delete;

    void addPlayer(int player_id, Socket&& sock);
    /**
     * Descripción: destructor.
     */
    ~ClientList();
};

#endif  // __ClientList_H__
