Experiment in using a state machine to specify the low-level behaviour of
Graphical User Interface (GUI) elements, as opposed to a typical Object Oriented
approach.

See `src/example/1-example.c` for a demonstration of how to construct a simple
state machine and interact with it.

See `src/example/2-example.c` for a demonstration of how to apply an alphabet
of GUI mouse and keyboard input operations to supplied state machine for a
button.

See `src/state-machine/models/` for sample state machines specified using
this system.

Hopefully advantages of this system include:

* Ability to rigourously test the behaviour of GUI elements (e.g. with a graph
  search of the automata to formally prove properties).
* Very clear separation of the behaviour of GUI elements from display logic and
  from having to deal with the mouse and keyboard.
    + Layout elements (containers, hboxes) are treated by most other frameworks
      in the same way as they treat input elements like buttons. Maybe these
      shouldn't be conflated so readily.
    + These input elements can be lower-level building blocks for different
      types of GUIs with different requirements. In particular, computer game
      applications may want to completely own input and not have to fight for
      it with the GUI framework.
    + Parent application has complete control over how to draw elements, but
      doesn't have to worry about the tedious logic.
* Can be extended with any type of GUI element without having stupid OOP things
  like a "RadioButton inherits from a CheckButton inherits from a ToggleButton
  inherits from a Button inherits from a Container inherits from a Widget" and
  the associated headache of worrying about where things go.
* Advantages in simplifying logic like "can this element get keyboard focus?"
  when it may not be able to. Simply send the focus action, and if the automata
  accepts it then you're done. Similarly there may be reasons why an automata
  moves to a state where it loses focus (what happens if the program disables
  the currently focused element?) but all you have to check is if the automata
  has moved into a state where the focus flag isn't set (states are just bitmasks
  of their properties e.g. (ENABLED | FOCUSED | HOVERED) is a very useful state
  ID!)
