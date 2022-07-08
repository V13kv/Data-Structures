/**
 * @file main.cpp
 * @author Vladislav Skvortsov (vladislavskvo@gmail.com)
 * @brief Implementation of one-linked list data structure with arbitrary number of elements (+ basic interface)
 * @version 0.1
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <assert.h> // for assert
#include <stdio.h>  // for printf
#include <stdlib.h> // for calloc && free

struct list
{
    int data;

    struct list *next_node;
};

struct list *list_new(int elem)
{
    // Construction of `list` structure
    struct list *l = (struct list *) calloc(1, sizeof(struct list));
    assert(l != NULL);

    // Fill `list` fields
    l->data = elem;
    l->next_node = NULL;

    return l;
}

struct list *list_delete(struct list *head)
{
    // Deallocate all branch beginning from head
    while (head)
    {
        struct list *next_node = head->next_node;
        free(head);

        head = next_node;
    }

    return NULL;
}

struct list *list_insert(struct list *head, int elem)
{
    // If there is no head => create new list with one node with value `elem`
    if (!head)
    {
        return list_new(elem);
    }
    
    // Insertion (find last element of an list with head `head` and paste new list node in list with head `head`)
    struct list *listHead = head;
    while (head->next_node)
    {
        head = head->next_node;
    }
    head->next_node = list_new(elem);

    return listHead;
}

struct list *list_find(struct list *head, int elem)
{
    if (head)
    {
        while (head)
        {
            if (head->data == elem)
            {
                return head;
            }
            else
            {
                head = head->next_node;
            }
        }
    }

    return NULL;
}

struct list *list_erase(struct list *head, int elem)
{
    // Basic cases
    if (!head)
    {
        return NULL;
    }
    else if (head->data == elem)
    {
        struct list *newHead = head->next_node;
        free(head);

        return newHead;
    }

    // Save list head
    struct list *listHead = head;

    // Erasing
    while (head->next_node)
    {
        if (head->next_node->data == elem)
        {
            struct list *nodeToErase = head->next_node;
            head->next_node = head->next_node->next_node;
            free(nodeToErase);

            return listHead;
        }
        head = head->next_node;
    }

    return listHead;
}

struct list *list_insert_after(struct list *head, struct list *where, struct list *what)
{
    // Basic cases
    if (where == NULL || what == NULL)
    {
        return head;
    }
    else if (head == NULL)
    {
        return what;
    }

    // Inserting after element
    struct list *whereNode = list_find(head, where->data);
    what->next_node = whereNode->next_node;
    whereNode->next_node = what;

    return head;
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what)
{
    // Basic cases
    if (where == NULL || what == NULL)
    {
        return head;
    }
    else if (head == NULL)
    {
        return what;
    }
    else if (head == where)
    {
        what->next_node = where;

        return what;
    }

    // Inserting before element
    struct list *listHead = head;
    while (head->next_node)
    {
        if (head->next_node == where)
        {
            head->next_node = what;
            what->next_node = where;

            return listHead;
        }
        head = head->next_node;
    }

    return NULL;
}

struct list *list_next(struct list *curr)
{
    // Basic check
    if (!curr)
    {
        return NULL;
    }

    // Get next element
    return curr->next_node;
}

void list_print(struct list const *head)
{
    putchar('[');
    if (head)
    {
        while (head->next_node)
        {
            printf("%d, ", head->data);
            head = head->next_node;
        }
        printf("%d", head->data);
    }
    printf("]\n");

    return;
}

int main()
{
    struct list *head = list_new(0);
    head = list_insert(head, 1);
    head = list_insert(head, 2);
    head = list_insert(head, 35);
    head = list_insert(head, 4);
    list_print(head);

    printf("Before erasing: %d\n", list_find(head, 35)->data);
    list_erase(head, 35);
    printf("After erasing: %d\n", list_find(head, 4)->data);

    list_insert_after(head, list_new(4), list_new(5));
    list_print(head);
    printf("After inserting after: %d\n", list_find(head, 5)->data);


    head = list_delete(head);
    
    return 0;
}

/**
 * @brief   list_next           - O(1),
 *          list_insert_before  - O(n),
 *          list_insert_after   - O(n),
 *          list_erase          - O(n),
 *          list_find           - O(n),
 *          list_insert         - O(n),
 * 
 */
