#ifndef __CLIENT_DEFS_H__
#define __CLIENT_DEFS_H__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Argumentos esperados en la invocación

enum ExpectedArgs { NAME, HOSTNAME, PORT, EXPECTED_ARGC };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Códigos de retorno

#define SUCCESS 0
#define ERROR 1

#define USAGE_ERROR 100
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#endif  // __CLIENT_DEFS_H__
