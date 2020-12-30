#define _POSIX_SOURCE
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <ucontext.h>
#include <assert.h>
#include "green.h"

#define FALSE 0
#define TRUE 1
#define PERIOD 100

#define STACK_SIZE 4096

static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};

static green_t *running = &main_green;

void add_last(green_t *, green_t **);
void add_first(green_t *, green_t **);
green_t *remove_first(green_t **);
green_t *remove_last(green_t **);

static sigset_t block;

void timer_handler(int);

static void init() __attribute__((constructor));

void init() {
    getcontext(&main_cntx);
    sigemptyset(&block);
    sigaddset(&block, SIGALRM);

    struct sigaction act = {0};
    struct timeval interval;
    struct itimerval period;

    act.sa_handler = timer_handler;
    assert(sigaction(SIGALRM, &act, NULL) == 0);
    interval.tv_sec = 0;
    interval.tv_usec = PERIOD;
    period.it_interval = interval;
    period.it_value = interval;
    assert(setitimer(ITIMER_REAL, &period, NULL) == 0);
}

green_t *ready = NULL;        // initialize ready queue

int id = 0;

void timer_handler(int sig) {
    sigprocmask(SIG_BLOCK, &block, NULL);
    green_t *susp = running;
    susp->next = NULL;
    add_last(susp, &ready);

    green_t *next = remove_first(&ready);
    running = next;
    swapcontext(susp->context, next->context);
    sigprocmask(SIG_UNBLOCK, &block, NULL);
}



void add_last(green_t *thread, green_t **list) {        // add thread to end of list
    if(*list == NULL) {                             // initialize new ready ready of its empty
        *list = thread;
    } else {
        green_t *tmp = *list;
        while(tmp->next != NULL) {
            tmp = tmp->next;                            // go through queue until end
        }
        tmp->next = thread;                         // add new thread to end of list
    }
}

void add_first(green_t *thread, green_t **list) {     // add thread to start of list
    if(*list == NULL) {                             // initialize new list queue if its empty
        *list = thread;
        (*list)->next = NULL;
    } else {
        thread->next = *list;                     // add to begining
        *list = thread;                                 // update start pointer
    }
}

green_t *remove_last(green_t **list) {                        // takes last thread from list queue
    if(*list == NULL) {
        printf("Queue empty, cannot remove thread\n");
        exit (1);
    } else if((*list)->next == NULL) {    // if one element in queue return this element and NULL queue
        green_t *tmp = *list;
        *list = NULL;
        return tmp;
    } else {
        green_t *tmp = *list;
        while(tmp->next->next != NULL) {    // iter through queue to find second to last element
            tmp = tmp->next;
        }
        green_t *copy = tmp->next;
        tmp->next = NULL;                                 // remove last element
        return copy;                                            // return last element
    }
}

green_t *remove_first(green_t **list) {     // takes first thread from list queue
    if(*list == NULL) {
        printf("Queue empty, cannot remove thread\n");
        exit (1);
    } else {
        green_t *cp = *list;
        *list = (*list)->next;        // remove first element
        cp->next = NULL;                // NULL dangling pointer
        return cp;                            // return first element
    }
    
}

void green_thread() {
    // sigprocmask(SIG_BLOCK, &block, NULL);
    green_t *this = running;
    void *result = (*this->fun)(this->arg);
    // place waiting (joining) thread in ready queue
    if(this->join != NULL) {
        this->join->next = NULL;
        add_last(this->join, &ready);
    }
    // save result of execution
    this->retval = result;
    // we're a zombie
    this->zombie = TRUE;
    // find the next thread to run
    green_t *next = remove_first(&ready);
    running = next;
    setcontext(next->context);
    // sigprocmask(SIG_UNBLOCK, &block, NULL);
}

int green_create(green_t *new, void *(*fun)(void*), void *arg) {
    sigprocmask(SIG_BLOCK, &block, NULL);
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
    add_last(new, &ready);
    sigprocmask(SIG_UNBLOCK, &block, NULL);
    return 0;
}

int green_yield() {
    // sigprocmask(SIG_BLOCK, &block, NULL);
    // printf("- YIELD START -\n");
    green_t *susp = running;
    // add susp to end of ready queue
    // printf("- ADDING TO END OF READY QUEUE -\n");
    susp->next = NULL;
    add_last(susp, &ready);
    // select next thread for execution
    // printf("- TAKING FIRST THREAD FROM READY QUEUE -\n");
    green_t *next = remove_first(&ready);
    running = next;
    // printf("- SWAPING CONTEXT -\n");
    swapcontext(susp->context, next->context);
    // printf("- YIELD END -\n");
    // sigprocmask(SIG_UNBLOCK, &block, NULL);
    return 0;
}

int green_join(green_t *thread, void **res) {
     if(!thread->zombie) {    
        green_t *susp = running;
        // add as joining thread
        thread->join = susp;
        // select the next thread for execution
        green_t *next = remove_first(&ready);
        // sigprocmask(SIG_BLOCK, &block, NULL);
        running = next;
        swapcontext(susp->context, next->context);
        // sigprocmask(SIG_UNBLOCK, &block, NULL);
    }
    // collect result
    if(res != NULL){
        *res = thread->retval;
    }
    // free context
    free(thread->context);
    return 0;
}

void green_cond_init(green_cond_t *cond) {
    cond->wait = NULL;
}

int green_cond_wait(green_cond_t *cond, green_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &block, NULL);
    green_t *susp = running;

    // insert current running thread into waiting list :DDD
    susp->next = NULL;
    add_last(susp, &cond->wait);

    if(mutex != NULL) {
        mutex->taken = FALSE;

        add_last(mutex->susp_l, &ready);
        mutex->susp_l = NULL;
    }

    green_t *next = remove_first(&ready);
    running = next;
    swapcontext(susp->context, next->context);

    if(mutex != NULL) {
        if(mutex->taken) {
            green_t *susp = running;
            susp->next = NULL;
            add_last(susp, &mutex->susp_l);

            green_t *next = remove_first(&ready);
            running = next;
            swapcontext(susp->context, next->context);
        } else {
            mutex->taken = TRUE;
        }
    }
    sigprocmask(SIG_UNBLOCK, &block, NULL);
    return 0;
}

void green_cond_signal(green_cond_t *cond) {
    // sigprocmask(SIG_BLOCK, &block, NULL);
    if(cond == NULL || cond->wait == NULL) {
        return;
    }
    green_t *signaled = remove_first(&cond->wait);
    signaled->next = NULL;
    add_last(signaled, &ready);
    // sigprocmask(SIG_UNBLOCK, &block, NULL);
}

int green_mutex_init(green_mutex_t *mutex) {
    mutex->taken = FALSE;
    mutex->susp_l = NULL;
    return 0;
}

int green_mutex_lock(green_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &block, NULL);

    green_t *susp = running;
    if(mutex->taken) {
        susp->next = NULL;
        add_last(susp, &mutex->susp_l);

        green_t *next = remove_first(&ready);
        running = next;

        swapcontext(susp->context, next->context);
    } else {
        mutex->taken = TRUE;
    }
    sigprocmask(SIG_UNBLOCK, &block, NULL);
    return 0;
}

int green_mutex_unlock(green_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &block, NULL);

    if(mutex->susp_l != NULL) {
        add_last(mutex->susp_l, &ready);
    }
    mutex->taken = FALSE;
    mutex->susp_l = NULL;
    
    sigprocmask(SIG_UNBLOCK, &block, NULL);
    return 0;
}
