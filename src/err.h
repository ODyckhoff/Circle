#ifndef __CIRCLE_ERR_H
#define __CIRCLE_ERR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util/hashmap.h"

extern hashmap_t *err_map;

int err_main ();
void err_destroy ();

int reg_err ( char *errname, char *desc, int flags );
void throw_err ( char *errname, char *errstr );

int _err_init ( char *errname );
int _err_chk_key ( char *errname );
int _err_chk_flags ( int flags );

char *_err_get_desc ( char *errname );
void _err_set_desc ( char *errname, char *desc );

int _err_get_flags( char *errname );
void _err_set_flags( char *errname, int flags );

enum _flags_e
{
    E_NONE  = 0,
    E_WARN  = 1,
    E_ERROR = 2,
    E_FATAL = 4
};

struct err_t
{
    char *errname;
    char *desc;
    int flags;
} err_t;

#endif /* __CIRCLE_ERR_H */
