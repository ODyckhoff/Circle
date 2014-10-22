/* handler.c - responsible for parsing signals from the IRC server. */

/* Standard headers. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Handler required headers. */
#include <time.h>

/* Local Headers. */
#include "irc.h"
#include "handler.h"
#include "command.h"

int irc_handle_data(irc_t *irc) {
    char tempbuffer[512];
    int rc, i;

    if ( (rc = sck_recv(irc->s, tempbuffer, sizeof(tempbuffer) - 2 ) ) <= 0 ) {
        fprintf(stderr, "Disconnected from server.\n");
        return -1;
    }

    tempbuffer[rc] = '\0';

    for ( i = 0; i < rc; ++i ) {
        switch (tempbuffer[i]) {
            case '\r':
            case '\n': {
                irc->servbuf[irc->bufptr] = '\0';
                irc->bufptr = 0;

                if ( irc_parse_action(irc) < 0 )
                    return -1;

                break;
            }

            default: {
                irc->servbuf[irc->bufptr] = tempbuffer[i];
                if ( irc->bufptr >= (sizeof ( irc->servbuf ) -1 ) )
                    /* Overflow! */
                ;
                else
                   irc->bufptr++;
            }
        }
    }
    return 0;
}

int irc_parse_action(irc_t *irc) {
   
    char irc_nick[128];
    char irc_msg[512];
    char irc_chan[128];
    char *ptr;
    int privmsg = 0;

    fprintf(irc->file, "SERVBUF = %s\n", irc->servbuf);
    if ( strncmp(irc->servbuf, "PING :", 6) == 0 ) {
        return irc_pong(irc->s, &irc->servbuf[6]);
    }
    else if ( strncmp(irc->servbuf, "NOTICE AUTH :", 13) == 0 ) {
        /* Don't care. */
        return 0;
    }
    else if ( strncmp(irc->servbuf, "ERROR :", 7) == 0 ) {
        /* Still don't care. */
        return 0;
    }
   
    /* Parses IRC message for nick, channel and message content. */
    else {
        *irc_nick = '\0';
        *irc_msg  = '\0';
        *irc_chan = '\0';

        /* Check for non-message string. */
        if ( strchr(irc->servbuf, 1) != NULL )
            return 0;
   
        if ( irc->servbuf[0] == ':') {
            ptr = strtok(irc->servbuf, "!");
            if ( ptr == NULL ) {
                printf("ptr == NULL\n");
                return 0;
            }
            else {
                strncpy(irc_nick, &ptr[1], 127);
                irc_nick[127] = '\0';
            }

            while ( (ptr = strtok(NULL, " ")) != NULL ) {
                if ( strcmp(ptr, "PRIVMSG") == 0 ) {
                    privmsg = 1;
                    break;
                }
            }

            if ( privmsg ) {
                if ( (ptr = strtok(NULL, " ")) != NULL) {
                    strncpy(irc_chan, ptr, 127);
                    irc_chan[127] = '\0';

                    /* Check if it's a query. */
                    if( strcmp(irc_chan, irc->nick) == 0) {
                        memset(irc_chan, 0, strlen(irc_chan));
                        strncpy(irc_chan, irc_nick, 127);
                        irc_chan[127] = '\0';
                    }
                }
                if ( (ptr = strtok(NULL, ":")) != NULL) {
                    strncpy(irc_msg, ptr, 511);
                    irc_msg[511] = '\0';
                }
            }
         
            if ( privmsg == 1 && strlen(irc_nick) > 0 && strlen(irc_msg) > 0 ) {
                irc_log_message(irc, irc_nick, irc_chan, irc_msg);
                if ( irc_reply_message(irc, irc_nick, irc_chan, irc_msg) < 0 )
                    return -1;
            }
        }
    }
    return 0;
}

int irc_reply_message(irc_t *irc, char *irc_nick, char *irc_chan, char *msg) {
    /* Checks if someone calls on the bot. */
    if ( *msg == '&' )
        return bot_command(irc, irc_nick, irc_chan, msg);
    else
        return general_parse(irc, irc_nick, irc_chan, msg);
}

int bot_command(irc_t *irc, char *irc_nick, char *irc_chan, char *msg) {
    /* Bot commands are all internal, and are prefixed with '&'. */

    char *command;
    char *arg;
    // Gets command
    command = strtok(&msg[1], " ");
    arg = strtok(NULL, "");
    if ( arg != NULL )
        while ( *arg == ' ' )
            arg++;

    if ( command == NULL )
        return 0;

    if ( strcmp(command, "ping") == 0) {
        if ( irc_privmsg(irc->s, irc_chan, "pong") < 0 )
            return -1;
    }

    else if ( strcmp(command, "poke") == 0) {
        char out[254] = "pokes ";
        if(arg != NULL)
            strcat(out, arg);
        else
            strcat(out, irc_nick);

        out[254] = '\0';

        if( irc_action(irc->s, irc_chan, out) < 0 )
            return -1;
    }

    else if ( strcmp(command, "raw") == 0 ) {
        if( strncmp(irc_nick, "ijz", 3) == 0) {
            if( irc_raw(irc->s, arg) < 0 )
                return -1;
        }
        else {
            if(irc_privmsg(irc->s, irc_chan, "You don't have permission to use that command") < 0)
                return -1;
            return 0;
        }
    }

    else if ( strcmp(command, "ducks") == 0) {
        irc_privmsg(irc->s, irc_chan, "   _          _          _          _          _          ");
        irc_privmsg(irc->s, irc_chan, " >(')____,  >(')____,  >(')____,  >(')____,  >(') ___,    ");
        irc_privmsg(irc->s, irc_chan, "   (` =~~/    (` =~~/    (` =~~/    (` =~~/    (` =~~/    ");
        irc_privmsg(irc->s, irc_chan, "~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~^`---'~^~^~");
    }

    else if ( strcmp(command, "VERSION") == 0) {
        if(irc_privmsg(irc->s, irc_chan, "0.2") < 0)
            return -1;
    }

    /* Else if ... and so on... */    
   
   return 0;
}

int general_parse(irc_t* irc, char *irc_nick, char *irc_chan, char *msg) {
    /* For Non-internal commands. Messages can be handled here, 
     *     or be sent to a handler registered with the irc_t struct.
     * These messages are not prefixed with anything,
     *     and 'msg' can be used directly.
     */

    fprintf(irc->file, "MSG = %s\n", msg);
    if ( strcmp(msg, "VERSION") == 0) {
        if(irc_privmsg(irc->s, irc_chan, "0.2") < 0)
            return -1;
    }

    /* if( irc->handler(irc, irc_nick, irc_chan, irc_msg) < 0)
     *     return -1;
     */

    return 0;
}

int irc_log_message(irc_t *irc, const char *nick, const char *chan, const char *message) {
    char timestring[128];
    time_t curtime;
    time(&curtime);
    strftime(timestring, 127, "%F - %H:%M:%S", localtime(&curtime));
    timestring[127] = '\0';

    fprintf(irc->file, "%s - [%s] <%s> %s\n", chan, timestring, nick, message);
    fflush(irc->file);
}

