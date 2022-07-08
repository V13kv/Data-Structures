/**
 * @file main.cpp
 * @author Vladislav Skvortsov
 * @brief Implementation of vector data structure with arbitrary number of elements of any type support (+ basic interface)
 * @version 0.1
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <assert.h> // for assert
#include <stddef.h> // for size_t
#include <stdio.h>  // for printf
#include <stdlib.h> // for malloc (not calloc because calloc inits elems with 0's, the task says not to init them)
#include <string.h> // for memcpy

struct vector
{
    void *elems;
    size_t elem_size;

    size_t size;
    size_t capacity;
};

struct vector *vector_new(size_t elems, size_t elem_size)
{
    // Allocate memory for structure
    struct vector *v = (struct vector *) malloc(sizeof(struct vector));
    assert(v != NULL);

    // Initialize basic vector fields
    v->elems = (void *) malloc(elem_size * elems); // allocating bytes for vector elements
    assert(v->elems != NULL);
    
    v->size         = elems;
    v->capacity     = elems;
    v->elem_size    = elem_size;

    return v;
}

struct vector *vector_delete(struct vector *v)
{
    // Error check
    assert(v != NULL);

    // Delete vector object
    free(v->elems);
    free(v);

    return NULL;
}

int vector_resize(struct vector *v, size_t new_size)
{
    // Error check
    assert(v != NULL);

    if (v->elems == NULL)
    {
        return 1;
    }

    // Resize only if new_size is greater than current vector capacity
    if (new_size > v->capacity)
    {
        void *new_data_location = (void *) realloc(v->elems, new_size * v->elem_size);
        if (new_data_location == NULL)
        {
            return 1;
        }

        v->elems    = new_data_location;
        v->capacity = new_size;
    }
    v->size = new_size;

    return 0;
}

int vector_set(struct vector *v, size_t index, void const *elem)
{
    // Error checks
    assert(v != NULL && elem != NULL);

    if (v->elems == NULL || index >= v->size)
    {
        return 1;
    }

    // Set new value of `index` element
    memcpy(&( ((char *) v->elems)[v->elem_size * index] ), elem, v->elem_size);  

    return 0; 
}

int vector_get(struct vector const *v, size_t index, void *elem)
{
    // Error check
    assert(v != NULL && elem != NULL);

    if (v->elems == NULL || index >= v->size)
    {
        return 1;
    }

    // Get vector `index` element
    memcpy(elem, & ((char *) v->elems)[index * v->elem_size], v->elem_size);

    return 0;
}

int vector_push(struct vector *v, void const *elem)
{
    // Error check
    assert(v != NULL && elem != NULL);

    if (v->elems == NULL)
    {
        return 1;
    }

    // Check for reallocation
    if (v->size == v->capacity)
    {
        void *new_data_location = (void *) realloc(v->elems, (2 * v->capacity + 1) * v->elem_size);
        if (new_data_location == NULL)
        {
            return 1;
        }

        v->elems = new_data_location;
        v->capacity = 2 * v->capacity + 1;
    }

    // Push new element
    memcpy( &( ((char *) v->elems)[v->size * v->elem_size] ), elem, v->elem_size );
    v->size++;

    return 0;
}

int vector_pop(struct vector *v, void *elem)
{
    // Error check
    assert(v != NULL && elem != NULL);

    if (v->size == 0)
    {
        return 1;
    }

    // Popping
    memcpy(elem, &( ((char *) v->elems)[v->elem_size * --v->size] ), v->elem_size);
    
    // Erase popped stack value (fill with 0's)
    memset( &( ((char *) v->elems)[v->elem_size * v->size] ), 0x00, v->elem_size);

    // Check for reallocation
    if (2 * v->size <= v->capacity)
    {
        vector_resize(v, v->size);
    }

    return 0;
}

size_t vector_size(struct vector const *v)
{
    return v->size;
}

int vector_empty(struct vector const *v)
{
    return !v->size;
}

void vector_print(struct vector const *v, void (*pf)(void const *data))
{
    // Error check
    assert(v != NULL);

    // Printing
    putchar('[');
    if (v->size != 0)
    {
        int i = 0;
        while (i < v->size - 1)
        {
            pf( &((char *) v->elems)[i * v->elem_size] );
            printf(", ");

            ++i;
        }
        pf( &((char *) v->elems)[i * v->elem_size] );
    }
    printf("]\n");

    return;
}


static void print_int(void const *data)
{
   printf("%d", *(int *)data);
}

// Should print [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 123]
//              123
//              123
//              0
//              9
//              0
//              [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 63998, 1144873504, 586, 1144848720, 586, 1869044851, 1546937452, 1147498063, 1702259058]
//              20

int main()
{
    struct vector *v = vector_new(10, sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        vector_set(v, i, &i);
    }

    int elem = 123;
    vector_push(v, &elem);
    vector_print(v, print_int);

    vector_get(v, 10, &elem);
    printf("%d\n", elem);

    vector_pop(v, &elem);
    printf("%d\n", elem);
    printf("%d\n", ((char *) v->elems)[10]);

    vector_get(v, 9, &elem);
    printf("%d\n", elem);
    
    printf("%d\n", vector_empty(v));

    vector_resize(v, 2 * v->size);
    vector_print(v, print_int);
    printf("%d\n", vector_size(v));

    v = vector_delete(v);
}

/**
 * @brief   vector_push - O(1)
 *          vector_pop - O(1)
 *          vector_get - O(1)
 *          vector_set - O(1),
 *          since the number of operations is proportional to the number of bytes that the stack element represents.
 */
