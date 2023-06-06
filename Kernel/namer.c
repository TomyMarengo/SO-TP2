#include <defs.h>
#include <memoryManager.h>
#include <namer.h>
#include <string.h>

#define BUFFER_CHUNK_SIZE 8

typedef struct {
    char *name;
    void *resource;
} NamedResource;

struct NamerData {
    NamedResource *resources;
    int count;
    int bufferSize;
};

static int
isValidName(const char *name) {
    if (name == NULL)
        return 0;

    for (int i = 0; i <= MAX_NAME_LENGTH; i++) {
        char c = name[i];
        if (c == '\0') {
            return i > (name[0] == '/' ? 1 : 0);
        }

        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z')) {
            if (i == 0 ? (c != '/') : (c < '0' || c > '9'))
                return 0;
        }
    }

    return 0;
}

Namer
newNamer() {
    Namer namer = malloc(sizeof(struct NamerData));

    if (namer != NULL) {
        namer->resources = NULL;
        namer->count = 0;
        namer->bufferSize = 0;
    }

    return namer;
}

int
freeNamer(Namer namer) {
    return free(namer->resources) + free(namer);
}

static int
findResourceIndex(Namer namer, const char *name, int *c) {
    int i;
    *c = 1;
    for (i = 0; i < namer->count && (*c = strcmp(name, namer->resources[i].name)) < 0; i++)
        ;
    return i;
}

int
addResource(Namer namer, void *resource, const char *name, const char **nameData) {
    if (!isValidName(name))
        return 1;

    int c;
    int index = findResourceIndex(namer, name, &c);

    // If a resource with that name is already present, return 1.
    if (c == 0)
        return 1;

    char *nameCopy = malloc(strlen(name) + 1);
    if (nameCopy == NULL)
        return -1;

    // Expand the buffer if necessary.
    if (namer->count == namer->bufferSize) {
        size_t newBufferSize = namer->bufferSize + BUFFER_CHUNK_SIZE;
        NamedResource *newBuffer = realloc(namer->resources, newBufferSize * sizeof(NamedResource));
        if (newBuffer == NULL) {
            free(nameCopy);
            return -1;
        }

        namer->resources = newBuffer;
        namer->bufferSize = newBufferSize;
    }

    for (int i = namer->count; i > index; i--)
        namer->resources[i] = namer->resources[i - 1];

    namer->resources[index].name = nameCopy;
    namer->resources[index].resource = resource;
    namer->count++;
    strcpy(nameCopy, name);
    if (nameData != NULL)
        *nameData = nameCopy;
    return 0;
}

void *
deleteResource(Namer namer, const char *name) {
    int c;
    int index = findResourceIndex(namer, name, &c);
    void *resource = namer->resources[index].resource;

    if (c != 0)
        return NULL;

    free(namer->resources[index].name);
    namer->count--;

    for (; index < namer->count; index++)
        namer->resources[index] = namer->resources[index + 1];

    return resource;
}

void *
getResource(Namer namer, const char *name) {
    int c;
    int index = findResourceIndex(namer, name, &c);
    return c == 0 ? namer->resources[index].resource : NULL;
}