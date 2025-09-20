# JoystickToMouseApplication

## Joystick to Mouse Application, written in C++ using Qt, SDL, and CMake

This application converts Joystick/Controller input into mouse inputs via SDL3.
My goals for this application are to gain experience handling hardware input and convert them into certain actions on Computer.

## Features

Functionality and GUI is built in Qt 6.9.2 and reads in whatever joystick/controller is plugged in and maps button 0 and 1 to left and right click, whilst movement is handled via joystick movement.
The "hat" controls on the Joystick act as the scroll wheel. Currently there are only two screens, main and activation, both can traverse too and from each other.
Keybinds can be changed in the settings tab, currently left and right click buttons can be changed.
I am testing this all with a Logitech 3D Extreme Pro Joystick.

## Tech Stack
- C++ 20
- Qt 6.9.2
- SDL3
- CMake 3.21

## Future Plans

As I have just started implementing this idea into Qt, I still many plans on user customization, some of which include:
- Ability to switch between multiple connected Joysticks/Controllers
- Differentiate between Joysticks/Controllers
- Ability to change sensitivity and DPI controls
- Movement Visual to display current joystick movement
