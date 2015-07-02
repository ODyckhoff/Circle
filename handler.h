#ifndef HANDLER_H
#define HANDLER_H

#include "irc.h"

int  irc_handle_data( struct irc_t *irc );
int  irc_parse_action( struct irc_t *irc );
int  irc_reply_message( struct irc_t *irc, char *irc_nick, char *irc_chan, char *msg );
int  irc_log_message( struct irc_t *irc, const char *nick, const char *chan, const char *msg );
int  bot_command( struct irc_t *irc, char *irc_nick, char *irc_chan, char *msg );
int  general_parse( struct irc_t* irc, char *irc_nick, char *irc_chan, char *msg );
void addhndlr( struct irc_t* irc, int ( *hndlf )( struct irc_t*,char*,char*,char* ) );
void rmhndlr( struct irc_t* irc, int ( *hndlf )( struct irc_t*,char*,char*,char* ) );
void callhndlr( struct irc_t* irc, char *nick, char *chan, char *msg );

#endif /* HANDLER_H */
