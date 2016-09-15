#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <math.h>
#include <stdint.h>
#define __ROUND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#define INITIAL_SIZE 512
#define BUCKETTHOLD 20
#define THRESHOLD(x) (__ROUND(0.8*x))

#define E_NOKEY = 1 /* The specified key cannot be found. */
#define E_FULL  = 2 /* The hashmap is full. */
#define E_NUNIQ  = 3 /* The key already exists. Not unique */

typedef struct _entry_t
{
    char *key;
    void *value;
    struct _entry_t *next;
} entry_t;

typedef struct _bucket_t
{
    int numentries;
    entry_t *entries;
} bucket_t;

typedef struct _hashmap_t
{
    int capacity;
    int size;
    bucket_t **buckets;
} hashmap_t;

hashmap_t *hm_init ( int sizefactor );
void hm_free ( hashmap_t *hm );
int rehash ( hashmap_t *hm );
int hm_set ( hashmap_t *hm, char *key, void *value );
void *hm_get ( hashmap_t *hm, char *str );
static int hashstr ( hashmap_t *hm, char *key );

#endif /* __HASHMAP_H */
