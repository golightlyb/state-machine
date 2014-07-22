#include "state-machine/state-machine.h"

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
# define ACTION_GUI_SCROLL      13
# define NUM_ACTIONS_GUI        14

#define STATE_GUI_ENABLED        1u // TODO make states common too
#define STATE_GUI_DISABLED       2u
#define STATE_GUI_HOVERED        4u
#define STATE_GUI_NOT_HOVERED    8u
#define STATE_GUI_FOCUSED       16u
#define STATE_GUI_UNFOCUSED     32u
#define STATE_GUI_ACTIVE        64u
#define STATE_GUI_INACTIVE     128u
#define STATE_GUI_CLICKED      256u
#define STATE_GUI_NOT_CLICKED  512u
#define STATE_GUI_CHECKED     1024u
#define STATE_GUI_UNCHECKED   2048u

#define NUM_STATES_GUI 32 // limit on number of state flag combinations used

#define STATE_GUI_BUTTON_DEFAULT (0 \
    | STATE_GUI_ENABLED \
    | STATE_GUI_NOT_HOVERED \
    | STATE_GUI_UNFOCUSED \
    | STATE_GUI_INACTIVE \
    | STATE_GUI_NOT_CLICKED \
    )

state_machine *state_machine_new_gui_button(void);

const char **state_machine_gui_button_state_strings(void);

const char **state_machine_gui_action_strings(void);
