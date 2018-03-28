#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#include "search_tree.h"


/* global vars */
node_list head, current;
size_t arr_index;


/* init  & free functions */
node_list init_node_list() {
  node_list new = malloc(sizeof(struct node_list_t));
  new->node_array = malloc(MALLOC_BATCH*sizeof(node));
  new->next = NULL;
  arr_index = 0;
  if (current) {
    current->next = new;
    current = new;
  }
  return new;
}

tree init_node() {
  if (arr_index == MALLOC_BATCH-1) {
    current->next = init_node_list();
  }
  memset((void *) &(current->node_array[arr_index]), 0, sizeof(node));
  return &(current->node_array[arr_index++]);
}

void free_it_all() {
  do {
    current = head->next;
    free(head->node_array);
    free(head);
    head = current;
  } while (head != NULL);
}


/* insert a uint32 in the tree and checks for collisions */
int is_in(tree t, uint32_t n64) {
  uint8_t n[4];
  memcpy((void *)n, (void *)&n64, sizeof(uint32_t));
  //*((uint64_t *)n) = n64;
  return insert(t, n, 0);
}

int insert(tree t, uint8_t *n, int depth) {
  if (depth < 3) {
    if (t->children[n[depth]] == NULL){
      t->children[n[depth]] = init_node();
    }
    return insert(t->children[n[depth]], n, depth+1);
  } else {
    return t->present[n[3]]++;
  }
}
