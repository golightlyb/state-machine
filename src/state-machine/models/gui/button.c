/*
 
 state-machine/models/gui/button.c -
 defines a state machine to model the basic behaviour of a typical GUI button
 
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

#include "base.h"
#include "state-machine/state-machine.h"
#include "state-machine/models/gui.h"
#include <assert.h>

#define S(x) STATE_GUI_BUTTON_##x
#define A(x) ACTION_GUI_##x

#define S_Dh (S(DISABLED) | S(NOT_HOVERED)  | S(UNFOCUSED) | S(INACTIVE) | S(NOT_CLICKED))
#define S_DH (S(DISABLED) | S(HOVERED)  | S(UNFOCUSED) | S(INACTIVE) | S(NOT_CLICKED))

#define S_Ehfac (S(ENABLED) | S(NOT_HOVERED) | S(UNFOCUSED) | S(INACTIVE) | S(NOT_CLICKED))

#define S_EHfac (S(ENABLED) | S(HOVERED) | S(UNFOCUSED) | S(INACTIVE) | S(NOT_CLICKED))
#define S_EhFac (S(ENABLED) | S(NOT_HOVERED) | S(FOCUSED) | S(INACTIVE) | S(NOT_CLICKED))
#define S_EhfAc (S(ENABLED) | S(NOT_HOVERED) | S(UNFOCUSED) | S(ACTIVE) | S(NOT_CLICKED))
#define S_EhfaC (S(ENABLED) | S(NOT_HOVERED) | S(UNFOCUSED) | S(INACTIVE) | S(CLICKED))

#define S_EHFac (S(ENABLED) | S(HOVERED) | S(FOCUSED) | S(INACTIVE) | S(NOT_CLICKED))
#define S_EHfAc (S(ENABLED) | S(HOVERED) | S(UNFOCUSED) | S(ACTIVE) | S(NOT_CLICKED))
#define S_EHfaC (S(ENABLED) | S(HOVERED) | S(UNFOCUSED) | S(INACTIVE) | S(CLICKED))
#define S_EhFAc (S(ENABLED) | S(NOT_HOVERED) | S(FOCUSED) | S(ACTIVE) | S(NOT_CLICKED))
#define S_EhFaC (S(ENABLED) | S(NOT_HOVERED) | S(FOCUSED) | S(INACTIVE) | S(CLICKED))
//#define S_EhfAC (S(ENABLED) | S(NOT_HOVERED) | S(UNFOCUSED) | S(ACTIVE) | S(CLICKED))

#define S_EHFAc (S(ENABLED) | S(HOVERED) | S(FOCUSED) | S(ACTIVE) | S(NOT_CLICKED))
#define S_EHFaC (S(ENABLED) | S(HOVERED) | S(FOCUSED) | S(INACTIVE) | S(CLICKED))
//#define S_EHfAC (S(ENABLED) | S(HOVERED) | S(UNFOCUSED) | S(ACTIVE) | S(CLICKED))
//#define S_EhFAC (S(ENABLED) | S(NOT_HOVERED) | S(FOCUSED) | S(ACTIVE) | S(CLICKED))

state_machine *state_machine_new_gui_button(void)
{
    state_machine *m = state_machine_new(23, NUM_ACTIONS_GUI);
    if (!m) { X(state_machine_new); }
    
    assert(state_machine_add_state(m, S_Dh));
    assert(state_machine_add_state(m, S_DH));
    
    assert(state_machine_add_state(m, S_Ehfac));
    
    assert(state_machine_add_state(m, S_EHfac));
    assert(state_machine_add_state(m, S_EhFac));
    assert(state_machine_add_state(m, S_EhfAc));
    assert(state_machine_add_state(m, S_EhfaC));
    
    assert(state_machine_add_state(m, S_EHfac));
    assert(state_machine_add_state(m, S_EhFac));
    assert(state_machine_add_state(m, S_EhfAc));
    assert(state_machine_add_state(m, S_EhfaC));
    
    assert(state_machine_add_state(m, S_EHFac));
    assert(state_machine_add_state(m, S_EHfAc));
    assert(state_machine_add_state(m, S_EHfaC));
    assert(state_machine_add_state(m, S_EhFAc));
    assert(state_machine_add_state(m, S_EhFaC));
    //assert(state_machine_add_state(m, S_EhfAC));
    
    assert(state_machine_add_state(m, S_EHFAc));
    assert(state_machine_add_state(m, S_EHFaC));
    //assert(state_machine_add_state(m, S_EHfAC));
    //assert(state_machine_add_state(m, S_EhFAC));
    
    assert(state_machine_add_transition(m, A(ENABLE),       S_Dh, S_Ehfac));
    assert(state_machine_add_transition(m, A(MOUSE_ENTER),  S_Dh, S_DH));
    
    assert(state_machine_add_transition(m, A(ENABLE),       S_DH, S_EHfac));
    assert(state_machine_add_transition(m, A(MOUSE_LEAVE),  S_DH, S_Dh));
    
    assert(state_machine_add_transition(m, A(MOUSE_ENTER),  S_Ehfac, S_EHfac));
    
    assert(state_machine_add_transition(m, A(MOUSE_LEAVE),  S_EHfac, S_Ehfac));
    assert(state_machine_add_transition(m, A(MOUSE_DOWN),   S_EHfac, S_EHFAc));
    
    assert(state_machine_add_transition(m, A(DISABLE),      S_EhFac, S_Dh));
    assert(state_machine_add_transition(m, A(MOUSE_ENTER),  S_EhFac, S_EHFac));
    assert(state_machine_add_transition(m, A(KEY_DOWN),     S_EhFac, S_EhFAc));
    assert(state_machine_add_transition(m, A(KEY_DOWN),     S_EhFac, S_EhFAc));
    
    // note -- caller is supposed to observe gain or loss of focus by an
    // element which may mean taking the appropriate steps to unfocus or focus
    // a neighbour or previously focused element
    
    // for all enabled states, add a disable transition directly to the
    // equivalent disabled state while maintaining only any hovered state
    assert(state_machine_add_transition_from_all_states_replacing(m, A(DISABLE),
            ~(S(HOVERED) | S(NOT_HOVERED)),
            S_DH & ~(S(HOVERED) | S(NOT_HOVERED)),
            S(ENABLED)));
    
    // for all enabled unfocused states, add a focus transition directly
    // to the equivalent enabled focused state.
    assert(state_machine_add_transition_from_all_states_replacing(m, A(FOCUS),
        S(UNFOCUSED), S(FOCUSED), S(ENABLED) | S(UNFOCUSED)));
    
    // for all enabled focused states, add an unfocus transition directly
    // to the equivalent enabled unfocused state
    assert(state_machine_add_transition_from_all_states_replacing(m, A(UNFOCUS),
        S(FOCUSED), S(UNFOCUSED), S(ENABLED) | S(FOCUSED)));
    
    // for all non-hovered enabled states, add a mouse enter transition directly
    // to the equivalent hovered state
    assert(state_machine_add_transition_from_all_states_replacing(m, A(MOUSE_ENTER),
            S(NOT_HOVERED), S(HOVERED), S(ENABLED) | S(NOT_HOVERED)));
    
    // for all hovered enabled states, add a mouse leave transition directly to
    // the equivalent unhovered state
    assert(state_machine_add_transition_from_all_states_replacing(m, A(MOUSE_LEAVE),
        S(HOVERED), S(NOT_HOVERED), S(ENABLED) | S(HOVERED)));
    
    // for all hovered enabled inactive states, add a mouse down transition to 
    // the equivalent active state. Clicked states cannot be active. This
    // focuses the element.
    assert(state_machine_add_transition_from_all_states_replacing(m, A(MOUSE_DOWN),
        S(INACTIVE) | S(UNFOCUSED), S(ACTIVE) | S(FOCUSED), S(ENABLED) | S(HOVERED) | S(INACTIVE) | S(NOT_CLICKED)));
    
    // for all hovered enabled active states, add a mouse up transition to
    // the clicked state
    assert(state_machine_add_transition_from_all_states_replacing(m, A(MOUSE_UP),
        S(ACTIVE) | S(NOT_CLICKED), S(INACTIVE) | S(CLICKED), S(ENABLED) | S(HOVERED) | S(ACTIVE)));
    
    // and for all the non-hovered enabled active states, add a mouse up
    // transition back to the non-hovered enabled state
    assert(state_machine_add_transition_from_all_states_replacing(m, A(MOUSE_UP),
        S(ACTIVE), S(INACTIVE), S(ENABLED) | S(NOT_HOVERED) | S(ACTIVE)));
    
    // for all focused enabled inactive states, add a key down transition to 
    // the equivalent active state. Clicked states cannot be active.
    assert(state_machine_add_transition_from_all_states_replacing(m, A(KEY_DOWN),
        S(INACTIVE), S(ACTIVE), S(ENABLED) | S(FOCUSED) | S(INACTIVE) | S(NOT_CLICKED)));
    
    // for all active states, add a key up transition to the clicked state.
    assert(state_machine_add_transition_from_all_states_replacing(m, A(KEY_UP),
        S(ACTIVE) | S(NOT_CLICKED), S(INACTIVE) | S(CLICKED), S(ENABLED) | S(ACTIVE)));
    
    // for all enabled states, add an accelerator transition directly to the
    // equivalent clicked state, adding focus.
    assert(state_machine_add_transition_from_all_states_replacing(m, A(ACCEL),
        S(NOT_CLICKED) | S(UNFOCUSED) | S(ACTIVE),
        S(CLICKED) | S(FOCUSED) | S(INACTIVE),
        S(ENABLED) | S(NOT_CLICKED)));
    //assert(state_machine_add_transition_from_all_states_replacing(m, A(ACCEL),
     //   S(NOT_CLICKED), S(CLICKED), S(ENABLED) | S(FOCUSED) | S(NOT_CLICKED)));
    
    // for all clicked states, add a continue transition to mark the state
    // as handled, directly back to the equivalent unclicked state.
    assert(state_machine_add_transition_from_all_states_replacing(m, A(CONTINUE),
        S(CLICKED), S(NOT_CLICKED), S(CLICKED)));
    
/*
# define ACTION_GUI_ENABLE       0
# define ACTION_GUI_DISABLE      1
# define ACTION_GUI_MOUSE_ENTER  2
# define ACTION_GUI_MOUSE_LEAVE  3
# define ACTION_GUI_MOUSE_DOWN   4 // activation using the mouse
# define ACTION_GUI_MOUSE_UP     5
# define ACTION_GUI_KEY_DOWN     6 // activation using the keyboard e.g. enter, space
# define ACTION_GUI_KEY_UP       7
# define ACTION_GUI_ACCELERATOR  8 // single-operation click
                        // ACCEL
# define ACTION_GUI_INPUT        9 // e.g. any typed input
# define ACTION_GUI_FOCUS       10
# define ACTION_GUI_UNFOCUS     11
# define ACTION_GUI_CONTINUE    12 // for when a state blocks until handled
# define NUM_ACTIONS_GUI        13
 */
    
    /*
    assert(state_machine_add_state(m, STATE_OFF | STATE_WORKING));
    assert(state_machine_add_state(m, STATE_ON  | STATE_WORKING));
    assert(state_machine_add_state(m, STATE_OFF | STATE_BROKEN));
    
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
    */
    
    return m;
    
    err_state_machine_new:
        return NULL;
}
