# Perspective Design Doc

## Vision
### Design Vision

* A 3D Puzzle game with mechanics related to Perspective manipulation. Examples:
  * Change 3Cs from 3D to 2D Perspective and vice versa, where certain interactables and puzzles are solvable only in one of those perspectives or in a combination of both.
  * Rotating of floors, rooms, and entire levels in order to create an interesting puzzle design where the player has to think and calculate the right rotation combination to achieve his goal.
* Each puzzle (sublevel) should represent a unique/new challenge that the player needs to overcome.

### Technical Vision

* All features should be implemented mostly in C++, with Blueprints used when actually needed
* Emphasis on the 3Cs:
  * Camera - Smooth Camera transitions, Clear Camera view in 2D and Gameplay Camera System (Optional) 
  * Character Movement - Support smooth and clean Character Movement, either by using Advanced Locomotion System or Motion Matching 
  * Control - Support both Keyboard and Controller aiming for smooth and fun to use controls

## Controls

| Action          | Mouse & Keyboard | Controller (TODO)                         |
|-----------------|------------------|-------------------------------------------|
| Crouch (Toggle) | Left Ctrl        | Gamepad Face Button - Right               |
| Interact        | E                | Gamepad Face Button - Left                |
| Look            | Mouse XY 2D-Axis | Gamepad Right Thumbstick 2D-Axis          |
| Move Left       | A                | Gamepad Left Thumbstick X-Axis (Negative) |
| Move Right      | D                | Gamepad Left Thumbstick X-Axis (Positive) |
| Move Down       | S                | Gamepad Left Thumbstick Y-Axis (Negative) |
| Move Up         | W                | Gamepad Left Thumbstick Y-Axis (Positive) |
| Sprint (Hold)   | Left Shift       | Gamepad Left Thumbstick Button            |

## Features
### Player Character

The player character is an Epic Games demo character (taken from the Hour of Code project).

The player character should have basic movement capabilities, crouch, and interact.

### Puzzle Mechanics (in order of appearance)

Buttons - Buttons are basic interactable actors that can trigger other puzzle mechanic actions, such as opening a door, or turning off a gate.

Doors - Doors come in two variations, Sliding (like an elevator's door) and Rotating (Hinge door). A Sliding door can use a timer before closing once
more. A Rotating door can be used to not only open a path, but to close a path, resulting in smarter decision-making.

Gates - A Gate is a mechanic that transfers the character's control and view from 3D to 2D and vice versa, adding on challenges, such as paths being
unreachable once in 2D, or interactables that can only be activated in 2D or 3D.

Platforms - Platforms can be turned on/off making the player able to walk on it, or fall through it.

## Level Instances Purposes

Each level (except Level 4) will be split into Level Instances, each representing a sublevel of that level.

Each sublevel is independent of each other, and serves as a one small puzzle.

### Level 0 - Entrance (Tutorial Level + 2D)

| ID      | Main Purpose                                                           |
|---------|------------------------------------------------------------------------|
| LVLI_00 | Introduce - Controls, Button, Sliding Door, Closing Sliding Door       |
| LVLI_01 | Introduce - Rotating Doors                                             |
|         | Puzzle - Player must interact with interactables in the right order    |
| LVLI_02 | Puzzle - Player must experiment with interactables to find the way out |
| LVLI_03 | Introduce - Gates, Platforms                                           |
|         | Puzzle - Player must exit by going through 2 gates                     |
| LVLI_04 | Puzzle - Player must find the way out while facing all LVL_0 mechanics |

### Level 1 - Floor 1 (2D Continued)

| ID      | Main Purpose |
|---------|--------------|
| LVLI_10 | TODO         |
| LVLI_11 | TODO         |
| LVLI_12 | TODO         |
| LVLI_13 | TODO         |
| LVLI_14 | TODO         |

### Level 2 - Floor 2 (Gravity)

| ID      | Main Purpose |
|---------|--------------|
| LVLI_20 | TODO         |
| LVLI_21 | TODO         |
| LVLI_22 | TODO         |
| LVLI_23 | TODO         |
| LVLI_24 | TODO         |

### Level 3 - Floor 3 (Avoidability)

| ID      | Main Purpose |
|---------|--------------|
| LVLI_30 | TODO         |
| LVLI_31 | TODO         |
| LVLI_32 | TODO         |
| LVLI_33 | TODO         |
| LVLI_34 | TODO         |

### Level 4 - Rooftop (Madness)

The final level won't contain any sublevel level instances, instead it will be a one big level containing all the features from previous levels,
resulting in one big puzzle.

## Concepts, Sketches & Inspirations

<!---
Esher (M. C. Esher).
Portal.
-->

None at the moment…