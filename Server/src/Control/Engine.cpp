#include "../../includes/Control/Engine.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Engine::_init() {
    // Cargamos la configuración
    json config = JSON::loadJsonFile(ENGINE_CONFIG_FILEPATH);
    int ticks_per_second = config["ticks_per_second"];
    rate = 1000 / ticks_per_second; /* ms por cada tick (floor) */
}

void Engine::_processNewConnections() {
    NewConnection* new_connection = nullptr;
    while ((new_connection = new_connections.pop())) {
        fprintf(stderr, "ENGINE: Procesando una nueva conexión...\n");

        InstanceId id = this->game.newCharacter((*new_connection).data);
        Id map = this->game.getMapId(id);
        active_clients.add(id, map, (*new_connection).peer);
        this->game.broadcastNewCharacter(id);

        delete new_connection;
    }
}

void Engine::_processCommands() {
    Command* cmd = nullptr;
    while ((cmd = commands.pop())) {
        fprintf(stderr, "ENGINE: Ejecutando comando\n");
        cmd->exec(game);
        delete cmd;
    }
    // fprintf(stderr, "No hay mas comandos por ejecutar\n");
}

void Engine::_processFinishedConnections() {
    InstanceId* finished_connection = nullptr;
    while ((finished_connection = finished_connections.pop())) {
        fprintf(stderr, "ENGINE: Eliminando una conexión terminada...\n");

        game.deleteCharacter(*finished_connection);
        active_clients.remove(*finished_connection);
        // acá habría que persistir al cliente que se desconecto

        delete finished_connection;
    }
}

void Engine::_freeQueues() {
    {
        InstanceId* p = nullptr;
        while ((p = finished_connections.pop())) {
            delete p;
        }
    }

    {
        InstanceId* p = nullptr;
        while ((p = finished_connections.pop())) {
            game.deleteCharacter(*p);
        }
    }

    {
        Command* p = nullptr;
        while ((p = commands.pop())) {
            delete p;
        }
    }
}

void Engine::_loopIteration(int it) {
    _processNewConnections();
    _processCommands();
    game.actCharacters(it);
    game.actCreatures(it);
    game.spawnNewCreatures(it);
    _processFinishedConnections();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Engine::Engine(Database& database,
               NonBlockingQueue<NewConnection*>& new_connections)
    : keep_executing(true),
      database(database),
      rate(0),
      new_connections(new_connections),
      finished_connections(),
      commands(),
      active_clients(commands, finished_connections),
      game(active_clients) {}

void Engine::run() {
    fprintf(stderr, "DEBUG: Comienza la ejecución del engine.\n");

    // Inicializamos recursos necesarios
    _init();

    // Variables para controlar el frame-rate
    auto t1 = std::chrono::steady_clock::now();
    auto t2 = t1;
    std::chrono::duration<float, std::milli> diff;
    int rest = 0, behind = 0, lost = 0;
    int it = 1;

    // Loop principal
    while (keep_executing) {
        _loopIteration(it);

        // Controlamos el rate y verificamos pérdida de frames.
        // Idea de implementación:
        // https://eldipa.github.io/book-of-gehn/articles/2019/10/23/Constant-Rate-Loop.html
        it = 0;
        t2 = std::chrono::steady_clock::now();
        diff = t2 - t1;
        rest = rate - std::ceil(diff.count());

        if (rest < 0) {
            fprintf(stderr, "\n\n=== PÉRDIDA DE FRAME/S ===\n\n\n");
            behind = -rest;
            lost = rate + (behind - behind % rate);
            rest = rate - behind % rate;
            t1 += std::chrono::milliseconds(lost);
            it += std::floor(lost / rate);
        }

        // fprintf(stderr, "ENGINE: Sleeping for %i ms.\n", rest);

        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        t1 += std::chrono::milliseconds(rate);
        it += 1;
    }

    //-------------------------------------------------------------------------
    // Salimos ordenadamente:

    // Terminamos las conexiones forzosamente
    active_clients.stop();

    // Vaciamos las colas para no perder memoria:
    _freeQueues();

    // Persistir

    fprintf(stderr, "DEBUG: Termina la ejecución del engine.\n");
}

void Engine::stop() {
    keep_executing = false;
}

Engine::~Engine() {}

//-----------------------------------------------------------------------------
