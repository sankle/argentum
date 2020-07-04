#include "../../includes/GameView/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_init() {
    json config = JSON::loadJsonFile(paths::config(VIEWS_CONFIG_FILEPATH));

    /* Iniciamos la cámara */
    camera.init(config["camera"]);

    /* Iniciamos la HUD */
    hud.init(config["hud"]);

    /* Iniciamos el EventHandler */
    event_handler.init(config, renderer.getWidthScaleFactor(),
                       renderer.getHeightScaleFactor());
}

void GameView::_loadMedia() {
    unit_sprites.loadMedia();
    item_sprites.loadMedia();
    player.loadMedia();
    characters.loadMedia();
    creatures.loadMedia();
    hud.loadMedia();
    map.loadMedia();
}

void GameView::_processSDLEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        event_handler.handleEvent(e);

        // Propagamos el evento al mixer
        Mixer::handleEvent(e);
    }
}

void GameView::_processMessages() {
    Message* message = NULL;
    while ((message = messages.pop())) {
        message->update(hud);
        delete message;
    }
}

void GameView::_processBroadcasts() {
    Broadcast* broadcast = NULL;
    while ((broadcast = broadcasts.pop())) {
        broadcast->exec(map, player, characters, creatures);
        delete broadcast;
    }
}

void GameView::_processEvents() {
    GameEvent* event = NULL;
    while ((event = events.pop())) {
        event->act(player.getPos());
        delete event;
    }
}

void GameView::_func(const int it) {
    /* Vaciamos las colas a procesar*/
    _processSDLEvents();

    // auto t1 = std::chrono::steady_clock::now();
    _processMessages();
    _processBroadcasts();

    /* Limpiamos la pantalla */
    renderer.clearScreen();

    /* Acciones previas al renderizado*/
    player.act(it);
    characters.act(it);
    creatures.act(it);
    camera.center(player.getBox(), map.widthInPx(), map.heightInPx());
    hud.update(it);

    /* Renderizamos y presentamos la pantalla */
    stage.render();
    hud.render();
    // auto t2 = std::chrono::steady_clock::now();

    renderer.presentScreen();

    // std::chrono::duration<float, std::milli> diff = t2 - t1;
    // fprintf(stderr, "Iteration time: %i ms.\n", (int)diff.count());
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GameView::GameView(BlockingQueue<Command*>& commands,
                   NonBlockingQueue<Broadcast*>& broadcasts,
                   NonBlockingQueue<Message*>& messages,
                   NonBlockingQueue<GameEvent*>& events,
                   const Renderer& renderer)
    :  // Comunicación entre hilos
      commands(commands),
      broadcasts(broadcasts),
      messages(messages),
      events(events),

      // Componentes principales
      renderer(renderer),

      // Contenedores de sprites
      unit_sprites(&renderer),
      item_sprites(&renderer),

      // Unidades
      player(&renderer, camera, &unit_sprites),
      characters(&renderer, camera, &unit_sprites),
      creatures(&renderer, camera, &unit_sprites),

      // Componentes de la vista
      hud(&renderer, item_sprites, player),
      map(&renderer, camera, item_sprites),

      // Otros
      stage(map, player, characters, creatures),
      event_handler(exit, commands, hud, map, camera) {
    _init();
    _loadMedia();
}

GameView::~GameView() {}

//-----------------------------------------------------------------------------
