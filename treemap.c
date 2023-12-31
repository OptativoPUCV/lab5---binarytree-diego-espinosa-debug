#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap * mapaArbol = (TreeMap*)malloc(sizeof(TreeMap));
  mapaArbol->current = NULL;
  mapaArbol->root = NULL;
  mapaArbol->lower_than = lower_than;
  return mapaArbol;
}



TreeNode * minimum(TreeNode * x){
  
  while(x->left != NULL){
    x = x->left;
  }

  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

  if(node->left == NULL && node->right == NULL){
    if(node->parent == NULL){
      tree->root = NULL;
    }else if(node->parent->left == node){
      node->parent->left = NULL;
    }else{
      node->parent->right = NULL;
    }
    free(node);
  }else if(node->left == NULL && node->right != NULL){
    if(node->parent->right == node){
      node->right->parent = node->parent;
      node->parent->right = node->right;
      free(node);
    }else{
      node->right->parent = node->parent;
      node->parent->left = node->right;
      free(node);
    }
  }else if (node->left != NULL && node->right == NULL){
    if(node->parent->right == node){
      node->left->parent = node->parent;
      node->parent->right = node->left;
      free(node);
    }else{
      node->left->parent = node->parent;
      node->parent->left = node->left;
      free(node);
    }
  }else if(node->left != NULL && node->right != NULL){
    TreeNode* min = minimum(node->right);

    node->pair->key = min->pair->key;
    node->pair->value = min->pair->value;
    removeNode(tree,min);
  }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  int comprobacion = is_equal(tree,tree->root->pair->key,key);
  if(comprobacion == 1){
    tree->current = tree->root;
    return tree->root->pair;
  }else{
    tree->current = tree->root;
  }
  
  while(comprobacion < 1){
    int resultado = tree->lower_than(tree->current->pair->key,key);
    if(resultado == 1)
    {
      if(tree->current->right == NULL) return NULL;
      tree->current = tree->current->right;
      
    }else if(resultado == 0){
      if(tree->current->left == NULL) return NULL;
      tree->current = tree->current->left;
    }

    comprobacion = is_equal(tree,tree->current->pair->key,key);
  }
  
  return tree->current->pair;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
  Pair* temp = searchTreeMap(tree, key);
  if(temp != NULL) return;
  int rightOrLeft = 1;

  tree->current = tree->root;
  TreeNode * busqueda =  tree->current;

  while(busqueda != NULL){
    int resultado = tree->lower_than(tree->current->pair->key,key);
    if(resultado == 1)
    {
      if(tree->current->right == NULL) {
        busqueda = NULL;
        rightOrLeft = 1;
      }else{
        tree->current = tree->current->right;
      }
      
    }else if(resultado == 0){
      
      if(tree->current->left == NULL){
        busqueda = NULL;
        rightOrLeft = 0;
      }else{
        tree->current = tree->current->left;
      }
    }
  }
  
  TreeNode* new = createTreeNode(key,value);
  new->parent = tree->current;
  if(rightOrLeft == 1){
    tree->current->right = new;
  }else{
    tree->current->left = new;
  }
  tree->current = new;

}



Pair * upperBound(TreeMap * tree, void* key) {
  if(searchTreeMap(tree, key) != NULL){
    return tree->current->pair;
  }else{
    tree->current = tree->root;
    TreeNode * ub_node = tree->current;

    while(tree->current != NULL){
      
      if(tree->lower_than(key,tree->current->pair->key)){
        ub_node = tree->current;
      }
      
      if(tree->lower_than(tree->current->pair->key,key)){
        tree->current = tree->current->right;
      }else{
        tree->current = tree->current->left;
      }
    }

    if(tree->lower_than(ub_node->pair->key,key)) return NULL;

    return ub_node->pair;
  }
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode* min = minimum(tree->root);
  tree->current = min;
  return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if(tree->current->right != NULL){
    tree->current = minimum(tree->current->right);
  }else{
    while(tree->current->parent != NULL && tree->current == tree->current->parent->right){
      tree->current = tree->current->parent;
    }
    
    if(tree->current->parent == NULL) return NULL;

    tree->current = tree->current->parent;
  }
  return tree->current->pair;
}
