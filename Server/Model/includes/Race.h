#ifndef __RACE_H__
#define __RACE_H__

#include <string>

#include "config_structs.h"

/*
 * Raza:
 *      Humanos, elfos, enanos, gnomos.
 * 
 * No tienen comportamiento alguno, simplemente
 * aportan, por un lado, factores a tener en cuenta en el calculo
 * de las ecuaciones de mana y vida, y por otro lado, modificadores
 * que intervienen en el calculo de atributos.
 */
struct Race {
        const unsigned int id;
        const std::string name;

        const unsigned int max_health_factor;
        const unsigned int max_mana_factor;
        const unsigned int health_recovery_factor;
        const unsigned int mana_recovery_factor;

        Race(const RaceCfg& data);
        ~Race();
};

#endif
// CONSTITUCION, INTELIGENCIA, FUERZA, AGILIDAD