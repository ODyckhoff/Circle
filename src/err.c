/*
 * err.c
 * ------
 * This file provides the interface mechanism for the core and for other
 * modules to raise errors within the system, which may or may not be
 * fatal.
 *
 * Author: Owen Rodger Dyckhoff
 * Creation Date: 29th March 2016
 *
 */

#include "err.h"

hashmap_t *_err_map;

/* Function reg_err
 * Register error with the system.
 * Param 'errname': string; The name identifier for the error.
 * Param 'desc': string; Short description of the error.
 * Param 'flags': int; Error type; error, warning, fatal.
 * Returns int indicating success or failure.
*/

int err_main ()
{
    int state;

    _err_map = hm_init( 1 );

    state = reg_err( "core/config", "Unable to initialise configuration.", E_FATAL );
    printf("reg core/config\n");
    if ( state != 0 )
        return state;

    state = reg_err( "core/logger", "Unable to initialise logger.", E_FATAL );
    printf("reg core/logger\n");
    if ( state != 0 )
        return state;

    state = reg_err( "core/engine", "Unable to initialise engine.", E_FATAL );
    printf("reg core/engine\n");
    if ( state != 0 )
        return state;

    return 0;
}

void err_destroy ()
{
    hm_free( _err_map );
}

int reg_err ( char *errname, char *desc, int flags )
{
    int status = 0;

    /* Check errname for illegal characters. */

    status = _err_init( errname );

    if( status == 0 )
    {
        _err_set_desc ( errname, desc );
        _err_set_flags( errname, flags );
    }
    else
    {
        /* Unable to initialise error. */
        /* Check return code for reason. */
        return status;
    }

    return status;
}

/* Function throw_err
 * Throw an error.
 * Param 'errname': string; The key of the error being thrown.
 * Param 'errstr': string; Optional string containing further information
 *                 about the error.
 * Returns void.
*/

void throw_err ( char *errname, char *errstr )
{
    char *desc;
    int flags;

    if ( _err_chk_key( errname ) != 0 )
    {
        /* This error key does not exist. */
        return;
    }

    desc = _err_get_desc( errname );
    flags = _err_get_flags( errname );
    
    /* Format error string. */

    /* Print to console and/or file. */

    if ( flags & E_FATAL )
    {
        /* Fatal error. Execution cannot continue. */
        fprintf ( stderr, "[FATAL]: %s\n", desc ); 
    }
    else if ( flags & E_ERROR )
    {
        /* Error. Non-fatal, so execution may continue. */
        fprintf ( stderr, "[ERROR]: %s\n", desc );
    }
    else if ( flags & E_WARN )
    {
        /* Warning. Just print the advisory notice. */
        fprintf ( stderr, "[WARN]: %s\n", desc );
    }
    else if ( flags == E_NONE )
    {
        fprintf( stderr, "[DEBUG]: %s\n", desc );
        /* Not an error at all. Perhaps for debugging purposes. */
        /* If you're reading this, consider using LOG_DEBUG for debugging. */
    }

    if( strlen( errstr ) > 0 ) {
        fprintf( stderr, "[MORE INFO]: %s\n", errstr );
    }

    if( flags & E_FATAL )
        exit( E_FATAL );

    return;
}

/* Internal Function _err_init
 * Attempts to initialise error entity.
 * Param 'errname': string; The key of the error.
 * Returns int indicating success status. 
*/

int _err_init ( char *errname )
{
    struct err_t *e;
    int status = _err_chk_key ( errname );

    if ( status != 0 )
        return 1; /* Key is illegal. */

    if ( ( e = hm_get ( _err_map, errname ) ) != NULL ) 
         return 2;
    /* Key already exists. */

    e = malloc ( sizeof ( struct err_t ) );
    if( e == NULL )
        return 3; /* Failed to init memory. */

    /* Add 'e' to the hashtable. */
    hm_set ( _err_map, errname, e );

    return 0;
}

/* Internal Function _err_chk_key
 * Checks key for illegal characters.
 * Param 'errname': string; The key of the error.
 * Returns int indicating result of check.
*/

int _err_chk_key ( char *errname )
{
    int i;

    /* Check if illegal characters are contained. */
    /* Start with the first letter. */

    if ( ! ( 
            ( errname[0] > 'A' && errname[0] < 'Z' ) ||
            ( errname[0] > 'a' && errname[0] < 'z' )
    ) )
    {
        return 1; /* Cannot start with anything other than a letter. */
    }

    for ( i = 1; i < strlen ( errname ); i++ )
    {
        if ( ! (
                ( errname[i] > 'A' && errname[i] < 'Z' ) ||
                ( errname[i] > 'a' && errname[i] < 'z' ) ||
                ( errname[i] == '_' )                    ||
                ( errname[i] == '/' )
        ) )
        {
            return 2; /* Character out of range. */
        }
    }

    if ( errname[ strlen ( errname ) - 1 ]  == '/' )
    {
        return 3; /* Cannot end with a forward-slash character. */
    }

    return 0;
}

/* Internal Function _err_chk_flags
 * Checks flags to make sure they're in range.
 * Param 'flags': int; flag bitmask.
 * Returns int indicating result of check.
*/

int _err_chk_flags ( int flags )
{
    if ( flags < E_NONE || flags > ( E_FATAL | E_ERROR | E_WARN ) )
        return 1; /* Flags out of range. */
    else
        return 0;
}

/* Internal Function _err_get_desc
 * Param 'errname': string; The key of the error.
 * Returns string containing error description.
*/

char *_err_get_desc ( char *errname )
{
    struct err_t *e;
    char *desc;
    /* Retrieve description from hashtable. */
    e = hm_get ( _err_map, errname );
    desc = strdup( e->desc );

    return desc;
}

/* Internal Function _err_set_desc
 * Param 'errname': string; The key of the error. 
 * Param 'desc': string; Short description of the error.
 * Returns void.
*/

void _err_set_desc ( char *errname, char *desc )
{
    struct err_t *e;

    /* Set error description in hashtable. */
    e = hm_get ( _err_map, errname );
        
    e->desc = malloc ( strlen ( desc ) + 1 );

    strncpy ( e->desc, desc, strlen ( desc ) );
    return;
}

/* Internal Function _err_get_flags
 * Param 'errname': string; The key of the error.
 * Returns int representing bitmask of flags.
*/

int _err_get_flags ( char *errname )
{
    struct err_t *e;
    int flags;

    /* Get flags from hashtable. */
    e = hm_get ( _err_map, errname );

    flags = e->flags;
    return flags;
}

/* Internal Function _err_set_flags
 * Param 'errname': string; The key of the error.
 * Param 'flags': int; bitmask representing error type.
 * Returns void.
*/

void _err_set_flags ( char *errname, int flags )
{
    struct err_t *e;
    int status;

    /* Set error flags in hashtable. */
    e = hm_get ( _err_map, errname );

    if ( ( status = _err_chk_flags ( flags ) ) == 0 )
        e->flags = flags;

    return;
}
