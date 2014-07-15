#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stddef.h> // size_t
#include <limits.h> // UINT_MAX

#define STATE_MACHINE_INVALID UINT_MAX

typedef struct state_machine state_machine;
typedef struct state_machine_memory_manager state_machine_memory_manager;

// Concrete definition of a structure used to indicate how memory should be
// allocated or deallocated for a state machine. Fill in the function pointers
// and the user_arg for your memory manager state, if any.
// Optional: use state_machine_new to use default malloc/free behaviour.
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
int state_machine_add_transition_from_all_states
    (state_machine *m, unsigned int action, unsigned int to, unsigned int mask);

// Return the resulting state when an action is taken from a specific state of
// a specific machine. If there is no transition, 0 is returned.
unsigned int state_machine_take_action
    (state_machine *m, unsigned int state, unsigned int action);

// Given a state ID for a machine, this returns a number >= 0 and
// < than the total number of states in the machine. This number identifies
// exactly that state, or returns (-1 for an invalid state.
unsigned int state_machine_state_index(state_machine *m, unsigned int state);

void state_machine_print
    (state_machine *m, const char **states, const char **actions);

#endif
