#ifndef LIST_H_128426742367468236895030498593453
#define LIST_H_128426742367468236895030498593453

#define NULL_NODE                       ((list_node *)0)

#define list_get_data_from_node(node)   node->data
#define list_get_count(header)          header->count
#define list_foreach(header, node)      for (node = header->head; \
                                            node != ((list_node *)0); \
                                            node = node->next)

typedef void list_data;

typedef void (*free_data_func)(list_data *);

/* a node */
typedef struct LIST_NODE {
    int index;
    
    struct LIST_NODE *next;
    struct LIST_NODE *prev;
    list_data        *data;
} list_node;

/* the header (think of it as the list) */
typedef struct LIST_HEADER {
    int count;

    list_node *head;
    list_node *tail;

    free_data_func   free_func;
} list_header;

list_header *list_init(free_data_func free_func);
void list_destroy(list_header *header);
int list_append_node(list_header *header, list_data *data);
int list_prepend_node(list_header *header, list_data *data);
list_data *list_get_data_by_index(list_header *header, int index);

#endif
