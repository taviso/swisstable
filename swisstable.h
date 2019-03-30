#ifndef __SWISSTABLE_H
#define __SWISSTABLE_H

#ifndef __cplusplus
typedef struct swisstableset swisstableset_t;
typedef struct swisstablemap swisstablemap_t;
typedef struct swisstableumap swisstableumap_t;
#endif

// Create a new set object.
swisstableset_t * swisstable_set_create(void);

// Free a set object, note that it is your responsibility to free the keys.
void swisstable_set_free(swisstableset_t *root);

// Attempt to insert a new key into set.
// Returns key if the key was not known and successfully inserted.
// Returns existing key if already known.
void * swisstable_set_insert(swisstableset_t *root, const void *key, size_t keysize);

// Call callback on every known key.
void swisstable_set_foreach(swisstableset_t *root, void (*callback)(void *key));

// Create a new map object.
swisstablemap_t * swisstable_map_create(void);

// Free a map object, note that it is your responsibility to free the keys and values.
void swisstable_map_free(swisstablemap_t *root);

// Attempt to associate key with value.
// Returns key if the key was not known and successfully inserted.
// Returns existing key if key already known.
void * swisstable_map_insert(swisstablemap_t *root, const void *key, size_t keysize, void *value);

// Lookup a key in table.
// Returns value if key is known.
// Returns NULL if key is not known.
void * swisstable_map_search(swisstablemap_t *root, const void *key, size_t keysize);

// Call callback for every known key.
void swisstable_map_foreach(swisstablemap_t *root, void (*callback)(void *key, void *value));

// These alternatives use integers instead of pointers, so avoid some
// dereferences and overhead from creating string_view wrappers.
swisstableumap_t * swisstable_map_create_uintptr(void);
void swisstable_map_free_uintptr(swisstableumap_t *root);
void * swisstable_map_insert_uintptr(swisstableumap_t *root, uintptr_t key, void *value);
void * swisstable_map_search_uintptr(swisstableumap_t *root, uintptr_t key);
void swisstable_map_foreach_uintptr(swisstableumap_t *root, void (*callback)(uintptr_t key, void *value));

// You can give a hint about expected number of elements to avoid allocator
// overhead (RECOMMENDED).
void swisstable_set_reserve(swisstableset_t *root, size_t sizehint);
void swisstable_map_reserve(swisstablemap_t *root, size_t sizehint);
void swisstable_map_reserve_uintptr(swisstableumap_t *root, size_t sizehint);

#else
# warning swisstable.h included twice
#endif
