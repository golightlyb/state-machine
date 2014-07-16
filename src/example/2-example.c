/*
 * This example uses a provided state machine for modelling a GUI button
 * element and demonstrates how to apply an alphabet of GUI mouse and keyboard
 * input operations to supplied state machine.
 * 
 * This model is completely decoupled from how the button is displayed or its
 * physical dimensions or any actual input -- this is the responsibility of the
 * implementor.
 * 
 * Different types of GUI elements share the same basic alphabet of actions
 * with a similar set of states. Implementing a completely new type of element
 * with different behaviour is simply a matter of defining a new machine to
 * model it.
 * 
 * See for example, state-machine/models/gui/button.c
 */

// Public Domain BSAG 2014

#include "state-machine/state-machine.h"
#include "state-machine/models/gui.h"
#include <stdio.h>
#include <assert.h>


int input(void)
{
    int c;
    
    do
    {
        c = fgetc(stdin);
    } while (c == '\n');
    
    return c;
}


// shorthand
#define S(x) STATE_GUI_BUTTON_##x
#define A(x) ACTION_GUI_##x
#define HAS(state, flag) ((state & flag) == flag)

int main(void)
{
    state_machine *m = state_machine_new_gui_button();
    assert(m);
    
    unsigned int state = S(DEFAULT);
    
    while (1)
    {
        printf("Button is ");
        if (HAS(state, S(DISABLED))) { printf("disabled, "); }
        if (HAS(state, S(ENABLED))) { printf("enabled, "); }
        if (HAS(state, S(HOVERED))) { printf("hovered, "); }
        if (HAS(state, S(NOT_HOVERED))) { printf("not hovered, "); }
        if (HAS(state, S(FOCUSED))) { printf("focused, "); }
        if (HAS(state, S(UNFOCUSED))) { printf("unfocused, "); }
        if (HAS(state, S(ACTIVE))) { printf("active, "); }
        if (HAS(state, S(INACTIVE))) { printf("inactive, "); }
        if (HAS(state, S(CLICKED))) { printf("clicked.\n"); }
        if (HAS(state, S(NOT_CLICKED))) { printf("not clicked.\n"); }
        
        printf("Command?\n");
        printf("    0: Enable\n");
        printf("    1: Disable\n");
        printf("    2: Mouse Enter\n");
        printf("    3: Mouse Leave\n");
        printf("    4: Mouse Down\n");
        printf("    5: Mouse Up\n");
        printf("    6: Key Down\n");
        printf("    7: Key Up\n");
        printf("    8: Accelerator\n");
        printf("    9: Input\n");
        printf("    A: Focus\n");
        printf("    B: Unfocus\n");
        printf("    C: Continue (from clicked state)\n");
        
        int c = input();
        unsigned int action;
        
        if ((c == 'q') || (c == EOF)) { break; }
        else if (c == '0') { action = A(ENABLE); }
        else if (c == '1') { action = A(DISABLE); }
        else if (c == '2') { action = A(MOUSE_ENTER); }
        else if (c == '3') { action = A(MOUSE_LEAVE); }
        else if (c == '4') { action = A(MOUSE_DOWN); }
        else if (c == '5') { action = A(MOUSE_UP); }
        else if (c == '6') { action = A(KEY_DOWN); }
        else if (c == '7') { action = A(KEY_UP); }
        else if (c == '8') { action = A(ACCELERATOR); }
        else if (c == '9') { action = A(INPUT); }
        else if (c == 'A') { action = A(FOCUS); }
        else if (c == 'B') { action = A(UNFOCUS); }
        else if (c == 'C') { action = A(CONTINUE); }
        else { printf("Invalid Command %c\n", c); continue; }
        
        unsigned int next = state_machine_take_action(m, state, action);
        if (next) { state = next; } else { printf("Nothing happens.\n"); }
    }
    
    return 0;
}

