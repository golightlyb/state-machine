#include "state-machine/models/gui.h"
#include <assert.h>

const char **state_machine_gui_action_strings(void)
{
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
# define ACTION_GUI_ACCEL        ACTION_GUI_ACCELERATOR
# define ACTION_GUI_INPUT        9 // e.g. any typed input
# define ACTION_GUI_FOCUS       10
# define ACTION_GUI_UNFOCUS     11
# define ACTION_GUI_CONTINUE    12 // for when a state blocks until handled
 */
    static const char *actions[] =
    {
        "enable",
        "disable",
        "mouse enter",
        "mouse leave",
        "mouse down",
        "mouse up",
        "key down",
        "key up",
        "accelerator",
        "input",
        "focus",
        "unfocus",
        "continue"
    };
    
    assert(NUM_ACTIONS_GUI == 13); // reminder to update the string table
    
    return actions;
}
