
// Create a new set object.
void * swisstable_set_create(void);

// Free a set object, note that it is your responsibility to free the keys.
void swisstable_set_free(void *root);

// Attempt to insert a new key into set.
// Returns key if the key was not known and successfully inserted.
// Returns existing key if already known.
void * swisstable_set_insert(void *root, void *key, size_t keysize);

// Call callback on every known key.
void swisstable_set_foreach(void *root, void (*callback)(void *key));

// Create a new map object.
void * swisstable_map_create(void);

// Free a map object, note that it is your responsibility to free the keys and values.
void swisstable_map_free(void *root);

// Attempt to associate key with value.
// Returns key if the key was not known and successfully inserted.
// Returns existing key if key already known.
void * swisstable_map_insert(void *root, void *key, size_t keysize, void *value);

// Lookup a key in table.
// Returns value if key is known.
// Returns NULL if key is not known.
void * swisstable_map_search(void *root, void *key, size_t keysize);

// Call callback for every known key.
void swisstable_map_foreach(void *root, void (*callback)(void *key, void *value));
