# Accessing Abseil SwissTables from C

This is a tiny wrapper library allowing C code to use SwissTables, Google's
state-of-the-art hash table implementation.

There is no pure C implementation of SwissTables, so it can't be used without a
wrapper library. I think the majority of C programmers use POSIX `hcreate(3)`
when they need a hash table, which is portable across UNIX systems.

I had a question, are SwissTables so good that I should start using them in C
code instead of
[`hcreate()`](http://pubs.opengroup.org/onlinepubs/009695299/functions/hcreate.html)
or even GLib's [`GHashTable`](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html)?

Here is a quick test to find out.

# Building

From within the swisstable directory, do this:

```bash
$ git clone https://github.com/abseil/abseil-cpp.git
$ cmake .
$ make
```

Now you have `libswisstable.a` and `swisstable.h` that you can use from C,
remember to link against `libstdc++` and `libm`, and use `-pthread`, something
like this:

```bash
$ gcc -pthread yourcode.c libswisstable.a -lstdc++ -lm
```

To compile the benchmark use:

```bash
$ gcc -O3 -DNDEBUG -march=native -pthread -o benchmark benchmark.c libswisstable.a $(pkg-config --cflags --libs glib-2.0) -lm -lstdc++
```

The benchmark requires `libglib2.0-dev` to compare against `GHashTable`.

# Results

On my system, here are the default results:

```bash
$ ./benchmark
hash tables initialized, begin benchmark...
Trying string keys...
hsearch(3) ENTRY completed in 10 seconds, 261506 duplicates
swisstable_map_insert ENTRY completed in 3 seconds, 261506 duplicates
GHashTable ENTRY completed in 7 seconds, 261506 duplicates
Trying integer keys...
GHashTable ENTRY completed in 5 seconds, 261506 duplicates
swisstable_map_insert ENTRY completed in 3 seconds, 261506 duplicates
```

Pretty impressive default performance without any tuning. I think the answer is
"yes, you should consider switching C code to Swiss Tables if you have
performance critical hash tables".

You can extend the wrapper code to allow specifying custom hashing routines,
and so on (not implemented yet).

# Example

```c
#include <stdint.h>
#include "swisstable.h"

int main(int argc, char **argv)
{
    void *swisstable = swisstable_map_create();

    if (argc != 3) {
        printf("Usage: %s key value\n", *argv)
    }

    swisstable_map_insert(swisstable, argv[1],          // pointer to key
                                      strlen(argv[1]),  // sizeof or strlen of key, key can be an object or integer
                                      argv[2]);         // value

    char *result = swisstable_map_search(swisstable, argv[1], strlen(argv[1]));

    printf("Key %s associated with value %s\n", argv[1], result);
    swisstable_map_free(swisstable);
    return 0;
}
```

```bash
$ gcc example.c -pthread libswisstable.a -lstdc++ -lm
```

# Notes

You can use anything as a key - strings, structs, integers, floats, etc. However, be
aware that whatever you pass is interpreted as a buffer of size keylen, so if
you're using a struct, structure padding might cause unexpected results.

Either initialize structures with `calloc`/`memset`, or use
`__attribute__((packed))` or equivalent.

You can also use `swisstable_map_create_uintptr` if you're using an integer
type that doesn't require a pointer.

# API

```c
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

// These alternatives use integers instead of pointers, so avoid some
// dereferences and overhead from creating string_view wrappers.
void * swisstable_map_create_uintptr(void);
void swisstable_map_free_uintptr(void *root);
void * swisstable_map_insert_uintptr(void *root, uintptr_t key, void *value);
void * swisstable_map_search_uintptr(void *root, uintptr_t key);
void swisstable_map_foreach_uintptr(void *root, void (*callback)(uintptr_t key, void *value));

// You can give a hint about expected number of elements to avoid allocator
// overhead (RECOMMENDED).
void swisstable_set_reserve(void *root, size_t sizehint);
void swisstable_map_reserve(void *root, size_t sizehint);
void swisstable_map_reserve_uintptr(void *root, size_t sizehint);
```
