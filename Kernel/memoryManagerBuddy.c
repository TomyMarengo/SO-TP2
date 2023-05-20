//Initial code from https://github.com/evanw/buddy-malloc
#ifndef CUSTOM

#include <memoryManager.h>

// Defining macros
#define HEAP_START 0x800000 // Starting address of the heap
#define HEADER_SIZE 8 // Size of memory block header
#define MIN_ALLOC_LOG2 4 // Log2 of minimum block size 
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2) // Minimum block size
#define MAX_ALLOC_LOG2 21 // Log2 of maximum block size
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2) // Maximum block size
#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1) // Number of buckets

// Macros for heap tree operations
#define PARENT(i) ((i - 1) / 2) // Parent index in the binary heap tree
#define LEFT_CHILD(i) (i)*2 + 1 // Left child index in the binary heap tree
#define RIGHT_CHILD(i) (i)*2 + 2 // Right child index in the binary heap tree
#define SIBLING(i) (((i) - 1) ^ 1) + 1 // Sibling index in the binary heap tree

// Structure for doubly linked list
typedef struct list_t {
  struct list_t *prev, *next; // Pointers to previous and next elements
} list_t;

// Global variables
static list_t buckets[BUCKET_COUNT]; // Buckets for managing memory blocks
static size_t bucket_limit; // The size limit for a bucket
static uint8_t node_is_split[(1 << (BUCKET_COUNT - 1)) / 8]; // Array indicating if a node is split in the heap
static uint8_t *base_ptr; // Pointer to the base of the heap
static size_t free_bytes_remaining = MAX_ALLOC; // Total free bytes in the heap

// Function for initializing list
static void list_init(list_t *list) {
  list->prev = list; // Set previous pointer to itself
  list->next = list; // Set next pointer to itself
}

// Function for pushing an element to the list
static void list_push(list_t *list, list_t *entry) { 
  list_t *prev = list->prev; // Get the previous element
  entry->prev = prev; // Update previous pointer of the new entry
  entry->next = list; // Update next pointer of the new entry
  prev->next = entry; // Set next pointer of the last element to the new entry
  list->prev = entry; // Set previous pointer of the list to the new entry
}

// Function for removing an element from the list
static void list_remove(list_t *entry) {
  list_t *prev = entry->prev; // Get the previous element
  list_t *next = entry->next; // Get the next element
  prev->next = next; // Set next pointer of the previous element to the next element
  next->prev = prev; // Set previous pointer of the next element to the previous element
}

// Function for popping an element from the list
static list_t *list_pop(list_t *list) {
  list_t *back = list->prev; // Get the last element
  if (back == list) // If the list is empty
    return NULL;
  list_remove(back); // Remove the last element
  return back; // Return the last element
}

// Function for getting the pointer for a node
static uint8_t *ptr_for_node(size_t index, size_t bucket) {
  return base_ptr + ((index - (1 << bucket) + 1) << (MAX_ALLOC_LOG2 - bucket)); // Compute the address
}

// Function for getting the node for a pointer
static size_t node_for_ptr(uint8_t *ptr, size_t bucket) {
  return ((ptr - base_ptr) >> (MAX_ALLOC_LOG2 - bucket)) + (1 << bucket) - 1; // Compute the node index
}

// Function for checking if parent node is split
static int parent_is_split(size_t index) {
  index = PARENT(index); // Get the parent index
  return (node_is_split[index / 8] >> (index % 8)) & 1; // Check if the parent is split
}

// Function for flipping the split status of a parent node
static void flip_parent_is_split(size_t index) {
  index = PARENT(index); // Get the parent index
  node_is_split[index / 8] ^= 1 << (index % 8); // Flip the bit
}

// Function for getting the bucket for a request
static size_t bucket_for_request(size_t request) {
  size_t bucket = BUCKET_COUNT - 1; // Start from the last bucket
  size_t size = MIN_ALLOC; // Start from the minimum allocation size
  while (size < request) { // While size is less than request
    bucket--; // Move to the previous bucket
    size *= 2; // Double the size
  }
  return bucket; // Return the bucket for the request
}

// Function for lowering the bucket limit
static int lower_bucket_limit(size_t bucket) {
  while (bucket < bucket_limit) {
    size_t root = node_for_ptr(base_ptr, bucket_limit);
    uint8_t *right_child;
    if (!parent_is_split(root)) {
      list_remove((list_t *)base_ptr);
      list_init(&buckets[--bucket_limit]);
      list_push(&buckets[bucket_limit], (list_t *)base_ptr);
      continue;
    }
    right_child = ptr_for_node(root + 1, bucket_limit);
    if (right_child + sizeof(list_t) > base_ptr + MAX_ALLOC) {
      return 0;
    }
    list_push(&buckets[bucket_limit], (list_t *)right_child);
    list_init(&buckets[--bucket_limit]);
    root = PARENT(root);
    if (root != 0) 
        flip_parent_is_split(root);
  }
  return 1;
}

// Function for initializing the heap
static void initialize_heap() {
    base_ptr = (uint8_t *) HEAP_START; // Set base pointer to the heap start
    bucket_limit = BUCKET_COUNT - 1; // Set bucket limit to the total bucket count minus 1
    list_init(&buckets[BUCKET_COUNT - 1]); // Initialize the last bucket
    list_push(&buckets[BUCKET_COUNT - 1], (list_t *)base_ptr); // Push the base pointer to the last bucket
}

// Function for allocating memory
void * my_malloc(size_t request) {
  size_t original_bucket, bucket;

  // Check if the request size is larger than maximum allocation size
  if (request + HEADER_SIZE > MAX_ALLOC)
    return NULL;

  // Initialize heap if it's not initialized yet
  if (base_ptr == NULL)
    initialize_heap();

  // Find the bucket for the request
  bucket = bucket_for_request(request + HEADER_SIZE);
  original_bucket = bucket;

  while (bucket + 1 != 0) {
    size_t i;
    uint8_t *ptr;

    if (!lower_bucket_limit(bucket)) 
      return NULL;
    
    ptr = (uint8_t *)list_pop(&buckets[bucket]);
    if (ptr == NULL) {
      if (bucket != bucket_limit || bucket == 0) {
        bucket--;
        continue;
      }

      if (!lower_bucket_limit(bucket - 1)) 
        return NULL;

      ptr = (uint8_t *)list_pop(&buckets[bucket]);
    }

    i = node_for_ptr(ptr, bucket);
    if (i != 0) {
      flip_parent_is_split(i);
    }

    while (bucket < original_bucket) {
      i = LEFT_CHILD(i);
      bucket++;
      flip_parent_is_split(i);
      list_push(&buckets[bucket], (list_t *)ptr_for_node(i + 1, bucket)); 
    }

    free_bytes_remaining -= (1 << (BUCKET_COUNT + 1 - bucket));

    *(size_t *)ptr = request;
    return ptr + HEADER_SIZE;
  }

  return NULL;
}

// Function for freeing memory
int my_free(void *ptr) {
  size_t bucket, i;

  // If the pointer is NULL, then there is nothing to free
  if (ptr == NULL)
    return 1;
  
  ptr = (uint8_t *)ptr - HEADER_SIZE;
  bucket = bucket_for_request(*(size_t *)ptr + HEADER_SIZE);
  i = node_for_ptr((uint8_t *)ptr, bucket);

  while (i != 0) {
    flip_parent_is_split(i);

    if (parent_is_split(i) || bucket == bucket_limit)
      break;
    
    list_remove((list_t *)ptr_for_node(SIBLING(i), bucket));
    i = PARENT(i);
    bucket--;
  }
  free_bytes_remaining += (1 << (BUCKET_COUNT + 1 - bucket));
  list_push(&buckets[bucket], (list_t *)ptr_for_node(i, bucket));

  return 0;
}

void my_init(void* memoryStart, size_t memorySize)
{
  return;
}

// Function for getting memory information
void mem_info(){
    scr_print("Total memory: ");
    scr_print(" bytes\n");

    scr_print("Memory in use: ");
    scr_print(" bytes\n");

    scr_print("Free memory: ");
    scr_print(" bytes\n");
}
#endif
