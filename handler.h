#ifndef HANDLER_H
#define HANDLER_H

#include "irc.h"

int irc_handle_data(struct irc_t *irc);
int irc_parse_action(struct irc_t *irc);
int irc_reply_message(irc_t *irc, char *irc_nick, char *irc_chan, char *msg);
int irc_log_message(irc_t *irc, const char *nick, const char *chan, const char *message);
int bot_command(irc_t *irc, char *irc_nick, char *irc_chan, char *msg);
int general_parse(irc_t* irc, char *irc_nick, char *irc_chan, char *msg);

#endif /* HANDLER_H */
