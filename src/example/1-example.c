// This example models some sort of simple lamp with a button as an automata.

// Public Domain BSAG 2014

#include "state-machine/state-machine.h"
#include <stdio.h>
#include <assert.h>

# define STATE_ON       1
# define STATE_OFF      2
# define STATE_WORKING  4
# define STATE_BROKEN   8

# define ACTION_TOGGLE 0
# define ACTION_BREAK  1
# define ACTION_FIX    2
# define NUM_ACTIONS 3

const char *actions[] =
{
    "TOGGLE",
    "BREAK",
    "FIX"
};

int input(void)
{
    int c;
    
    do
    {
        c = fgetc(stdin);
    } while (c == '\n');
    
    return c;
}


int main(void)
{
    state_machine *m = state_machine_new(3, NUM_ACTIONS);
    assert(m);
    
    assert(state_machine_add_state(m, STATE_OFF | STATE_WORKING));
    assert(state_machine_add_state(m, STATE_ON  | STATE_WORKING));
    assert(state_machine_add_state(m, STATE_OFF | STATE_BROKEN));
    
    const char *states[] =
    {
        "OFF | WORKING",
        "ON  | WORKING",
        "OFF | BROKEN"
    };
    
    assert(state_machine_add_transition(m, ACTION_TOGGLE,
        STATE_OFF | STATE_WORKING,
        STATE_ON  | STATE_WORKING));
    
    assert(state_machine_add_transition(m, ACTION_TOGGLE,
        STATE_ON  | STATE_WORKING,
        STATE_OFF | STATE_WORKING));
    
    assert(state_machine_add_transition_from_all_states(m, ACTION_BREAK,
        STATE_OFF | STATE_BROKEN, STATE_WORKING));
    
    assert(state_machine_add_transition_from_all_states(m, ACTION_FIX,
        STATE_OFF | STATE_WORKING, STATE_BROKEN));
    
    unsigned int state = STATE_OFF | STATE_WORKING;
    
    // print in the DOT graph description language.
    state_machine_print(stdout, m, "lamp", states, actions);
    
    while (1)
    {
        printf("Lamp is ");
        if ((state & STATE_OFF)    == STATE_OFF)    { printf("off"); }
        if ((state & STATE_ON)     == STATE_ON)     { printf("on"); }
        if ((state & STATE_BROKEN) == STATE_BROKEN) { printf(" and broken"); }
        
        printf(" (state: %u)\n", state);
        printf("Command? (t: toggle, b: break, f: fix, q: quit)\n");
        
        int c = input();
        unsigned int action;
        
        if ((c == 'q') || (c == EOF)) { break; }
        else if (c == 't') { action = ACTION_TOGGLE; }
        else if (c == 'b') { action = ACTION_BREAK;  }
        else if (c == 'f') { action = ACTION_FIX;  }
        else { printf("Invalid Command %c\n", c); continue; }
        
        unsigned int next = state_machine_take_action(m, state, action);
        if (next) { state = next; } else { printf("Nothing happens.\n"); }
    }
    
    return 0;
}
