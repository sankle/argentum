#ifndef __CLIENT_DEFS_H__
#define __CLIENT_DEFS_H__

//-----------------------------------------------------------------------------
// Argumentos esperados en la invocación

enum ExpectedArgs { NAME, EXPECTED_ARGC };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Códigos de retorno

#define SUCCESS 0
#define ERROR 1

#define USAGE_ERROR 100
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración del frame-rate

#define FPS 60            /* frames/s */
#define RATE (1000 / FPS) /* ms/frame */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración de información de las unidades

#define INFO_SPACE_FROM_HEAD 10 /* px */
#define INFO_NAME_FONTSIZE 16   /* fontsize del nickname + lvl de c/ unidad */
#define INFO_LVL_FONTSIZE 10    /* fontsize del nickname + lvl de c/ unidad */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración de las vistas

// Fuentes
#define VIEWS_INPUT_FONT FONT_SANFORD_FP
#define VIEWS_CURSOR_FONT FONT_SANFORD_FP
#define VIEWS_INFO_FONT FONT_FORCEDSQUARE_FP

// Lógica
#define VIEWS_MAX_INPUT_SIZE 20
#define VIEWS_INPUT_TEXTBOX_X_OFFSET 5
#define VIEWS_CURSOR_TIME_ANIMATION 500
#define VIEWS_ITERATIONS_TO_SWITCH_CURSOR (VIEWS_CURSOR_TIME_ANIMATION / RATE)

// Colores
#define VIEWS_FONT_COLOR \
    { 0, 0, 0, 255 }
#define VIEWS_ERROR_COLOR \
    { 219, 4, 4, 255 }
#define VIEWS_WARNING_COLOR \
    { 255, 145, 0, 255 }
//-----------------------------------------------------------------------------

#endif  // __CLIENT_DEFS_H__
