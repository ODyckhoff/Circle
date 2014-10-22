/* socket.c - gets an IP address for the IRC module to use in connections. */

/* Standard headers. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Socket headers. */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <netdb.h>

/* Local headers. */
#include "sockirc.h"

