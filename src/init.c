/*
 * init.c
 * -------
 * This is where the Circle application starts.
 *
 * Author: Owen Rodger Dyckhoff
 * Creation Date: 29th March 2016
 *
 */

#include "init.h"

int main ( void )
{
    char *error_errors[4] = { "OK", "Illegal Key.", "Key exists.", "Out of Memory." };
    int status;
    /* Initialise error module. */
    if ( ( status = err_main () ) > 0 )
    {
        fprintf ( stderr, "[FATAL CORE ERROR]: Unable to start up error module.: %s\n", error_errors[status] );
        return EXIT_FAILURE;
    }

    /* Initialise config module. */
    if ( ( status = cfg_main () ) > 0 )
    {
        throw_err( "core/config", "" );
        return;
    }

    /* Iinitialise logging module. */
    if ( ( status = log_main () ) > 0 )
    { 
        throw_err( "core/logger", "" );
    }

    /* Initialise engine. */
    if ( ( status = engine_main () ) > 0 )
    {
        throw_err( "core/engine", "" );
    }

    /* End of processing. */
    err_destroy ();
    return EXIT_SUCCESS;
}
