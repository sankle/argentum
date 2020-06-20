#ifndef __CONSOLE_H__
#define __CONSOLE_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
#include "../paths.h"
#include "HUDComponent.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Console : public HUDComponent {
   private:
    // Texturas a renderizar

   public:
    /* Constructor */
    Console(const Renderer* renderer);

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const json& config) override;

    /* Carga los archivos necesarios */
    void loadMedia() override;

    /* Renderiza la consola */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Console();
};

//-----------------------------------------------------------------------------

#endif  // __CONSOLE_H__
