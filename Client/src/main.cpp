// CLIENTE

// ----------------------------------------------------------------------------
#include <stdio.h>

#include <string>

#include "../../Common/includes/Exceptions/Exception.h"
#include "../includes/Client.h"
#include "../includes/defs.h"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    if (argc != EXPECTED_ARGC) {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[NAME]);
        return USAGE_ERROR;
    }

    std::string hostname = argv[HOSTNAME];
    std::string port = argv[PORT];

    try {
        Client client(hostname, port);
        client.run();

    } catch (const Exception& e) {
        fprintf(stderr, "%s\n", e.what());
        return ERROR;
    } catch (...) {
        fprintf(stderr, "Unknown error\n");
        return ERROR;
    }

    return SUCCESS;
}

// ----------------------------------------------------------------------------