// This example models an alarm clock and prints the graph using the DOT
// description language. Use a program like xdot to see it.

// Public Domain BSAG 2014

#include "state-machine/state-machine.h"
#include <stdio.h>
#include <assert.h>
#include <limits.h>

# define STATE_OFF 512
# define STATE_ON  1024 // all ON states can be found with this flag set

# define ACTION_ON    0
# define ACTION_OFF   1
# define ACTION_TICK  2
# define ACTION_TOCK  3
# define ACTION_RESET 4
# define ACTION_RING  5
# define NUM_ACTIONS  6

const char *actions[] =
{
    "on",
    "off",
    "tick",
    "tock",
    "reset",
    "ring"
};


int main(void)
{
    state_machine *m = state_machine_new(8, NUM_ACTIONS);
    assert(m);
    
    assert(state_machine_add_state(m, STATE_OFF));
    assert(state_machine_add_state(m, STATE_ON + 1));
    assert(state_machine_add_state(m, STATE_ON + 2));
    assert(state_machine_add_state(m, STATE_ON + 3));
    assert(state_machine_add_state(m, STATE_ON + 4));
    assert(state_machine_add_state(m, STATE_ON + 5));
    assert(state_machine_add_state(m, STATE_ON + 6));
    assert(state_machine_add_state(m, STATE_ON + 7));
    
    assert(state_machine_add_transition(m, ACTION_ON,   STATE_OFF   , STATE_ON + 1));
    assert(state_machine_add_transition(m, ACTION_TICK, STATE_ON + 1, STATE_ON + 2));
    assert(state_machine_add_transition(m, ACTION_TOCK, STATE_ON + 2, STATE_ON + 3));
    assert(state_machine_add_transition(m, ACTION_TICK, STATE_ON + 3, STATE_ON + 4));
    assert(state_machine_add_transition(m, ACTION_TOCK, STATE_ON + 4, STATE_ON + 5));
    assert(state_machine_add_transition(m, ACTION_TICK, STATE_ON + 5, STATE_ON + 6));
    assert(state_machine_add_transition(m, ACTION_TOCK, STATE_ON + 6, STATE_ON + 7));
    assert(state_machine_add_transition(m, ACTION_RING, STATE_ON + 7, STATE_ON + 7));
    
    assert(state_machine_add_transition_from_all_states(m, ACTION_RESET, STATE_ON + 1, STATE_ON));
    assert(state_machine_add_transition_from_all_states(m, ACTION_OFF, STATE_OFF, STATE_ON));
    
    // print in the DOT graph description language.
    state_machine_print(stdout, m, "alarm clock", NULL, actions);
    
    return 0;
}
