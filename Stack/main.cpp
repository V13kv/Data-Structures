/**
 * @file main.cpp      
 * @author Vladislav Skvortsov
 * @brief Implementation of stack structure with arbitrary number of elements of any type support (+ basic interface)
 * @version 0.1
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <assert.h> // for assert
#include <stddef.h> // for size_t
#include <stdio.h>  // for putchar && printf
#include <stdlib.h> // for calloc && realloc
#include <string.h> // for memcpy && memset

struct stack
{
    char *elems;
    size_t elem_size;

    int stack_size;
    int stack_capacity;
};

enum STACK_ERRORS
{
    NO_ERRORS,
    NULL_PTR_IS_PASSED,
    BAD_STD_FUNC_RESULT,
    STACK_REALLOCATION_ERROR,
};

static const size_t MIN_STACK_CAPACITY  = 10;
static const int    REALLOC_COEFF       = 2;

struct stack *stack_new(size_t elem_size)
{
    // Error check
    assert(elem_size > 0 && "new stack size must be greater than zero!");

    // Allocating memory for stack
    struct stack *st = (struct stack *) calloc(1, sizeof(struct stack));
    assert (st != NULL && "error during stlib calloc function!");

    // Set stack fields
    st->elem_size = elem_size;  // size of each stack object
    st->elems = (char *) calloc(MIN_STACK_CAPACITY, st->elem_size); // allocating memory (in bytes)
    st->stack_capacity = MIN_STACK_CAPACITY;
    st->stack_size = 0;

    return st;
}

struct stack *stack_delete(struct stack *st)
{
    // Error check
    assert(st != NULL && "passed object st is nullptr!");

    // Destruction
    free(st->elems);
    free(st);

    return NULL;
}

static enum STACK_ERRORS stack_realloc(struct stack *st)
{
    // Error check
    if (st == NULL)
    {
        return NULL_PTR_IS_PASSED;
    }

    // Reallocate && check for realloc error
    char *temp = (char *) realloc(st->elems, REALLOC_COEFF * st->stack_capacity * st->elem_size);
    if (temp == NULL)
    {
        st = stack_delete(st);
        return STACK_REALLOCATION_ERROR;
    }

    // Update stack fields
    st->elems = temp;
    st->stack_capacity = REALLOC_COEFF * st->stack_capacity;

    return NO_ERRORS;
}

int stack_push(struct stack *st, const void *elem)
{
    // Error check
    if (st == NULL || elem == NULL)
    {
        return 1;
    }

    // Check for reallocation before push
    if (st->stack_size == st->stack_capacity)
    {
        // Reallocate stack elems && check for errors
        enum STACK_ERRORS ret = stack_realloc(st);
        if (ret != NO_ERRORS)
        {
            return 1;
        }
    }

    // Push
    memcpy(&st->elems[st->stack_size * st->elem_size], elem, st->elem_size);

    // Show that new element is added
    ++st->stack_size;

    return 0;    
}

int stack_pop(struct stack *st, void *elem)
{
    // Error check
    if (st == NULL || elem == NULL || st->stack_size == 0)
    {
        return 1;
    }

    // Pop && error check
    size_t beginning = (st->stack_size - 1) * st->elem_size;  // beginning (in bytes) of element that will be popped
    memcpy(elem, &st->elems[beginning], st->elem_size);

    // Erase popped stack value (fill with 0's)
    memset(&st->elems[beginning], 0x00, st->elem_size);

    // Show that element is popped
    --st->stack_size;

    return 0;
}

int stack_top(struct stack *st, void *elem)
{
    // Error check
    if (st == NULL || elem == NULL)
    {
        return 1;
    }

    // Extract top element
    size_t beginning = (st->stack_size - 1) * st->elem_size;
    memcpy(elem, &st->elems[beginning], st->elem_size);

    return 0;
}

int stack_empty(struct stack const *st)
{
    // Error check
    assert(st != NULL && "Passed object is nullptr!");

    return st->stack_size == 0;
}

void stack_print(struct stack const *st, void (*pf)(void const *st))
{
    // Error check
    assert(st != NULL && pf != NULL && "passed objects are nullptr's!");

    // Printing
    putchar('[');
    
    if (st->stack_size > 0)
    {
        // Print each stack object in `fp` way (the last object in stack (the bottom one) is a special case)
        for (int i = 0; i < st->stack_size - 1; ++i)
        {
            (*pf)( (void *)(&st->elems[i * st->elem_size]) );
            
            printf(", ");
        }

        // Proceed the last object in stack (the bottom one)
        (*pf)( (void *)(&st->elems[(st->stack_size - 1) * st->elem_size]) );
    } 

    printf("]\n");
}

//------------------------------------------------------TESTING-------------------------------------------------------

static void print_double(void const *st)
{
   printf("%lf", *(double *)st);
}

// Should print 81.000000
//              64.000000
//              0
//              [0.000000, 1.000000, 4.000000, 9.000000, 16.000000, 25.000000, 36.000000, 49.000000, 64.000000]

int main()
{
    struct stack *st = stack_new(sizeof (double));
    for (int i = 0; i < 10; i++)
    {
        double tmp = i * i;
        stack_push(st, &tmp);
    }
    double tmp = 0;
    stack_pop(st, &tmp);
    printf("%lf\n", tmp);

    stack_top(st, &tmp);
    printf("%lf\n", tmp);

    printf("%d\n", stack_empty(st));

    stack_print(st, print_double);
    st = stack_delete(st);
}

/**
 * @brief   stack_push is O(1)
 *          stack_pop is O(1)
 *          stack_top is O(1), 
 *          since the number of operations is proportional to the number of bytes that the stack element represents.
 * 
 */
