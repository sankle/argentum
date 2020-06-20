#include "../../includes/Control/NotificationReply.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NotificationReply::NotificationReply(char opcode,
                                     std::string reply)
        : opcode(opcode), reply(reply), length(reply.size()) {}

bool NotificationReply::send(const SocketWrapper& peer) {
    try {
        peer << char(0);
        peer << opcode;
        peer << length;
        peer << reply;
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}