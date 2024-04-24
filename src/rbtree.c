#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;

  return p;
}

// 동적 메모리 할당 해제
// rbtree를 후위순회하며 메모리에 할당된 모든 node를 free
void postorder_delete_rbtree(rbtree *t, node_t *x) {
  if (x != t->nil) {
    postorder_delete_rbtree(t, x->left);
    postorder_delete_rbtree(t, x->right);
    free(x);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  postorder_delete_rbtree(t, t->root);
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  
  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = y;
  }
  else if (x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;
  
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = y;
  }
  else if (x == x->parent->right) {
    x->parent->right = y;
  }
  else {
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
}

void rb_insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  // 앞서 z의 color를 RED로 설정했기 때문에, z's parent must be not RED
  {
    if (z->parent == z->parent->parent->left) {
      // z->parent가 z->parent->parent의 왼쪽 자리에 있을 때
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {
        // case 2 해결
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else { 
        if (z == z->parent->right) {
          // case 3 해결
          z = z->parent;
          left_rotate(t,z);
        }
        // case 4 해결
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    else {
      // z->parent가 z->parent->parent의 오른쪽 자리에 있을 때
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        // case 2 해결
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if (z == z->parent->left) {
          // case 3 해결
          z = z->parent;
          right_rotate(t,z);
        }
        // case 4 해결
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
  }
  // case 1 해결
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  // z is target node
  node_t *y = t->nil;
  // y's roll is pre_node
  node_t *x = t->root;
  // x's roll is current_node
  
  while (x != t->nil) {
    // x가 leaf(nil)에 도달할 때까지 반복
    y = x;
    if (key < x->key)
      x = x->left;
    else 
      x = x->right;
  }

  z->key = key;
  z->parent = y;
  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else 
    y->right = z;
  
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  
  rb_insert_fixup(t,z);

  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find

  node_t *cur = t->root;

  while (cur != t->nil)
  {
    if (key == cur->key)
      return cur;
    else if (key < cur->key) {
      cur = cur->left;
    }
    else {
      cur = cur->right;
    }
  }
  return NULL;
} 

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find

  node_t *cur = t->root;
  if (t->root == t->nil)
    return NULL;
  else {
    while (cur->left != t->nil)
    {
      cur = cur->left;
    }
  }  
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  
  node_t *cur = t->root;
  if (t->root == t->nil)
    return NULL;
  else {
    while (cur->right != t->nil)
    {
      cur = cur->right;
    }
  }  
  return cur;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

node_t *find_min_successor(rbtree *t, node_t *y) {
  while (y->left != t->nil) { 
    // y의 왼쪽 자식이 nil이 아닐 때까지
    y = y->left;                                      
  }
  return y;
}

void rb_delete_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    // if (x == t->nil)
    //   break;
    if (x == x->parent->left) {
      node_t* w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else { 
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else { 
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase

  node_t *y = z;
  node_t *x;
  color_t y_original_color = y->color;
  
  if (z->left == t->nil) {
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  else if (z->right == t->nil) {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  else {
    y = find_min_successor(t, z->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z)
      x->parent = y;
    else {
      rb_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK)
    rb_delete_fixup(t,x);
  
  free(z);
  // z가 삭제되고 그 자리를 y가 대체하므로 z를 free

  return 0;
}

void inorder_array(node_t *root, node_t *nil, key_t *arr, int *index) {
  if (root == nil) {
    return;
  }

  inorder_array(root->left, nil, arr, index);

  arr[*index] = root->key;
  (*index)++;

  inorder_array(root->right, nil, arr, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

  int *index = calloc(1, sizeof(int));
  inorder_array(t->root, t->nil, arr, index);
  free(index);
  return 0;
}

void printTree(rbtree *t, node_t *cur, int level, int isLeft) {
    if (cur == t->nil) {
        return;
    }

    // 오른쪽 자식 노드 출력
    printTree(t, cur->right, level + 1, 0);

    // 현재 노드 출력
    for (int i = 0; i < level - 1; i++) {
        printf("    ");
    }
    if (level > 0) {
        printf(isLeft ? " \\_ " : " /⎺ ");  // 왼쪽 자식일 경우 "\\" 출력, 오른쪽 자식일 경우 "/" 출력
    }
    if (cur->color == RBTREE_RED)
    {
      printf("\x1b[31m%d\x1b[0m\n", cur->key);
    }
    else{
      printf("%d\n", cur->key);
    }

    // 왼쪽 자식 노드 출력
    printTree(t, cur->left, level + 1, 1);
}