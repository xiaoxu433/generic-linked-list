#include <stdio.h>
#include <stdlib.h>

#include "gll_defs.h"

LinkedListType *gll_init() {
  LinkedListType *list = (LinkedListType *) malloc(sizeof(LinkedListType));
  list->size = 0;
  list->first = NULL;
  list->last = NULL;
}

void *gll_get(int pos, LinkedListType *list) {
  return gll_findNode(pos, list)->data;
}

NodeType *gll_findNode(int pos, LinkedListType *list) {
  if(pos > list->size || pos < 0)
    return NULL;  

  NodeType *currNode;
  int currPos;
  int reverse;
  
  if(pos > ((list->size-1) / 2)) {
    reverse  = 1;
    currPos  = list->size - 1;
    currNode = list->last;
  }
  else {
    reverse  = 0;
    currPos  = 0;
    currNode = list->first;
  }  

  while(currNode != NULL) {
    if(currPos == pos)
      break;

    currNode = (reverse ? (currNode->prev) : (currNode->next));
    currPos  = (reverse ? (currPos-1) : (currPos+1));
  }

  return currNode;
}

int gll_add(void *data, int pos, LinkedListType *list) {

  if(pos > list->size || pos < 0)
    return C_NOK;

  NodeType *newNode;
  NodeType *currNode;

  //Create the new node
  newNode = (NodeType *) malloc(sizeof(NodeType));
  newNode->data = data;
  newNode->prev = NULL;
  newNode->next = NULL;

  //list is empty
  if(list->size == 0) {
    list->first = newNode;
    list->last = newNode;

    list->size++;
    return C_OK;
  }
  
  currNode = gll_findNode(pos, list);

  //adding at the front or in the middle
  if(currNode != NULL) {
    newNode->prev = currNode->prev;
    newNode->next = currNode;  
    
    if(currNode->prev == NULL) {
      list->first = newNode;
    }
    else {
      currNode->prev->next = newNode;
    }
    currNode->prev = newNode;
  } 
  //adding at the end
  else {
    list->last->next = newNode;
    newNode->prev = list->last;
    list->last = newNode;    
  }
  
  list->size++;
  return C_OK;
}

int gll_remove(int pos, LinkedListType *list) {
  
  NodeType *currNode = gll_findNode(pos, list);

  //element not found
  if(currNode == NULL)
    return C_NOK;

  if(currNode->prev == NULL) 
    list->first = currNode->next;
  else 
    currNode->prev->next = currNode->next;
  
  if(currNode->next == NULL)
    list->last = currNode->prev; 
  else
    currNode->next->prev = currNode->prev;

  list->size--;
  gll_deallocNode(currNode);
  return C_OK;
}

void gll_each(void (*f)(void *), LinkedListType *list) {
  NodeType *currNode = list->first;

  while(currNode != NULL) {
    (*f)(currNode->data);
    currNode = currNode->next;
  }
}

void gll_deallocNode(NodeType *node) {
  free(node);
}

void gll_deallocList(LinkedListType *list) {
  NodeType *currNode = list->first;
  NodeType *nextNode;
  
  while(currNode != NULL) {
    nextNode = currNode->next;
    gll_deallocNode(currNode);
    currNode = nextNode;  
  }

  free(list);
} 
