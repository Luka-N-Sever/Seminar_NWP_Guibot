# Seminar_NWP_Guibot
a guibot
Commands are selected from a listbox {"LEFT","UP","RIGHT","DOWN"}, and the numeric value associated
with a movement is written into the top edit control.
On "Add" the selected movement is added with the numeric value into another
list box.
Commands can be added and removed from the list box.
On "Execute" the commands added to the list box with their numeric values
are executed one by one.
The commands can be observed taking effect on a small "Guibot" window
(Create Bot).
These commands can be looped, and the number of desired loops can be
written into the bottom edit control.
A hidden source file describes the movemnents of an actual robot looking device, the design of which can be seen by clicking "Schematic" from the menu.
These movements base themselves on the unit circle and degrees to calculate the position of a leg as it moves up and down.
These points are fed into a vector, which can then be used to animate the robot's movement.
