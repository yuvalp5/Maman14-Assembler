/*
TODO: implement types
    string - implemented string.h
    bool - implelented stdbool
    table - done
    stack - done
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "types.h"




  // stack basic functions - maybe we should move them to utils?
  Stack *create_stack(int capacity){
    if (capacity <= 0){
      return NULL;
      }
    else {
        Stack *stack = (Stack *)malloc(sizeof(Stack));
        if (stack == NULL){
          return NULL;
          }
          stack->collection =malloc(sizeof(int) * capacity);
          if (stack->collection == NULL){
            free(stack);
            return NULL;
            }
            stack->capacity = capacity;
            stack->size = 0;
            return stack;
      }
  }

  void destroy_stack(Stack *stack){
    free(stack->collection);
    free(stack);
  }

  bool is_full(Stack *stack){
    return stack->size == stack->capacity;
  }

  bool is_empty(Stack *stack){
    return stack->size == 0;
  }

  bool pop(Stack *stack, int *item){ //int item will not be int of course
    if (is_empty(stack)){
      return false;
      }
      else{
        *item = stack->collection[stack->size - 1];
        stack->size--;
        return true;
        }
  }

  bool push(Stack *stack, int item){
    if (is_full(stack)){
      return false;
      }
      stack->collection[stack->size] = item;
      stack->size++;
      return true;

  }

  bool peek(Stack *stack, int *item){
    if (is_empty(stack)){
      return false;
      }
      *item = stack->collection[stack->size - 1];
  }

