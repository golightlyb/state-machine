// This example models a simple clock and prints the graph using the DOT
// description language. Use a program like xdot to see it.

// Public Domain BSAG 2014

#include "state-machine/state-machine.h"
#include <stdio.h>
#include <assert.h>
#include <limits.h>

# define ACTION_TICK  0
# define ACTION_TOCK  1
# define NUM_ACTIONS  2

const char *actions[] =
{
    "tick",
    "tock"
};


int main(void)
{
    state_machine *m = state_machine_new(2, NUM_ACTIONS);
    assert(m);
    
    assert(state_machine_add_state(m, 1));
    assert(state_machine_add_state(m, 2));
        
    assert(state_machine_add_transition(m, ACTION_TICK, 1, 2));
    assert(state_machine_add_transition(m, ACTION_TOCK, 2, 1));
    
    // print in the DOT graph description language.
    state_machine_print(stdout, m, "clock", NULL, actions);
    
    return 0;
}
