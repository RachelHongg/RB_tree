#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
    NIL -> color = RBTREE_BLACK;
	// 굳이 NULL과 0으로 채울필요없이, 이미 되어있음.
    // NIL -> left = NULL;
    // NIL -> right = NULL;
    // NIL -> key = 0;
    p -> nil = NIL;
    p -> root = NIL;
    return p;
}

void delete_node(rbtree *t, node_t *n) {
    if (n == t -> nil)
        return ;
    delete_node(t, n ->right);
    delete_node(t, n -> left);
    free(n);
}

void delete_rbtree(rbtree *t) {
  delete_node(t, t -> root);
  free(t -> nil);
  free(t);
}

/*
delete_rbtree에 들어있는 변수가 rbtree* 타입이라 노드 하나씩 못끊어줌.
t -> root로 타고 내려온 다음에,
node_t* 타입을 변수로 갖는 delete_node를 만들어서 왼쪽, 오른쪽 각각 재귀를 돌려주자!
*/

void left_rotate(rbtree *t, node_t *cur_node){
    node_t *right_node = (node_t *)calloc(1, sizeof(node_t));
    right_node = cur_node -> right;
    cur_node -> right = right_node -> left;
    if (right_node == t -> nil) {
        return;
    }
    if (right_node -> left != t -> nil)
        right_node -> parent = cur_node;
    right_node -> parent = cur_node -> parent;
    if (cur_node -> parent == t -> nil)
        t -> root = right_node;
    else if (cur_node == cur_node -> parent -> left)
    cur_node -> parent -> left = right_node;
    else {
        cur_node -> parent -> right = right_node;
        right_node -> left = cur_node;
    }
    right_node -> left = cur_node;
    cur_node -> parent = right_node;
}

void right_rotate(rbtree *t, node_t *cur_node){
    node_t *left_node = (node_t *)calloc(1, sizeof(node_t));
    left_node = cur_node -> left;
    cur_node -> left = left_node -> right;
    if (left_node == t -> nil) {
        return;
    }
    if (left_node -> right != t -> nil)
        left_node -> parent = cur_node;
    left_node -> parent = cur_node -> parent;
    if (cur_node -> parent == t -> nil)
        t -> root = left_node;
    else if (cur_node == cur_node -> parent -> right)
    cur_node -> parent -> right = left_node;
    else {
        cur_node -> parent -> left = left_node;
        left_node -> right = cur_node;
    }
    left_node -> right = cur_node;
    cur_node -> parent = left_node;
}
void rb_insert_fixup(rbtree *t, node_t * cur_node, const key_t key) {
  node_t *uncle_node;
    // cur_node와 부모의 색 모두 RED 일때까지
    while (cur_node -> parent -> color == RBTREE_RED) {
        // 부모가 할아버지의 왼쪽 자식일때
        if (cur_node -> parent == cur_node -> parent -> parent -> left) {
            // 삼촌 노드 지정(할아버지의 오른쪽 자식을)
            uncle_node = cur_node -> parent -> parent -> right;
            // (경우1) 삼촌이 RED 일때
            if (uncle_node -> color  == RBTREE_RED) {
                cur_node -> parent -> color = RBTREE_BLACK;
                uncle_node -> color = RBTREE_BLACK;
                cur_node -> parent -> parent -> color = RBTREE_RED;
                cur_node = cur_node -> parent -> parent;    // 새로운 cur_node 지정
            }
      		// (경우2) 삼촌이 BLACK 이면서
            // cur_node가 부모의 오른쪽 자식일 때(껶여있음)
            else {
                if (cur_node == cur_node -> parent -> right) {
                    // cur_node를 부모로 바꿔서, cur_node 중심으로 왼쪽으로 회전
                    cur_node = cur_node -> parent;
                    left_rotate(t, cur_node);
                }
                // (경우3) 삼촌이 BLACK 이면서
                // cur_node가 부모의 왼쪽 자식일 때(펴져있음)
				cur_node -> parent -> color = RBTREE_BLACK;
            	cur_node -> parent -> parent -> color = RBTREE_RED;
                // 할아버지를 기준으로, 오른쪽으로 회전
                // cur_node 안바꾸고 종료!
            	right_rotate(t, cur_node -> parent -> parent);
        	}
		}
        else {
        // 삼촌 노드 지정(할아버지의 왼쪽 자식을)
            uncle_node = cur_node -> parent -> parent -> left;
            // (경우4) 삼촌이 RED 일 때
            if (uncle_node -> color  == RBTREE_RED) {
                cur_node -> parent -> color = RBTREE_BLACK;
                uncle_node -> color = RBTREE_BLACK;
                cur_node -> parent -> parent -> color = RBTREE_RED;
                cur_node = cur_node -> parent -> parent;    // 새로운 cur_node 지정
                }
            // (경우5) 삼촌이 BLACK 이면서
            // cur_node가 부모의 왼쪽 자식일 때(껶여있음)
            else 
            {
                if (cur_node == cur_node -> parent -> left) {
                // cur_node를 부모로 바꿔서, cur_node 중심으로 오른쪽으로 회전
                cur_node = cur_node -> parent;
                right_rotate(t, cur_node);
                }
                // (경우6) 삼촌이 BLACK 이면서
                // cur_node가 부모의 오른쪽 자식일 때(펴져있음)
				cur_node -> parent -> color = RBTREE_BLACK;
				cur_node -> parent -> parent -> color = RBTREE_RED;
				// 할아버지를 기준으로, 왼쪽으로 회전
				// cur_node 안바꾸고 종료!
				left_rotate(t, cur_node -> parent -> parent);
			}
        }
    }
    t -> root -> color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t * new_node = (node_t *)calloc(1, sizeof(node_t));
    node_t * new_tmp = t -> nil;
    node_t * new_child = t -> root;
    // 삽입되는 위치 찾기
    while (new_child != t -> nil) {
        new_tmp = new_child;
        if (key < new_child -> key) new_child = new_child -> left;
        else new_child = new_child -> right;
    }
    // new_node 이외의 초기값 세팅
    new_node -> left = t -> nil;
    new_node -> right = t -> nil;
    new_node -> color = RBTREE_RED; // 삽입의 규칙
    new_node -> key = key;
    new_node -> parent = new_tmp; // 현재 new_child는 nil값
    // while문에 안들어간, 트리가 초기 상태일때
    if(new_tmp == t -> nil) {
        t -> root = new_node;
        new_node -> color = RBTREE_BLACK;
        return t -> root;
    }
    // new_node의 방향 정해주기
    if (new_node -> key < new_node -> parent -> key) new_node -> parent -> left = new_node;
    else new_node -> parent -> right = new_node;
    rb_insert_fixup(t, new_node, key);
    return t -> root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t * p = t -> root;
  while(p != t -> nil) {
    if(key < p -> key)
        p = p -> left;
    else if(p -> key < key)
        p = p -> right;
    else
        return p;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement
  node_t* p = t -> root;
  if(p == NULL)
    return p;
  while(p -> left != t -> nil) {
    p = p -> left;
  }
  return p;
}

node_t *rbtree_max(const rbtree *t) {
    node_t* p = t -> root;
    if(p == NULL)
        return p;
    while(p -> right != t -> nil) {
        p = p -> right;
        }
    return p;
}

// nil parent없음!
void rb_transplant(rbtree* t, node_t* u, node_t* v) {
    if(u -> parent == t -> nil)
        t -> root = v;
    else if(u == u -> parent -> left)
        u -> parent -> left = v;
    else u -> parent -> left = v;
    v -> parent = u -> parent;
}

node_t* tree_minimum(rbtree* t, node_t* p) {
    p = p -> right;
    while(p != t -> nil)
        p = p -> left;
    return p -> parent;
}

rbtree* rb_delete_fixup(rbtree *t, node_t * x) {
    node_t* w;
    while(x != t -> root && x -> color == RBTREE_BLACK) {
        // 내가 부모의 왼쪽 자식일때
        if(x == x -> parent -> left) {
            w = x -> parent -> right;
            // 경우1: 형제가 RED인 경우
            if(w -> color == RBTREE_RED) {
                w -> color = RBTREE_BLACK;
                x -> parent -> color = RBTREE_RED;
                left_rotate(t, x -> parent);
                w = x -> parent -> right;
            }
            // 경우2: 형제가 BLACK이고, 형제의 자식이 모두 BLACK인 경우
            if(w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK) {
                w -> color = RBTREE_RED;
                x = x -> parent;
            }
            // 경우3: 형제가 BLACK, 형제의 오른쪽 자녀 BLACK, 형제의 왼쪽 자녀 RED
            else if(w -> right -> color == RBTREE_BLACK) {
                w -> left -> color = RBTREE_BLACK;
                w -> color = RBTREE_RED;
                right_rotate(t, w);
                w = x -> parent -> right;
            }
            // 경우4: 형제가 BLACK, 형제의 오른쪽 자식 RED
            w -> color = x -> parent -> color;
            x -> parent -> color = RBTREE_BLACK;
            w -> right -> color = RBTREE_BLACK;
            left_rotate(t, x -> parent);
            x = t -> root;
        }
        // 내가 부모의 오른쪽 자식일때
        else {
            w = x -> parent -> left;
            // 경우5: 형제가 RED
            if(w -> color == RBTREE_RED) {
                w -> color = RBTREE_BLACK;
                x -> parent -> color = RBTREE_RED;
                right_rotate(t, x -> parent);
                w = x -> parent -> left;
            }
            // 경우6: 형제와 형제의 자식 모두 BLACK
            if(w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK) {
                w -> color = RBTREE_RED;
                x = x -> parent;
            }
            // 경우7: 형제와 형제의 왼쪽 자식은 BLACK, 오른쪽 자식은 RED
            else{
                if(w -> left -> color == RBTREE_BLACK) {
                    w -> right -> color = RBTREE_BLACK;
                    w -> color = RBTREE_RED;
                    left_rotate(t, w);
                    w = x -> parent -> left;
            }
            // 경우8: 
            w -> color = x -> parent -> color;
            x -> parent -> color = RBTREE_BLACK;
            w -> left -> color = RBTREE_BLACK;
            right_rotate(t, x -> parent);
            x = t -> root;
            }
        }
    }
    x -> color = RBTREE_BLACK;
    return t;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t* y = p;
  node_t* x = p;
  // 원래의 y값 저장하기.
  color_t y_origin_color = y -> color;

  if(p -> left == t -> nil) {
    x = p -> right;
    rb_transplant(t, p, p -> right);
  }
  else
  { 
    if(p -> right == t -> nil) {
    x = p -> left;
    rb_transplant(t, p, p -> left);
    }
    y = tree_minimum(t, p -> right);
    y_origin_color = y -> color;
    p = y -> right;
    if(y ->parent == p) x -> parent = y;
    else {
        rb_transplant(t, y, y -> right);
        y -> right = p -> right;
        y -> right -> parent = y;
    }
    rb_transplant(t, p, y);
    y -> left = p -> left;
    y -> left -> parent = y;
    y -> color = p -> color;
  }
  if(y_origin_color == RBTREE_BLACK) rb_delete_fixup(t, x);
  free(p);
  return 0;
}
int node_to_array(rbtree* t, key_t *arr, node_t* pointer, int a);

int rbtree_to_array(rbtree *t, key_t *arr, const size_t n) {
	node_to_array(t, (int*)arr, t -> root, 0);
  return 0;
}

int node_to_array(rbtree* t, key_t *arr, node_t* pointer, int a) {
	if(pointer == t -> nil)
		return a;
	a = node_to_array(t, (int*)arr, pointer -> left, a); // 왼쪽
	arr[a++] = pointer -> key;	// 출력
	a = node_to_array(t, (int*)arr, pointer -> right, a); // 오른쪽
	return 0;
} 
