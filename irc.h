#ifndef IRC_H
#define IRC_H

#include <stdio.h>

typedef struct irc_t {
    int s;
    FILE *file;
    char channel[256];
    char *nick;
    char servbuf[512];
    int bufptr;
} irc_t;

int irc_connect(irc_t *irc, const char *server, const char *port);
int irc_login(irc_t *irc, const char *nick);
int irc_join_channel(irc_t *irc, const char *channel);
int irc_set_output(irc_t *irc, const char *file);
int irc_serv_auth(irc_t *irc, const char *pass);

void irc_close(irc_t *irc);

#endif /* IRC_H */
