#ifndef _NAMER_
#define _NAMER_

#include <defs.h>

/**
 * @brief Creates a new instance of a resource namer.
 *
 * @returns - The newly created resource namer instance, or NULL if the operation failed.
 */
Namer newNamer();

/**
 * @brief Releases all resources used by a resource namer.
 * Using a namer after it has been released leads to undefined behavior.
 *
 * @returns - 0 if the operation succeeded, or greater than 0 if not.
 */
int freeNamer(Namer namer);

/**
 * @brief Associates a specified resource with a given name. The namer creates an internal
 * copy of the name string, and if nameData is not NULL, it also stores a copy there.
 *
 * @returns - 0 if the operation is successful, 1 if the resource is already associated with a name,
 * or -1 if the operation fails for other reasons.
 */
int addResource(Namer namer, void *resource, const char *name, const char **nameData);

/**
 * @brief Removes the association between a specified resource and its corresponding name.
 *
 * @returns - The resource that was previously associated with the name, or NULL if no resource
 * was associated with the given name.
 */
void *deleteResource(Namer namer, const char *name);

/**
 * @brief Retrieves a resource by its name.
 *
 * @returns - The resource with the specified name, or NULL if no resource was found
 * with the given name.
 */
void *getResource(Namer namer, const char *name);

#endif