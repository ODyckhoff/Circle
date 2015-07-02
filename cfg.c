/* cfg.c - loads relevant configurations. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "util/str.h"

const char *cfgparse[5] = {
    "IRC_SERVER",
    "IRC_PORT",
    "IRC_NICK",
    "IRC_AUTH_NICK",
    "IRC_CHANNEL"
};

void lcfgitem( irc_t *irc, int arg ) {

    FILE *f;
    /* char c; */
    char *fn;
    char *buffer = 0;
    char *cptr;
    char *ptr;
    char **lines;
    long length;
    int i, count;
    int len;
    /* int found = 0; */

    fn = malloc( 100 );
    strcpy( fn, __FILE__ );
    i = strlen( __FILE__ ) - 1;
    while( fn[i] != '/' ) {
        fn[i] = '\0';
        i--;
    }

    strcat( fn, "irc.cfg" );

    f = fopen( fn, "r" );

    if( f == NULL ) {
        fprintf( stderr, "Error opening config file: %s\n", fn );
        return;
    }
    
    if( f ) {
        fseek( f, 0, SEEK_END );
        length = len = ftell( f );
        fseek( f, 0, SEEK_SET );
        buffer = malloc( length );
        if( buffer ) {
            fread( buffer, 1, length, f );
        }
        fclose( f );
    }

    if( buffer ) {
        for( i = 0, count = 0; i < len; i++ )
            count += ( buffer[i] == '\n' );

        buffer[ len - 1 ] = '\0';

        lines = strsplit( buffer, "\n" );
        if( lines == NULL ) {
            return;
        }

        for( i = 0; i < count; i++ ) {
            ptr = strsep( &lines[i], ":" );
            fprintf(stderr, "ptr = %s, lines[i] = %s\n", ptr, lines[i]);
            if( strncmp( ptr, cfgparse[ arg ], strlen( cfgparse[ arg ] ) ) == 0 ) {

                cptr = malloc( strlen( lines[i] ) );
                strncpy( cptr, lines[i], strlen( lines[i] ) );

                switch( arg ) {
                    case IRC_SERVER:
                        irc->server = cptr;
                    break;
                    case IRC_CHANNEL:
                        irc->channel = cptr;
                    break;
                    case IRC_NICK:
                        irc->nick = cptr;
                    break;
                    case IRC_AUTH_NICK:
                        irc->nicksrvauth = cptr;
                    break;
                    case IRC_PORT:
                        irc->port = cptr;
                    break;
                }
            }
        }
        
        if( ! irc->port && arg == 1 ) {
            irc->port = malloc( 5 );
            strncpy( irc->port, "6667", 4 );
            irc->port[4] = '\0';
        }
    }
}

