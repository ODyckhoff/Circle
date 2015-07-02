#include "irc.h"

irc_t *init_irc();
void sethandler(struct irc_t *irc, int ( *hndlf )(struct irc_t*,char*,char*,char*));
int irc_run();
