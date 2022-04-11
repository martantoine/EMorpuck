# EMorpuck
## Name
As you might have guessed, EMorpuck is the contraction (the game) of morpion and E-Puck2 (the robot)

## Description
The robot is able to play against a real player at the game "morpion".
For the sake of simplicity, the robot play only with the cross (easier to draw than a circle).
The robot use a combination of a camera with a mirror to scan the game.
The robot state machine is the following :
1. idle in the idle position (a case away from the 3x3 playing area)
2. scanning, enabled by approching ones hand to the robot's proximity sensors
3. reaching the case to be crossed, enter this mode once the entire paper is scanned
4. drawing the cross in the previously reached case
5. reaching the idle position again
The normal state machine transitions are 1->2->3->4->5->1

## Installation
### Prerequists
- ARM GCC Toolchain (with path to gcc, gdb added to environnement variable)
- Visual Studio Code
- Cortex-Debug extension by marus25
- C/C++ extensiion from Microsoft

## Authors and acknowledgment
- Antoine Martin (Martantoine)
- Alexandre Duval