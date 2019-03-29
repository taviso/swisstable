#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "swisstable.h"

int main(int argc, char **argv)
{
    void *swisstable = swisstable_map_create();

    if (argc != 3) {
        printf("Usage: %s key value\n", *argv);
        return 1;
    }

    swisstable_map_insert(swisstable, argv[1],          // pointer to key
                                      strlen(argv[1]),  // sizeof or strlen of key, can be an object or integer
                                      argv[2]);         // value

    char *result = swisstable_map_search(swisstable, argv[1], strlen(argv[1]));

    // For structs, integers, floats, etc you can do this
    // swisstable_map_search(swisstable, &obj, sizeof(obj));
    // This is also a swisstable_map_search_uintptr optimized for integer keys.

    printf("Key %s associated with value %s\n", argv[1], result);
    swisstable_map_free(swisstable);
    return 0;
}
