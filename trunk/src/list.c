#include <stdio.h>
#include <stdlib.h>

#include "list.h"

static void shift_index_right(list_node *node, int incr) {
    list_node *tmp_node = node;

    if (! node || incr == 0) return;

    /* from the node given add incr to all index vals */
    while (tmp_node) {
        tmp_node->index += incr;
        tmp_node = tmp_node->next;
    }
}

static list_node *get_node_by_index(list_header *header, int index) {
    list_node *node;
    
    list_foreach(header, node) {
        if (node->index == index)
            return (node);
    }

    return (NULL_NODE);
}

list_header *list_init(free_data_func free_func) {
    list_header *header;

    header = (list_header *)malloc(sizeof(list_header));

    header->head  = NULL_NODE;
    header->tail  = NULL_NODE;
    header->count = 0;

    header->free_func = free_func;

    return (header);
}

void list_destroy(list_header *header) {
    list_node *current_node;
    list_node *next_node;

    current_node = header->head;
    
    /* loop the nodes calling free_func() and then free() */
    while (current_node) {
        /* if free_func is defined then exec it */
        if (header->free_func)
            header->free_func(current_node->data);
        
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(header);

    header = ((list_header *)0);
}

list_data *list_get_data_by_index(list_header *header, int index) {
    list_node *node = get_node_by_index(header, index);

    if (node != ((list_data *)0))
        return ((list_data *)0);

    return (node->data);
}

int list_del_node_by_index(list_header *header, int index) {
    list_node *node;
    
    /* check constraints for index */
    if (index < 0 || index > header->count)
        return (0);

    node = get_node_by_index(header, index);

    /* update the index values */
    shift_index_right(node, -1);

    /* my head hurts */
    if (node->prev != NULL_NODE) {
        if (node->next != NULL_NODE) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        } else {
            node->prev->next = NULL_NODE;
            header->tail     = node->prev;
        }
    } else {
        if (node->next != NULL_NODE) {
            node->next->prev = NULL_NODE;
            header->head     = node->next;
        } else {
            header->head = NULL_NODE;
            header->tail = NULL_NODE;
        }
    }

    /* call the user defined free function and then free() itself */
    if (header->free_func)
        header->free_func(node->data);

    free(node);

    header->count--;

    return (1);
}

static list_node *list_get_new_node(list_header *header, void *data) {
    list_node *node;

    /* allocate space for the actual node */
    node = (list_node *)malloc(sizeof(list_node));

    /* if malloc fail inform the user and return null */
    if (node == NULL_NODE) {
        fprintf(stderr, "malloc failed when creating new node.\n");
        return (NULL_NODE);
    }

    node->data  = data;
    node->prev  = NULL_NODE;
    node->next  = NULL_NODE;
    node->index = -1;

    return (node);
}

list_node *list_prepend_node(list_header *header, void *data) {
    list_node *node;

    /* get our new node first */
    if ((node = list_get_new_node(header, data)) == NULL_NODE)
        return (NULL_NODE);

    /* first node needs special treatment */
    if (header->head == NULL_NODE)
        header->tail = node;
    else {
        node->next       = header->head;
        node->next->prev = node;
        shift_index_right(header->head, 1);
    }

    node->index  = 0;
    header->head = node;
    header->count++;
    
    return (node);
}

list_node *list_append_node(list_header *header, void *data) {
    list_node *node;

    /* get our new node first */
    if ((node = list_get_new_node(header, data)) == NULL_NODE)
        return (NULL_NODE);

    /* first node needs special treatment */
    if (header->head == NULL_NODE) {
        header->head = node;
        node->index  = 0;
    } else {
        header->tail->next = node;
        node->index        = header->tail->index + 1;
    }

    header->count++;

    node->prev   = header->tail;
    header->tail = node;

    return (node);
}
