#include <stdint.h>


#define MALLOC_BATCH 65536


typedef union node_t{
  union node_t *children[256];
  int present[256];
} node;

typedef node *tree;


struct node_list_t {
  struct node_list_t *next;
  node *node_array;
};


typedef struct node_list_t *node_list;


node_list init_node_list();

tree init_node();

void free_it_all();

int is_in(tree t, uint32_t n64);

int insert(tree t, uint8_t *n, int depth);
