#ifndef _NAMER_
#define _NAMER_

typedef struct NamerData *Namer;

/**
 * @brief Creates a new resource namer instance.
 *
 * @returns The created resource namer instance, or NULL if the operation failed.
 */
Namer newNamer();

/**
 * @brief Frees all resources used by a resource namer. Attempting to use a
 * resource namer after it has been freed results in undefined behavior.
 *
 * @returns 0 if the operation succeeded, != 0 if not.
 */
int freeNamer(Namer namer);

/**
 * @brief Maps a given resource to a given name. The namer does not keep a reference
 * to the string, but rather makes an internal copy. This internal copy is also placed
 * in nameData (if not NULL).
 *
 * @returns 0 if the operation succeeded, 1 if the resource was already present,
 * or -1 if the operation failed for other reasons.
 */
int addResource(Namer namer, void *resource, const char *name, const char **nameData);

/**
 * @brief Unmaps a given resource to a given name.
 *
 * @returns The resource whose name was removed, or NULL if no resource
 * had such name.
 */
void *deleteResource(Namer namer, const char *name);

/**
 * @brief Gets a resource by it's name.
 *
 * @returns The named resource, or NULL if none was found with such name.
 */
void *getResource(Namer namer, const char *name);

#endif