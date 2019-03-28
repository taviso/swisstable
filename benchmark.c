#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <search.h>
#include <glib.h>
#include <stdbool.h>

#include "swisstable.h"

static const int kNumEntries = 1 << 25;

int main(int argc, char **argv)
{
    unsigned duplicates;
    time_t start;
    void *swisstable;
    GHashTable *ghashtable;
    ENTRY *data = calloc(kNumEntries, sizeof(ENTRY));

    // Generate n strings
    for (int n = 0; n < kNumEntries; n++) {
        intptr_t num = rand();
        data[n].data = (void *)(num);
        asprintf(&data[n].key, "%d", (intptr_t)(data[n].data));
    }

    hcreate(kNumEntries);
    swisstable = swisstable_map_create();
    ghashtable = g_hash_table_new(g_str_hash, g_str_equal);

    fprintf(stderr, "hash tables initialized, begin benchmark...\n");

    duplicates = 0;
    start = time(0);

    for (int n = 0; n < kNumEntries; n++) {
        ENTRY *ep = hsearch(data[n], ENTER);

        // There is already an entry in there with this string.
        if (ep->key != data[n].key) {
            duplicates++;
        }
    }

    fprintf(stdout, "hsearch(3) ENTRY completed in %ld seconds, %u duplicates\n", time(0) - start, duplicates);

    duplicates = 0;
    start = time(0);

    for (int n = 0; n < kNumEntries; n++) {
        void *key = swisstable_map_insert(swisstable,
                                          data[n].key,
                                          strlen(data[n].key),
                                          &data[n]);

        // There is already an entry in there with this string.
        if (key != data[n].key) {
            duplicates++;
        }
    }

    fprintf(stdout, "swisstable_map_insert ENTRY completed in %ld seconds, %u duplicates\n", time(0) - start, duplicates);

    duplicates = 0;
    start = time(0);

    for (int n = 0; n < kNumEntries; n++) {
        gboolean r = g_hash_table_insert(ghashtable,
                                         data[n].key,
                                         &data[n]);

        // There is already an entry in there with this string.
        if (r == false) {
            duplicates++;
        }
    }

    fprintf(stdout, "GHashTable ENTRY completed in %ld seconds, %u duplicates\n", time(0) - start, duplicates);

    hdestroy();
    swisstable_map_free(swisstable);
    g_hash_table_destroy(ghashtable);
    return 0;
}
