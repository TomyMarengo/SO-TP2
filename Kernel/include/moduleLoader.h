#ifndef _MODULE_LOADER_H
#define _MODULE_LOADER_H

#include <defs.h>

/**
 * @brief Initializes kernel binaries.
 *
 * @param payloadStart Address to begin the load.
 * @param moduleTargetAddress Addresses of the modules to load.
 */
void loadModules(void *payloadStart, void **moduleTargetAddress);

#endif