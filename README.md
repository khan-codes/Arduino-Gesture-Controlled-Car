# Arduino-Gesture-Controlled-Car
Code for an Arduino based project
Gesture + Joystick Controlled Car:
This project has two main apparatuses, first the robot chassis and second the remote. The remote has two modes, gesture and joystick.
By default the system is in gesture control mode we can toggle it to joystick control with a push button.
In gesture mode the gravitational acceleration (orientation) of the remote is measured and an instruction is generated for the robot.
The joystick module basically is a pair of two potentiometers, one for the x-axis and one for the y-axis.
The values of resistance from potentiometer are measured and again an instruction is generated for the robot.
The communication between the remote and the robot is established via radio frequency, its range is from 90ft to 100ft.
