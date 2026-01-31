# Memory Allocation Utility Functions

SimpleGUI offers wrappers for memory allocation functions from
`stdlib.h` which check for when a NULL pointer is returned,
and will `exit()` the application if a memory allocation
fails.
They also keep track of a counter of how many allocs and frees
happened for debugging purposes.

## Wrapper for malloc

`void *sg_malloc(size_t size);`

Wrapper for `malloc`. Always returns valid pointer.
Increments alloc count.

## Wrapper for calloc

`void *sg_calloc(size_t count, size_t size);`

Wrapper for `calloc`. Always returns valid pointer.
Increments alloc count.

## Wrapper for realloc

`void *sg_realloc(void *p, size_t size);`

Wrapper for `realloc`. Always returns valid pointer.
Increments alloc count if `p` is `NULL`.

## Wrapper for free

`void sg_free(void *p);`

Free pointer received from `sg_malloc`, `sg_calloc` or `sg_realloc`.
Increments free count if `p` is not `NULL`.

## Print the number of allocs and frees done with these functions

`void sg_alloc_report(void);`

Useful to make sure that all memory allocations are freed.
Prints number of allocs and frees to stdout.
