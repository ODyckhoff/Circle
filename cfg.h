#ifndef __CFG_H
#define __CFG_H

#include "irc.h"

typedef enum irccfg_e {
    IRC_SERVER = 0,
    IRC_PORT = 1,
    IRC_NICK = 2,
    IRC_AUTH_NICK = 3,
    IRC_CHANNEL = 4
} irccfg_e;

extern const char *cfgparse[5];

void  lcfgitem( struct irc_t *irc, int arg );

#endif /* __CFG_H */
