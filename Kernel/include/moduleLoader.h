#ifndef _MODULELOADER_H_
#define _MODULELOADER_H_

/**
 * @brief Initializes kernel binaries.
 *
 * @param payloadStart Address to begin the load.
 * @param moduleTargetAddress Addresses of the modules to load.
 */
void loadModules(void *payloadStart, void **moduleTargetAddress);

#endif