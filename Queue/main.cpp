/**
 * @file main.cpp
 * @author Vladislav Skvortsov (vladislavskvo@gmail.com)
 * @brief Implementation of queue data structure with arbitrary number of elements of any type support (+ basic interface)
 * @version 0.1
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <assert.h> // for assert
#include <stdio.h>  // for printf && putchar
#include <stdlib.h> // for calloc && realloc
#include <string.h> // for memcpy && memmove

struct queue
{
    char *data;
    size_t elem_size;

    size_t size;
    size_t capacity;

    size_t headIdx;
    size_t tailIdx;
};

const int DEFAULT_QUEUE_CAPACITY    = 10;
const int POISON                    = 0xDEAD;


struct queue *queue_new(size_t elem_size)
{
    // Construction of `queue` structure
    struct queue *q = (struct queue *) calloc(1, sizeof(struct queue));
    assert(q != NULL);

    q->data = (char *) calloc(DEFAULT_QUEUE_CAPACITY, elem_size);
    assert(q->data != NULL);

    // Fill `queue` structure fields
    q->size         = 0;
    q->capacity     = DEFAULT_QUEUE_CAPACITY;
    q->headIdx      = 0;
    q->tailIdx      = 0;
    q->elem_size    = elem_size;

    return q;
}

struct queue *queue_delete(struct queue *q)
{
    // Error check
    assert(q != NULL && q->data != NULL);

    // Destruction of `queue` data structure
    free(q->data);
    
    q->size         = POISON;
    q->capacity     = POISON;
    q->headIdx      = POISON;
    q->tailIdx      = POISON;
    q->elem_size    = POISON;

    free(q);

    return NULL;
}

static int queue_reallocation(struct queue *q, size_t new_queue_capacity)
{
    // Error check
    assert(q != NULL);

    // Reallocation
    char *tmpData = (char *) realloc(q->data, new_queue_capacity * q->elem_size);
    if (tmpData == NULL)
    {
        return 1;
    }

    q->data = tmpData;
    q->capacity = new_queue_capacity;

    return 0;
}

int queue_push(struct queue *q, const void *elem)
{
    // Error check
    if (q == NULL || q->data == NULL || elem == NULL || q->size > q->capacity)
    {
        return 1;
    }

    // Reallocation check
    if (q->size == q->capacity - 1)
    {
        int ret = queue_reallocation(q, 2 * q->capacity + 1);
        if (ret)
        {
            return 1;
        }
    }

    // Push
    memcpy(&( q->data[q->elem_size * q->headIdx] ), elem, q->elem_size);
    ++q->size;
    ++q->headIdx;

    return 0;
}

int queue_pop(struct queue *q, void *elem)
{
    // Error check
    if (q == NULL || q->data == NULL || elem == NULL || q->size == 0)
    {
        return 1;
    }

    // Reallocation check
    if (q->size <= (q->capacity / 2 + (q->capacity & 1)))
    {
        // Shifting queue elements
        memmove(q->data, q->data + q->elem_size * q->tailIdx, q->elem_size * (q->headIdx - q->tailIdx));
        q->headIdx = q->headIdx - q->tailIdx;
        q->tailIdx = 0;

        int ret = queue_reallocation(q, q->capacity / 2 + (q->capacity & 1));
        if (ret)
        {
            return 1;
        }
    }

    // Popping
    memcpy(elem, &( q->data[q->elem_size * q->tailIdx] ), q->elem_size);
    ++q->tailIdx;
    --q->size;

    return 0;
}

int queue_empty(struct queue const *q)
{
    return !q->size;
}

void queue_print(struct queue const *q, void (*pf)(void const *data))
{
    // Error check
    assert(q != NULL);

    // Printing
    putchar('[');
    if (q->size)
    {
        for (int i = 0; i < q->size - 1; ++i)
        {
            pf(&( q->data[q->elem_size * (q->headIdx - 1 - i)] ));
            printf(", ");
        }
        pf(&( q->data[q->elem_size * (q->headIdx - q->size)] ));

    }
    printf("]\n");

    return;
}


void print_element(const void *element)
{
    printf("%d", *((int *) element));
}

int main()
{
    struct queue *q = queue_new(sizeof(int));

    for (int i = 0; i < 12; ++i)
    {
        queue_push(q, &i);
    }
    queue_print(q, print_element);
    printf("queue size: %d\nqueue capacity: %d\n\n", q->size, q->capacity);

    int elem = 0;
    for (int i = 0; i < 5; ++i)
    {
        queue_pop(q, &elem);
    }
    queue_print(q, print_element);
    printf("queue size: %d\nqueue capacity: %d\n", q->size, q->capacity);
    printf("%d\n\n", queue_empty(q));

    for (int i = 0; i < 9; ++i)
    {
        queue_pop(q, &elem);
    }
    queue_print(q, print_element);
    printf("%d\n", queue_empty(q));

    q = queue_delete(q);

    return 0;
}

/**
 * @brief   queue_empty - O(1)
 *          queue_pop   - O(n), since memmove function asymptotic (in big-O notation) is O(n)
 *          queue_push  - O(1),
 *          since the number of operations is proportional to the number of bytes that the stack element represents.
 * 
 */
