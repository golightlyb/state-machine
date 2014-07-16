/*
 
 state-machine/state-machine.c
 
 ------------------------------------------------------------------------------
 
 Copyright (c) 2014 Ben Golightly <golightly.ben@googlemail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ------------------------------------------------------------------------------
 
*/

#include "base.h" // eXceptions
#include "state-machine/state-machine.h"
#include <stddef.h> // NULL
#include <limits.h> // UINT_MAX
#include <string.h> // memcpy
#include <stdio.h> // printf
#include <assert.h>

#define P(x) state_machine_private_##x


struct state_machine
{
    state_machine_memory_manager mgr;
    
    unsigned int states;
    unsigned int actions;
    
    // map state_index -> state_id
    unsigned int *state_id;
    
    // for each state_index, map actions -> state_index
    unsigned int *transitions;
};


void *bse_default_malloc(size_t size, void *arg);
void bse_default_free(void *ptr, size_t size, void *arg);


static void *P(new)(state_machine *m, size_t size)
{
    if (!size) { return NULL; }
    return m->mgr.allocator(size, m->mgr.user_arg);
}


static void P(free)(state_machine *m, void *memory, size_t size)
{
    if (!memory) { return; }
    m->mgr.deallocator(memory, size, m->mgr.user_arg);
}


state_machine *state_machine_new_using
    (unsigned int states, unsigned int actions, state_machine_memory_manager *mgr)
{
    if (!mgr) { X(bad_arg); }
    
    state_machine *m = mgr->allocator(sizeof(state_machine), mgr->user_arg);
    if (!m) { X(allocate_state_machine); }
    
    memcpy(&m->mgr, mgr, sizeof(state_machine_memory_manager));
    
    m->states  = states;
    m->actions = actions;
    
    m->state_id    = NULL;
    m->transitions = NULL;
    
    m->state_id = P(new)(m, sizeof(unsigned int) * states);
    if (!m->state_id) { X(allocate_state_ids); }
    
    m->transitions = P(new)(m, sizeof(unsigned int) * states * actions);
    if (!m->transitions) { X(allocate_transitions); }
    
    state_machine_clear(m);
    
    return m;
    
    err_allocate_transitions:
    err_allocate_state_ids:
        state_machine_free(m);
    err_allocate_state_machine:
    err_bad_arg:
        return NULL;
}


state_machine *state_machine_new(unsigned int states, unsigned int actions)
{
    state_machine_memory_manager mgr;
    mgr.allocator   = bse_default_malloc;
    mgr.deallocator = bse_default_free;
    mgr.user_arg    = NULL;
    
    return state_machine_new_using(states, actions, &mgr);
}


void state_machine_free(state_machine *m)
{
    if (!m) { X(bad_arg); }
    
    P(free)(m, m->transitions, sizeof(unsigned int) * m->states * m->actions);
    P(free)(m, m->state_id, sizeof(unsigned int) * m->states);
    P(free)(m, m, sizeof(state_machine));
    
    err_bad_arg:
        return;
}


int state_machine_clear(state_machine *m)
{
    if (!m) { X(bad_arg); }
    
    for (unsigned int i = 0; i < m->states; i++)
        { m->state_id[i] = 0; }
        
    for (unsigned int i = 0; i < m->states * m->actions; i++)
        { m->transitions[i] = STATE_MACHINE_INVALID; }
    
    return 1;
    
    err_bad_arg:
        return 0;
}


int state_machine_add_state(state_machine *m, unsigned int state)
{
    if (!m)                { X(bad_arg); }
    if (state == 0)        { X2(bad_arg, "state must be non-zero"); }
    
    unsigned int top_state = STATE_MACHINE_INVALID;
    for (unsigned int i = 0; i < m->states; i++)
        { if (m->state_id[i] == 0) { top_state = i; break; } }
    
    if (top_state >= m->states) { X(state_machine_full); }
    
    // the added state_id is mapped to the index top_state
    m->state_id[top_state] = state;
    
    return 1;
    
    err_state_machine_full:
    err_bad_arg:
        return 0;
}


unsigned int P(state_index)(state_machine *m, unsigned int state_id)
{
    assert(m);
    
    for (unsigned int i = 0; i < m->states; i++)
    {
        if (m->state_id[i] == state_id) { return i; }
    }
    
    return STATE_MACHINE_INVALID;
}


int state_machine_add_transition
    (state_machine *m, unsigned int action, unsigned int from, unsigned int to)
{
    if (!m) { X(bad_arg); }
    unsigned int a = P(state_index)(m, from);
    unsigned int b = P(state_index)(m, to);
    
    if (a >= m->states)       { X4(bad_arg, "invalid from state", 0, from); }
    if (b >= m->states)       { X4(bad_arg, "invalid to state",   0, to); }
    if (action >= m->actions) { X4(bad_arg, "invalid action",     0, action); }
    
    m->transitions[(a * m->actions) + action] = b;
    
    return 1;
    
    err_bad_arg:
        return 0;
}


int state_machine_add_transition_from_all_states
    (state_machine *m, unsigned int action, unsigned int to, unsigned int mask)
{
    if (!m)                                { X(bad_arg); }
    if (P(state_index)(m, to) > m->states) { X2(bad_arg, "invalid to state"); }
    if (action >= m->actions)              { X2(bad_arg, "invalid action"); }
    
    for (unsigned int i = 0; i < m->states; i++)
    {
        unsigned int state = m->state_id[i];
        if ((mask & state) != mask) { continue; }
        
        assert(state_machine_add_transition(m, action, state, to));
    }
    
    return 1;
    
    err_bad_arg:
        return 0;
}


int state_machine_add_transition_from_all_states_replacing
    (state_machine *m, unsigned int action,
     unsigned int replace, unsigned int with, unsigned int mask)
{
    if (!m)                                { X(bad_arg); }
    if (action >= m->actions)              { X2(bad_arg, "invalid action"); }
    
    for (unsigned int i = 0; i < m->states; i++)
    {
        unsigned int state = m->state_id[i];
        if ((mask & state) != mask) { continue; }
        
        unsigned int to = (state & ~replace) | with;
        
        if (!state_machine_add_transition(m, action, state, to))
            { X(state_machine_add_transition); }
    }
    
    return 1;
    
    err_state_machine_add_transition:
        printf("Note that the state of the state_machine is now indeterminate\n");
    err_bad_arg:
        return 0;
}


unsigned int state_machine_take_action
    (state_machine *m, unsigned int state, unsigned int action)
{
    if (action >= m->actions)   { X2(bad_arg, "invalid action"); }
    
    printf("take_action\n");
    
    unsigned int from = P(state_index)(m, state);
    if (from >= m->states) { X4(bad_arg, "invalid state", 0, state); }
    
    unsigned int to = m->transitions[(from * m->actions) + action];
    if (to >= m->states) { return 0; }
    
    return m->state_id[to];
    
    err_bad_arg:
        return 0;
}


unsigned int state_machine_state_index(state_machine *m, unsigned int state)
{
    if (!m) { X(bad_arg); }
    
    return P(state_index)(m, state);
    
    err_bad_arg:
        return STATE_MACHINE_INVALID;
}


void state_machine_print
    (state_machine *m, const char **states, const char **actions)
{
    if (!m)       { X(bad_arg); }
    if (!states)  { X(bad_arg); }
    if (!actions) { X(bad_arg); }
    
    printf("state_machine_print: %u states %u actions\n", m->states, m->actions);
    
    for (unsigned int i = 0; i < m->states; i++)
    {
        printf("    state %u %s:\n", m->state_id[i], states[i]);
        
        for (unsigned int j = 0; j < m->actions; j++)
        {
            unsigned int to = m->transitions[(i * m->actions) + j];
            if (to >= m->states) { continue; }
            
            printf("        action %u %s -> state %u %s\n",
                    j, actions[j], m->state_id[to], states[to]);
        }
    }
    
    err_bad_arg:
        return;
}
