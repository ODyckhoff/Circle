/* irc.c - main IRC core that will connect, send and receive messages. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "irc.h"
#include "sockirc.h"
#include "command.h"
#include "handler.h"

int irc_connect(irc_t *irc, const char *server, const char *port) {
    if( (irc->s = get_socket(server, port)) < 0 ) {
        return -1;
    }
    
    irc->bufptr = 0;

    return 0;
}

int irc_login(irc_t *irc, const char *nick) {

    return irc_reg(irc->s, nick, "circle", "C IRC Robot");
}

int irc_join_channel(irc_t *irc, const char *channel) {

    strncpy(irc->channel, channel, 254);
    irc->channel[254] = '\0';

    return irc_join(irc->s, channel);
}

int irc_serv_auth(irc_t *irc, const char *pass) {
    char outline[254] = "identify ";
    strcat(outline, pass);
    outline[254] = '\0';

    return irc_privmsg(irc->s, "NickServ", outline);
}

int irc_set_output(irc_t *irc, const char *file) {
    irc->file = fopen(file, "w");
    if ( irc->file == NULL )
        return -1;
    return 0;
}

void irc_close(irc_t *irc) {
    close(irc->s);
    fclose(irc->file);
}

