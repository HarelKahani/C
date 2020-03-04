#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "slist.h"

slist_node_t *init_node(void *data, slist_node_t *next)
{
    slist_node_t *n_head = (slist_node_t *)malloc(sizeof(slist_node_t));
    if (n_head == NULL)
    {
        printf("node allocation failure");
        return NULL;
    }
    slist_data(n_head) = data;
    slist_next(n_head) = next;
    return n_head;
}
void slist_init(slist_t *list)
{
    slist_head(list) = NULL;
    slist_tail(list) = NULL;
    slist_size(list) = 0;
    
}

void slist_destroy(slist_t *list, slist_destroy_t flag)
{
    if (slist_head(list) == NULL)
    {
        return;
    }

    slist_node_t *temp = slist_head(list);
    while (temp != NULL)
    {
        slist_head(list) = slist_next(temp);
        if (flag != 0)
        {
            free(slist_data(temp)); //if data allocate
        }
        free(temp);
        temp = slist_head(list);
        slist_size(list)--;
    }
    slist_tail(list) = NULL; //making sure tail has no garbage value
}

int slist_append(slist_t *list, void *data)
{
    if (list == NULL) //if allocation in main failed
    {
        return -1;
    }

    //if init been done
    if (slist_head(list) == NULL)
    {
        slist_node_t *new_node = init_node(data, NULL);
        if (new_node == NULL)
        {
            return -1;
        }
        slist_head(list) = new_node;
        slist_tail(list) = new_node;
        slist_size(list)++;
        return 0;
    }
    //if list is allready exist
    slist_node_t *new_node = init_node(data, NULL);
    if (new_node == NULL)
    {
        return -1;
    }
    slist_next(slist_tail(list)) = new_node; //setting new node at enad of list
    slist_tail(list) = new_node;
    slist_size(list)++;
    return 0;
}

int slist_prepend(slist_t *list, void *data)
{

    if (list == NULL)
    {
        return -1;
    }
    //if init been done
    if (slist_head(list) == NULL)
    {
        slist_node_t *new_node = init_node(data, NULL);
        if (new_node == NULL)
        {
            return -1;
        }
        slist_tail(list) = new_node;
        slist_head(list) = new_node;
        slist_size(list)++;
        return 0;
    }
    //if list is allready exist
    slist_node_t *new_node = init_node(data, slist_head(list));
    if (new_node == NULL)
    {
        return -1;
    }
    slist_head(list) = new_node;
    slist_size(list)++;
    return 0;
}

void *slist_pop_first(slist_t *list)
{
    if (slist_head(list) == NULL)
    {
        return NULL;
    }
    void *data = slist_data(slist_head(list)); //saving data before freeing
    slist_node_t *temp = slist_head(list);
    slist_head(list) = slist_next(temp);
    slist_size(list) = slist_size(list) - 1;
    free(temp);
    return data;
}

int slist_append_list(slist_t *list1, slist_t *list2)
{
    if (slist_head(list1) == NULL && slist_head(list2) == NULL) //if lists empty
    {
        return 0;
    }

    if (slist_head(list1) != NULL && slist_head(list2) == NULL) //nothing to union
    {
        return 0;
    }

    if (slist_head(list1) == NULL && slist_head(list2) != NULL) //if list1 empty-init and union
    {
        slist_init(list1);
    }

    slist_node_t *curr = slist_head(list2);
    slist_node_t *next = slist_head(list2);
    while (curr != NULL)
    {
        next = slist_next(curr);
        slist_append(list1, slist_data(curr));
        curr = next;
    }
    return 0;
}

