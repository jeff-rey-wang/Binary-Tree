#include "expr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *makeString(char *s1, char *s2, char *s3) {
  char *pointTrack, *sample, *result;
  if (!(result = pointTrack = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1)))
    return NULL;
  // pointTrack points to start of result
  // starting at s1, sample saves char into pointTrack
  // pointers are incremented to save proceeding
  for (sample = s1; (*pointTrack = *sample) != '\0'; ++pointTrack, ++sample)
    ;
  for (sample = s2; (*pointTrack = *sample) != '\0'; ++pointTrack, ++sample)
    ;
  for (sample = s3; (*pointTrack = *sample) != '\0'; ++pointTrack, ++sample)
    ;
  result[strlen(s1) + strlen(s2) + strlen(s3)] = '\0';
  return result; // return to the original address of result
}
Node *createNode(char *s, double val) {
  if (!s)
    return NULL; // nothing copied of s is NULL
  Node *node;
  if (!(node = malloc(sizeof(Node))))
    return NULL; // Malloc fails, return NULL pointer
  char *nodeName = malloc(strlen(s) + 1);
  memcpy(nodeName, s, strlen(s));
  nodeName[strlen(s)] = '\0';
  node->expr_string = nodeName;
  node->left = NULL;
  node->right = NULL;
  node->num_parents = 0;
  node->value = val;
  return node;
}
Node *binop(Operation op, Node *a, Node *b) {
  // Possibly, create a node, and use binop to set the expr_string
  if (a->num_parents == 1 || b->num_parents == 1)
    return NULL; // NULL nodes already a parent
  Node *node;
  if (op == addop) {
    char *nodeStr = makeString(a->expr_string, "+", b->expr_string);
    node = createNode(nodeStr, 0);
    free(nodeStr);
  }
  if (op == subop) {
    char *nodeStr = makeString(a->expr_string, "-", b->expr_string);
    node = createNode(nodeStr, 0);
    free(nodeStr);
  }
  if (op == mulop) {
    char *str1 = makeString("(", a->expr_string, ")");
    char *str2 = makeString("(", b->expr_string, ")");
    char *nodeStr = makeString(str1, "*", str2);
    node = createNode(nodeStr, 0);
    free(str1);
    free(str2);
    free(nodeStr);
  }
  if (op == divop) {
    char *str1 = makeString("(", a->expr_string, ")");
    char *str2 = makeString("(", b->expr_string, ")");
    char *nodeStr = makeString(str1, "/", str2);
    node = createNode(nodeStr, 0);
    free(str1);
    free(str2);
    free(nodeStr);
  }
  a->num_parents++; // Increment parents
  b->num_parents++;
  node->left = a;
  node->right = b;
  node->operation = op;
  return node;
}
double evalTree(Node *root) {
  if (!(root->left) || !(root->right)) {
    return root->value; // if root isn't a parent of 2 nodes, return its value
  }
  switch (root->operation) { // Node operation switch
  case addop:
    // value of the root is the value of expression created by its operator and
    // its child nodes
    return root->value = evalTree(root->left) + evalTree(root->right);
  case subop:
    return root->value = evalTree(root->left) - evalTree(root->right);
  case mulop:
    return root->value = evalTree(root->left) * evalTree(root->right);
  case divop:
    return root->value = evalTree(root->left) / evalTree(root->right);
  }
}
void freeTree(Node *root) {
  if (root == NULL)
    return;
  else {
    freeTree(root->left); // free the children first
    freeTree(root->right);
    free(root->expr_string);
    free(root);
  }
}

Node *duplicateTree(Node *root) {
  if (!(root))
    return NULL;
  Node *copy;
  copy = createNode(root->expr_string, root->value);
  copy->operation = root->operation;
  copy->num_parents = root->num_parents;
  copy->left = duplicateTree(root->left);
  copy->right = duplicateTree(root->right);
  return copy;
}
void printTree(Node *root) {
  printf("Node\n\texpr_string\t= %s\n\tvalue\t\t= %g\n\tnum_parents\t= %d\n",
         root->expr_string, root->value, root->num_parents);
  if (root->left)
    printTree(root->left);
  if (root->right)
    printTree(root->right);
}
