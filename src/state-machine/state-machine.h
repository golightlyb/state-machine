/*
 
 sstate-machine/state-machine.h
 
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

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stddef.h> // size_t
#include <limits.h> // UINT_MAX

#define STATE_MACHINE_INVALID UINT_MAX

typedef struct state_machine state_machine;
typedef struct state_machine_memory_manager state_machine_memory_manager;

// Optional: concrete definition of a structure used to indicate how memory
// should be allocated or deallocated for a state machine. Fill in the function
// pointers and the user_arg for your memory manager state, if any.
// Otherwise use state_machine_new to use default malloc/free behaviour.
struct state_machine_memory_manager
{
    void *(*allocator)(size_t size, void *user_arg);
    void (*deallocator)(void *ptr, size_t size, void *user_arg);
    void *user_arg;
};

// Create a state machine that will hold a given number of unique states
// and a certain size alphabet of actions. For best results the number of
// states should be small but the same alphabet of actions should be shared
// with other related state machines.
state_machine *state_machine_new(unsigned int states, unsigned int actions);

// As state_machine_new, but accepts a structure indicating how memory should
// be allocated and deallocated.
state_machine *state_machine_new_using
    (unsigned int states, unsigned int actions, state_machine_memory_manager *mgr);

// Frees the memory associated with a state machine
void state_machine_free(state_machine *m);

// Clears all states and transitions in a state machine
int state_machine_clear(state_machine *m);

// Add a state to the machine identified by an arbitrary ID (for example
// constructed by ORing together flags representing properties of a state).
// The state ID must be non-zero.
int state_machine_add_state(state_machine *m, unsigned int state);

// Add a transition from one state in a machine to another state when the given
// action is applied. The machine is determinate such that an action can only
// transition from one state to exactly one destination state. Any existing
// transitions (of the same action from the same state) will be replaced by
// repeated applications of this function.
int state_machine_add_transition
    (state_machine *m, unsigned int action, unsigned int from, unsigned int to);

// Add a transition from all states in the machine, but only if the ID of a
// from state is in the given mask. Any existing transitions will be replaced.
// By "in the given mask" the meaning is where ((state & mask) == mask)
int state_machine_add_transition_from_all_states
    (state_machine *m, unsigned int action, unsigned int to, unsigned int mask);

// Add a transition from all states in the machine, but only if the ID of a
// from state is in the given mask. Any existing transitions will be replaced.
// The replace mask will be subtracted and the with mask will be added to
// give the transition target for each state matching the mask.
int state_machine_add_transition_from_all_states_replacing
    (state_machine *m, unsigned int action,
     unsigned int replace, unsigned int with, unsigned int mask);

// Return the resulting state when an action is taken from a specific state of
// a specific machine. If there is no transition, 0 is returned.
unsigned int state_machine_take_action
    (state_machine *m, unsigned int state, unsigned int action);

// Given a state ID for a machine, this returns a number >= 0 and
// < than the total number of states in the machine. This number identifies
// exactly that state, or returns STATE_MACHINE_INVALID for an invalid state.
// The idea for this is if you want some sort of lookup table from state IDs to
// an image or something similar.
unsigned int state_machine_state_index(state_machine *m, unsigned int state);

// for a states and actions array of pointers to null terminated strings
// the number of elements in both arrays being exactly the number of states
// and actions specified in state_machine_new
void state_machine_print
    (state_machine *m, const char **states, const char **actions);

#endif
