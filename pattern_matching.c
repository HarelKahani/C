#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "pattern_matching.h"

void queue_append(slist_t* queue, pm_state_t* state);
void destroy_state(pm_state_t* state);

pm_state_t *state_init(int id, int depth)
{

    pm_state_t *new_state = (pm_state_t *)malloc(sizeof(pm_state_t));
    if (new_state == NULL)
    {
        printf("new state allocation failed!\n");
        return NULL;
    }
    new_state->id = id;
    new_state->depth = depth;
    new_state->fail = NULL;
    new_state->output = (slist_t *)malloc(sizeof(slist_t));
    if (new_state->output == NULL)
    {
        printf("output alloc fail\n");
        return NULL;
    }
    slist_init(new_state->output);
    new_state->_transitions = (slist_t *)malloc(sizeof(slist_t));
    if (new_state->_transitions == NULL)
    {
        printf("transiton alloc fail\n");
        return NULL;
    }
    slist_init(new_state->_transitions);
    if (new_state->id != 0)
        printf("Allocating state %d\n", new_state->id);
    return new_state;
}

int pm_init(pm_t *pm_machine)
{
    pm_machine->newstate = 0;
    pm_machine->zerostate = state_init(0, 0);
    if (pm_machine->zerostate == NULL)
    {
        printf("state allocation failed!\n");
        return -1;
    }
    pm_machine->zerostate->fail = pm_machine->zerostate;
    return 0;
}

int pm_addstring(pm_t *pm_machine, unsigned char *string, size_t n)
{
    pm_state_t *state_ptr = pm_machine->zerostate; //ptr for states in the machines
    pm_state_t *init_state_ptr;
    int i = 0;
    while (i < n)
    {
        init_state_ptr = pm_goto_get(state_ptr, string[i]);
        if (init_state_ptr == NULL)
        {
            pm_machine->newstate++;                                                  //count++ for states in the machine
            init_state_ptr = state_init(pm_machine->newstate, state_ptr->depth + 1); //creating new state withe "father's" depth+1
            if (init_state_ptr == NULL)
            {
                return -1;
            }
            int check = pm_goto_set(state_ptr, string[i], init_state_ptr);
            if (check == -1)
            {
                return -1;
            }
            state_ptr = init_state_ptr;
        }
        else
        {
            state_ptr = init_state_ptr; //point to next state if exist
        }
        i++;
    }
  
    if (slist_prepend(state_ptr->output, (void *)string) == -1)
    {
        printf("adding string to slist failed!\n");
        return -1;
    }
    return 0;
}

int pm_makeFSM(pm_t *pm_machine)
{
    int i;
    pm_state_t *father_state;
    slist_node_t *ptr_transition;
    pm_labeled_edge_t *ptr_transition_state;
    pm_state_t *state_for_failure;
    pm_state_t *ptr_fail;
    if (slist_size(pm_machine->zerostate->_transitions) <= 0) //if no transition
    {
        return 0;
    }
    slist_t *queue = (slist_t *)malloc(sizeof(slist_t));
    if (queue == NULL)
    {
        printf("fail queue allocation\n");
        return -1;
    }
    slist_node_t *ptr_node = slist_head(pm_machine->zerostate->_transitions);
    pm_state_t *ptr_state;
    slist_init(queue);
    while (ptr_node != NULL)
    {
        ptr_state = (pm_state_t *)((pm_labeled_edge_t *)ptr_node->data)->state;
        slist_append(queue, ptr_state); //adding depth 1 states into queue
        ptr_state->fail = pm_machine->zerostate;
        ptr_node = slist_next(ptr_node);
    }

    while (queue->size > 0)
    {
        /*r*/ father_state = slist_pop_first(queue);
        ptr_transition = slist_head(father_state->_transitions); //pointer to state to check his transition list

        while (ptr_transition != NULL)
        {
            ptr_transition_state = (pm_labeled_edge_t *)ptr_transition->data; //ptr to run for all transition for father state
            /*b*/ unsigned char symbol = (unsigned char)ptr_transition_state->label;
            /*s*/ state_for_failure = ptr_transition_state->state; // ptr to the state for a transiton
            slist_append(queue, (pm_state_t *)state_for_failure);
            /*state*/ ptr_fail = father_state->fail; // ptr to father failure
            while ((pm_goto_get(ptr_fail, symbol) == NULL) && (ptr_fail != pm_machine->zerostate))
            {
                ptr_fail = ptr_fail->fail;
            }
            state_for_failure->fail = pm_goto_get(ptr_fail, symbol); //go up in failure's till reach zero state
            printf("Setting f(%d)=%d\n", state_for_failure->id, state_for_failure->fail->id);
            if (slist_head(state_for_failure->fail->output) != NULL)
            {   
                slist_append_list(state_for_failure->output, state_for_failure->fail->output);
            }
            ptr_transition = slist_next(ptr_transition); //move on to next transition
        }
    }
    free(queue);
    return 0;
}
int pm_goto_set(pm_state_t *from_state, unsigned char symbol, pm_state_t *to_state)
{

    pm_labeled_edge_t *transition = (pm_labeled_edge_t *)malloc(sizeof(pm_labeled_edge_t));
    if (transition == NULL)
    {
        printf("edge allocation fail\n");
        return -1;
    }
    transition->label = symbol;
    transition->state = to_state;
    int check = slist_append(from_state->_transitions, transition); // adding to transition list new edge
    if (check == -1)
    {
        printf("transition append failed!\n");
        return -1;
    }
    if (from_state->_transitions->size >= PM_CHARACTERS)
    {
        printf("overflow! only 256 charecters permited\n");
        return -1;
    }
    to_state->depth = from_state->depth + 1; //for sure the to_state is not at the same depth of from_state
    printf("%d->%c->%d\n", from_state->id, symbol, to_state->id);

    return 0;
}
pm_state_t *pm_goto_get(pm_state_t *state, unsigned char symbol)
{
    if (state == NULL)
    {
        return NULL;
    }
    if (slist_size(state->_transitions) == 0)
    {
        return NULL; //transition list is empty
    }
    slist_node_t *temp = slist_head(state->_transitions);
    while (temp != NULL)
    {
        //data is edge, therfor casting needed to reach label
        if (((pm_labeled_edge_t *)slist_data(temp))->label == symbol)
        {
            return ((pm_labeled_edge_t *)slist_data(temp))->state; //returning the destnation state
        }
        temp = slist_next(temp);
    }

    return NULL;
}
slist_t *pm_fsm_search(pm_state_t *state, unsigned char *txt, size_t txt_length)
{
    slist_t *res_list = (slist_t *)malloc(sizeof(slist_t));
    pm_state_t *ptr_state;
    int i;
    int flag = 0;
    slist_init(res_list);
    for (i = 0; i < txt_length; i++)
    {
        ptr_state = pm_goto_get(state, txt[i]);
        flag = 0;
        while (ptr_state == NULL && flag == 0)
        {
            state = state->fail;
            if (state->id == 0)
            { // at zero state
                flag = 1;
            }
            ptr_state = pm_goto_get(state, txt[i]);
        }
        ptr_state = pm_goto_get(state, txt[i]);
        if (ptr_state == NULL && flag == 1)
        {
            continue;
        }
        state = ptr_state;
        if (slist_size(state->output) > 0)
        {
            slist_node_t *ptr_list = slist_head(ptr_state->output);
            //ptr for output-list , adding matches to res list.
            for (int j = 0; j < slist_size(ptr_state->output); j++)
            {

                if (ptr_list->data != NULL)
                {
                    pm_match_t *match = (pm_match_t *)malloc(sizeof(pm_match_t));
                    unsigned char *pattern = (unsigned char *)ptr_list->data;
                    int start = i - strlen(pattern) + 1;
                    int end = i;
                    int last_state = state->id;
                    match->pattern = pattern;
                    match->fstate = state;
                    match->start_pos = start;
                    match->end_pos = end;
                    slist_append(res_list, match);
                    printf("Pattern: %s, start at: %d ends at: %d, last state= %d\n", pattern, start, end, last_state);
                }
                ptr_list = slist_next(ptr_list);
            }
        }
    }
    return res_list;
}

void pm_destroy(pm_t * pm_machine){
    if(pm_machine==NULL || pm_machine->zerostate==NULL){
        printf("there is no automaton to destroy\n");
        return;
    }
    slist_t* queue = (slist_t*)malloc(sizeof(slist_t));
    if(queue==NULL){
       printf(" queue allocation failed\n");
        return; 
    }
    slist_init(queue);
    if(pm_machine->newstate>0){
        queue_append(queue,pm_machine->zerostate);
        while(slist_size(queue)>0){
            pm_state_t* state_delete =slist_pop_first(queue);
            if (state_delete!=NULL){
                queue_append(queue,state_delete);
                destroy_state(state_delete);
            }
        }
    }
    destroy_state(pm_machine->zerostate);
    slist_destroy(queue, SLIST_LEAVE_DATA);
    free(queue);
}

void queue_append(slist_t* queue, pm_state_t* state){
    if(state->_transitions!=NULL){
        slist_node_t* ptr_state = slist_head(state->_transitions);
        while(ptr_state!=NULL){
           pm_state_t* child_state = (pm_state_t*)((pm_labeled_edge_t*)slist_data(ptr_state))->state;
            if(slist_append(queue, child_state)==-1){
                printf("problem with q append\n");
                return;
            }//appending success
            ptr_state=slist_next(ptr_state);
        }
    }
}

void destroy_state(pm_state_t* state){
    //for each slist_init in state_init --> will be a destroy
    slist_destroy(state->output,SLIST_LEAVE_DATA);
    free(state->output);
    slist_destroy(state->_transitions, SLIST_FREE_DATA);
    free(state->_transitions);
    state->fail=NULL;
    state->output=NULL;
    state->_transitions=NULL;
    free(state);
}

