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
    void *swisstableint;
    GHashTable *ghashtable;
    GHashTable *ghashtableint;
    ENTRY *strdata = calloc(kNumEntries, sizeof(ENTRY));
    ENTRY *intdata = calloc(kNumEntries, sizeof(ENTRY));

    // Generate n strings
    for (int n = 0; n < kNumEntries; n++) {
        intptr_t num = rand();
        strdata[n].data = (void *)(num);
        asprintf(&strdata[n].key, "%d", (intptr_t)(strdata[n].data));

        // Try with integer key as well (only glib and swiss, hcreate doesnt
        // support anything except strings).
        intdata[n].key = GINT_TO_POINTER(num);
        intdata[n].data = strdata[n].key;
    }

    hcreate(kNumEntries);
    swisstable = swisstable_map_create();
    swisstableint = swisstable_map_create_uintptr();
    ghashtable = g_hash_table_new(g_str_hash, g_str_equal);
    ghashtableint = g_hash_table_new(g_int64_hash, g_int64_equal);

    swisstable_map_reserve(swisstable, kNumEntries);
    swisstable_map_reserve_uintptr(swisstableint, kNumEntries);

    fprintf(stderr, "hash tables initialized, begin benchmark...\n");
    fprintf(stdout, "Trying string keys...\n");

    duplicates = 0;
    start = time(0);

    for (int n = 0; n < kNumEntries; n++) {
        ENTRY *ep = hsearch(strdata[n], ENTER);

        // There is already an entry in there with this string.
        if (ep->key != strdata[n].key) {
            duplicates++;
        }
    }

    fprintf(stdout, "hsearch(3) ENTRY completed in %ld seconds, %u duplicates\n", time(0) - start, duplicates);

    duplicates = 0;
    start = time(0);

    for (int n = 0; n < kNumEntries; n++) {
        void *key = swisstable_map_insert(swisstable,
                                          strdata[n].key,
                                          strlen(strdata[n].key),
                                          &strdata[n]);

        // There is already an entry in there with this string.
        if (key != strdata[n].key) {
            duplicates++;
        }
    }

    fprintf(stdout, "swisstable_map_insert ENTRY completed in %ld seconds, %u duplicates\n", time(0) - start, duplicates);

    duplicates = 0;
    start = time(0);

    for (int n = 0; n < kNumEntries; n++) {
        gboolean r = g_hash_table_insert(ghashtable,
                                         strdata[n].key,
                                         &strdata[n]);

        // There is already an entry in there with this string.
        if (r == false) {
            duplicates++;
        }
    }

    fprintf(stdout, "GHashTable ENTRY completed in %ld seconds, %u duplicates\n", time(0) - start, duplicates);

    fprintf(stdout, "Trying integer keys...\n");

    duplicates = 0;
    start = time(0);

    for (int n = 0; n < kNumEntries; n++) {
        gboolean r = g_hash_table_insert(ghashtableint,
                                         &intdata[n].key,
                                         intdata[n].data);

        // There is already an entry in there with this string.
        if (r == false) {
            duplicates++;
        }
    }

    fprintf(stdout, "GHashTable ENTRY completed in %ld seconds, %u duplicates\n", time(0) - start, duplicates);

    duplicates = 0;
    start = time(0);

    for (int n = 0; n < kNumEntries; n++) {
        void *data = swisstable_map_insert_uintptr(swisstableint,
                                                   GPOINTER_TO_INT(intdata[n].key),
                                                   intdata[n].data);

        // There is already an entry in there with this string.
        if (data != intdata[n].data) {
            duplicates++;
        }
    }

    fprintf(stdout, "swisstable_map_insert ENTRY completed in %ld seconds, %u duplicates\n", time(0) - start, duplicates);

    for (int n = 0; n < kNumEntries; n++) {
        free(strdata[n].key);
    }

    hdestroy();
    swisstable_map_free(swisstable);
    swisstable_map_free_uintptr(swisstableint);
    g_hash_table_destroy(ghashtable);
    g_hash_table_destroy(ghashtableint);
    free(strdata);
    free(intdata);
    return 0;
}
