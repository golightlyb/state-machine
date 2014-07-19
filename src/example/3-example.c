/*
 * This example uses a provided state machine for modelling a GUI button
 * element and simply outputs it in the DOT graph description language.
 */

// Public Domain BSAG 2014

#include "state-machine/state-machine.h"
#include "state-machine/models/gui.h"
#include <stdio.h>
#include <assert.h>

int main(void)
{
    state_machine *m = state_machine_new_gui_button();
    assert(m);
    
    const char **states = state_machine_gui_button_state_strings();
    const char **actions = state_machine_gui_action_strings();
    
    state_machine_print(stdout, m, "button model", states, actions);
    
    return 0;
}

