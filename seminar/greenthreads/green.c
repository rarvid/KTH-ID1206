#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <assert.h>
#include "green.h"

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096

static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};

static green_t *running = &main_green;

static void init() __attribute__((constructor));

void init() {
  getcontext(&main_cntx);
}

green_t *ready = NULL;    // initialize ready queue

void queue_debug() {
  green_t *tmp = ready;
  if(ready == NULL) {
    printf("READY QUEUE EMPTY\n");
    return ;
  }
  do {
    printf("\tthread\t%p\t\n", tmp);
    printf("\tcontext\t%p\t\n",tmp->context);
    printf("\targ\t%p\t\n", tmp->arg);
    printf("\tnext\t%p\t\n", tmp->next);
    printf("\tjoin\t%p\t\n", tmp->join);
    printf("\tretval\t%d\t\n", tmp->retval);
    printf("\tzombie\t%d\t\n\n", tmp->zombie);
    tmp = tmp->next;
  } while(tmp != NULL);
}

void add_last(green_t *thread) {    // add thread to end of list
  if(ready == NULL) {               // initialize new ready ready of its empty
    ready = thread;
    ready->next = NULL;
  } else {
    green_t *tmp = ready;           // copy pointer to ready queue
    while(tmp->next != NULL) {
      tmp = tmp->next;              // go through queue until end
    }
    tmp->next = thread;             // add new thread to end of list
    tmp->next->next = NULL;
  }
}

void add_first(green_t *thread) {   // add thread to start of list
  if(ready == NULL) {               // initialize new ready queue if its empty
    ready = thread;
    ready->next = NULL;
  } else {
    thread->next = ready;           // add to begining
    ready = thread;                 // update start pointer
  }
}

green_t *remove_last() {            // takes last thread from ready queue
  if(ready == NULL) {
    printf("Queue empty, cannot remove thread\n");
    exit (1);
  } else if(ready->next == NULL) {  // if one element in queue return this element and NULL queue
    green_t *tmp = ready;
    ready = NULL;
    return tmp;
  } else {
    green_t *tmp = ready;
    while(tmp->next->next != NULL) {  // iter through queue to find second to last element
      tmp = tmp->next;
    }
    green_t *copy = tmp->next;
    tmp->next = NULL;                 // remove last element
    return copy;                      // return last element
  }
}

green_t *remove_first() {   // takes first thread from ready queue
  if(ready == NULL) {
    printf("Queue empty, cannot remove thread\n");
    exit (1);
  } else {
    green_t *cp = ready;
    ready = ready->next;    // remove first element
    cp->next = NULL;        // NULL dangling pointer
    return cp;              // return first element
  }
}

void green_thread() {
  green_t *this = running;
  void *result = (*this->fun)(this->arg);
  // place waiting (joining) thread in ready queue
  if(this->join != NULL) {
    add_first(this->join);
  }
  // save result of execution
  this->retval = result;
  // we're a zombie
  this->zombie = TRUE;
  // find the next thread to run
  green_t *next = remove_first(ready);
  running = next;
  setcontext(next->context);
}

int green_create(green_t *new, void *(*fun)(void*), void *arg) {
  ucontext_t *cntx = (ucontext_t *)malloc(sizeof(ucontext_t));
  getcontext(cntx);

  void *stack = malloc(STACK_SIZE);

  cntx->uc_stack.ss_sp = stack;
  cntx->uc_stack.ss_size = STACK_SIZE;
  makecontext(cntx, green_thread, 0);

  new->context = cntx;
  new->fun = fun;
  new->arg = arg;
  new->next = NULL;
  new->join = NULL;
  new->retval = NULL;
  new->zombie = FALSE;

  // add new to the ready queue
  add_first(new);
  return 0;
}

int green_yield() {
  green_t *susp = running;
  // add susp to end of ready queue
  add_last(susp);
  // select next thread for execution
  green_t *next = remove_first(ready);
  running = next;
  swapcontext(susp->context, next->context);
  return 0;
}

int green_join(green_t *thread, void **res) {

   if(!thread->zombie) {
    green_t *susp = running;
    // add as joining thread
    thread->join = susp;
    // select the next thread for execution
    green_t *next = remove_first(ready);
    running = next;
    swapcontext(susp->context, next->context);
  }
  // collect result
  if(res != NULL){
    res = thread->retval;
  }
  // free context
  free(thread->context);
  return 0;
}

void green_cond_init(green_cond_t *cond) {
  // TODO IZDOMAT KAPEC COND IR FUCKING NULL<, AAAAAAAA vajadzetu but inicialzietam ?>?>>?>?
  cond->count = 0;
  cond->wait = NULL;
}

void green_cond_wait(green_cond_t *cond) {
  green_t *susp = running;

  // insert current running thread into waiting list :DDD
  if(cond == NULL) {
    green_cond_init(cond);
  }
  if(cond->wait == NULL) {
    cond->wait = susp;
    cond->wait->next = NULL;
    cond->count++;
  } else {
    susp->next = cond->wait;
    cond->wait = susp;
    cond->count++;
  }

  // take new thread
  green_t *next = remove_first();
  running = next;
  swapcontext(susp->context, next->context);

}

void green_cond_signal(green_cond_t *cond) {
  // schedule last thread from waiting list
  green_t *susp = running;

  add_first(susp);
  if(cond->wait == NULL) {
    printf("Wait list empty, no one to signal to\n");
    exit (1);
  } else if(cond->wait->next == NULL) {  // if one element in queue return this element and NULL queue
    green_t *tmp = cond->wait;
    cond->wait = NULL;
    cond->count--;
    running = tmp;
    swapcontext(susp->context, tmp->context);
  } else {
    green_t *tmp = cond->wait;
    while(tmp->next->next != NULL) {  // iter through queue to find second to last element
      tmp = tmp->next;
    }
    green_t *copy = tmp->next;
    cond->count--;
    tmp->next = NULL;                 // remove last element
    running = copy;
    swapcontext(susp->context, copy->context);
  }
}
