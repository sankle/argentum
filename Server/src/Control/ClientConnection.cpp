#include "../../includes/Control/ClientConnection.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ClientConnection::_finishThread() {
    std::unique_lock<std::mutex> l(m);
    fprintf(stderr, "Se llamó a finishThread...\n");
    if ((++finished_threads) == 2) {
        fprintf(stderr,
                "finishThread agregando conexión terminada a la cola...\n");
        finished_connections.push(new InstanceId(id));
    }
}

void ClientConnection::_sender() {
    // ejecución del sender loop
    fprintf(stderr, "SENDER DE UN CLIENTE EMPEZANDO! Id: %i\n", id);

    /** LOOP DE SENDER DE NOTIFICATIONS
     * Implementar el loop principal de este hilo.
     * Tiene que enviar notificacioens al cliente de la siguiente forma:
     *
     * 1. Saca un Notification* de la cola de notificaciones (aun no está
     * implementada, es similar a comando sólo que en vez de saber ejecutarse,
     * saben enviarse).
     *
     * 2. Se llama a Notification->send(peer), es decir se le pasa el peer
     * socket para que la notificación se envíe a si misma.
     *
     * 3. Se libera la notificacion haciendo delete Notification*.
     *
     * 4. Se repite desde 1.
     *
     * Deberá salir del loop cuando el socket peer deje de funcionar, ya sea
     * porque fue cerrado por el server o por el cliente.
     */

    // avisamos que terminamos
    _finishThread();
    fprintf(stderr, "SENDER DE UN CLIENTE TERMINANDO! Id: %i\n", id);
}

void ClientConnection::_receiver() {
    // ejecución del receiver loop
    fprintf(stderr, "RECEIVER DE UN CLIENTE EMPEZANDO! Id: %i\n", id);

    /** LOOP DE RECEIVER DE COMANDOS
     * Implementar el loop principal de este hilo.
     * Tiene que recibir comandos desde el cliente de la siguiente forma:
     *
     * 1. Se recibe 1 byte de opcode.
     *
     * 2. Se switchea con el opcode y se recibe el resto del comando.
     *
     * 3. Se crea el comando en memoria dinámica y se lo almacena como un
     * Command* (de manera polimorfica) para luego pushearlo a la cola de
     * comandos `commands`.
     *
     * 4. Se repite desde 1.
     *
     * Deberá salir del loop cuando el receive del opcode devuelva 0 (es decir,
     * que se recibieron 0 bytes).
     */

    // avisamos que terminamos
    _finishThread();
    fprintf(stderr, "RECEIVER DE UN CLIENTE TERMINANDO! Id: %i\n", id);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientConnection::ClientConnection(
    const InstanceId id, SocketWrapper& peer,
    NonBlockingQueue<InstanceId*>& finished_connections,
    NonBlockingQueue<Command*>& commands)
    : id(id),
      peer(std::move(peer)),
      finished_connections(finished_connections),
      finished_threads(0),
      commands(commands) {}

void ClientConnection::start() {
    sender = std::thread(&ClientConnection::_receiver, this);
    receiver = std::thread(&ClientConnection::_sender, this);
}

void ClientConnection::push(Notification* notification) {
    notifications.push(notification);
}

void ClientConnection::join() {
    if (sender.joinable()) {
        sender.join();
    }

    if (receiver.joinable()) {
        receiver.join();
    }
}

void ClientConnection::stop() {
    peer.shutdown();
    peer.close();
    join();
}

ClientConnection::~ClientConnection() {
    peer.close();

    if (sender.joinable()) {
        sender.join();
    }

    if (receiver.joinable()) {
        receiver.join();
    }
}

//-----------------------------------------------------------------------------
