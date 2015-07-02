/* str.c - home roll a few string functions not included in <string.h> */

#include <stdlib.h>
#include <string.h>

#include "str.h"

char *strrep(char *orig, char *rep, char *with) {
    char *result; /* the return string */
    char *ins;    /*  the next insert point */
    char *tmp;    /*  varies */
    int len_rep;  /*  length of rep */
    int len_with; /*  length of with */
    int len_front; /*  distance between rep and end of last rep */
    int count;    /*  number of replacements */

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; ( tmp = strstr(ins, rep) ); ++count) {
        ins = tmp + len_rep;
    }

    /*  first time through the loop, all the variable are set correctly */
    /*  from here on, */
    /*     tmp points to the end of the result string */
    /*     ins points to the next occurrence of rep in orig */
    /*     orig points to the remainder of orig after "end of rep" */
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return orig;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; /* move to next "end of rep" */
    }
    strcpy(tmp, orig);

    free(ins);
    free(tmp);

    return result;
}

char **strsplit(char *instr, const char *delim) {

    char *token;
    char *str;
    char **rtn;
    int n = 0;

    str = (char *) malloc( sizeof(char *) * strlen(instr) );
    str = strdup(instr);
    rtn = (char **) malloc( sizeof(char *) );

    if(str != NULL) {

        while ((token = strsep(&str, delim)) != NULL) {

            if(0 == strcmp(token, "")) /* Empty string because delimiters. */
                continue;

            rtn    = (char **) realloc( rtn, sizeof(char *) * (n + 1) );
            rtn[n] = (char *) malloc( sizeof(char) * ( strlen(token) + 1 ) );
            strcpy(rtn[n], token);
            strcat(rtn[n], "\0");
            n++;
        }
    }   

    free(token);
    free(str);

    return rtn;
}

void strrev(char *str) {
    char temp, *end_ptr;

    if( str == NULL || !(*str) )
        return;

    end_ptr = str + strlen(str) - 1;

    while( end_ptr > str ) {
        temp = *str;
        *str = *end_ptr;
        *end_ptr = temp;
        str++;
        end_ptr--;
    }
}

