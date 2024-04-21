#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  p->nil = nilNode;
  p->root = nilNode;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void rb_insert_fixup(rbtree *t, node_t **insert)
{
  // while (insert)
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  nilNode->color = RBTREE_BLACK;

  if (t->root == nilNode)
  {
    t->root->color = RBTREE_BLACK;
    t->root->key = key;

    return t->root;
  }
  else
  {
    node_t *cur = t->root;
    node_t *pre = t->nil;
    node_t *insert = t->nil;
    insert->key = key;

    while (cur != t->nil)
    {
      pre = cur;
      if (insert->key < cur->key) // 입력key가 cur보다 작으면 cur를 left child로 교체
      {
        cur = cur->left;
      }
      else  // 입력key가 cur보다 크면 cur를 right child로 교체
      {
        cur = cur->right;
      }
    }  

    insert->parent = pre; // 입력할 노드의 부모를 pre로 설정
    // 입력할 key가 pre 보다 작으면 insert 노드를 pre의 left child로 설정
    if (insert->key < pre->key) 
    {
      pre->left = insert;
    }
    // 입력할 key가 pre 보다 크거나 같으면 insert 노드를 pre의 right child로 설정
    else
    {
      pre->right = insert;
    }
    // leaf 노드를 위한 초기화
    insert->left = t->nil;
    insert->right = t->nil;
    insert->color = RBTREE_RED;
  }

  // rb_insert_fixup(t,insert);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
