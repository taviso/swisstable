#include <iostream>
#include <string>
#include <string_view>
#include "absl/container/flat_hash_set.h"
#include "absl/container/flat_hash_map.h"

typedef absl::flat_hash_set<std::string_view> swisstableset_t;
typedef absl::flat_hash_map<std::string_view, void *> swisstablemap_t;
typedef absl::flat_hash_map<uintptr_t, void *> swisstableumap_t;

extern "C" {
    #include "swisstable.h"
}

extern "C" swisstableset_t * swisstable_set_create(void)
{
    return new absl::flat_hash_set<std::string_view>;
}

extern "C" void swisstable_set_reserve(swisstableset_t *root, size_t sizehint)
{
    root->reserve(sizehint);
}

extern "C" void swisstable_set_free(swisstableset_t *root)
{
    delete root;
}

extern "C" void * swisstable_set_insert(swisstableset_t *root, const void *key, size_t keysize)
{
    std::string_view result((char *)(key), keysize);
    result = *(root->insert(result).first);
    return const_cast<char *>(result.data());
}

extern "C" void swisstable_set_foreach(swisstableset_t *root, void (*callback)(void *key))
{
    for (const auto &p : *root) {
        callback(const_cast<char *>(p.data()));
    }
}

extern "C" swisstablemap_t * swisstable_map_create(void)
{
    return new absl::flat_hash_map<std::string_view, void *>;
}

extern "C" void swisstable_map_reserve(swisstablemap_t *root, size_t sizehint)
{
    root->reserve(sizehint);
}

extern "C" void swisstable_map_reserve_uintptr(swisstableumap_t *root, size_t sizehint)
{
    root->reserve(sizehint);
}

extern "C" swisstableumap_t * swisstable_map_create_uintptr(void)
{
    return new absl::flat_hash_map<uintptr_t, void *>;
}

extern "C" void swisstable_map_free(swisstablemap_t *root)
{
    delete root;
}

extern "C" void swisstable_map_free_uintptr(swisstableumap_t *root)
{
    delete root;
}

extern "C" void * swisstable_map_insert(swisstablemap_t *root, const void *key, size_t keysize, void *value)
{
    std::string_view result((char *)(key), keysize);
    result = (*(root->try_emplace(result, value).first)).first;
    return const_cast<char *>(result.data());
}

extern "C" void * swisstable_map_insert_uintptr(swisstableumap_t *root, uintptr_t key, void *value)
{
    return (*(root->try_emplace(key, value).first)).second;
}

extern "C" void * swisstable_map_search(swisstablemap_t *root, const void *key, size_t keysize)
{
    std::string_view result((char *)(key), keysize);
    auto search = root->find(result);
    if (search == root->end())
        return NULL;
    return const_cast<void *>((*search).second);
}

extern "C" void * swisstable_map_search_uintptr(swisstableumap_t *root, uintptr_t key)
{
    auto search = root->find(key);
    if (search == root->end())
        return NULL;
    return const_cast<void *>((*search).second);
}

extern "C" void swisstable_map_foreach(swisstablemap_t *root, void (*callback)(void *key, void *value))
{
    for (const auto &p : *root) {
        callback(const_cast<char *>(p.first.data()), const_cast<void *>(p.second));
    }
    return;
}

extern "C" void swisstable_map_foreach_uintptr(swisstableumap_t *root, void (*callback)(uintptr_t key, void *value))
{
    for (const auto &p : *root) {
        callback(p.first, const_cast<void *>(p.second));
    }
    return;
}
