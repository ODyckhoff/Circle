#include <stdlib.h>

#include "irc.h"
#include "cfg.h"
#include "handler.h"
#include "sockirc.h"

irc_t *init_irc() {
    irc_t *irc = malloc( sizeof( irc_t ) );
    lcfgitem( irc, IRC_SERVER );
    lcfgitem( irc, IRC_PORT );
    lcfgitem( irc, IRC_NICK );
    lcfgitem( irc, IRC_AUTH_NICK );
    lcfgitem( irc, IRC_CHANNEL );

    if ( irc_connect( irc, irc->server, irc->port ) < 0 ) {
        fprintf( stderr, "Connection failed.\n" );
        goto exit_err;
    }

    irc_set_output( irc, "/dev/stdout" );

    fprintf(stderr, "irc->nick = %s\n", irc->nicksrvauth);
    if ( irc_login( irc, irc->nick ) < 0 ) {
        fprintf( stderr, "Couldn't log in.\n" );
        goto exit_err;
    }

    if( irc_serv_auth( irc, irc->nicksrvauth ) < 0 ) {
        fprintf( stderr, "Couldn't auth with services.\n" );
        goto exit_err;
    }

    if ( irc_join_channel( irc, irc->channel ) < 0 ) {
        fprintf( stderr, "Couldn't join channel.\n" );
        goto exit_err;
    }

    return irc;

exit_err:
    irc_close( irc );
    return NULL;
}

int irc_run( irc_t *irc ) {
   while ( irc_handle_data( irc ) >= 0 );

   irc_close( irc );
   return 0;
}
