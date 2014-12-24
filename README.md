# Make fun of a bouncing ball
### Developed by Ran Bao and Kane Williams

The directory structure is:
apps         --- contains a sub-directory for each application
drivers      --- device driver modules (hardware independent)
drivers/avr  --- device driver modules specifically for AVR architecture
drivers/test --- device driver modules for test scaffold
doc          --- documentation
etc          --- miscellaneous scripts and makefile templates
fonts        --- fonts and font creation program
utils        --- utility modules
src          --- source file

---------------
## Updates

15-10-2014:
Finish IR transmission. 
New end of game screen.
We have finished!

15-10-2014:
Added health and player collision.

13-10-2014:
Player shoots ball with correct position and direction.
No longer uses a list of bouncing balls. (Can fire indefinitely)

12-10-2014: 
Add feature that simulate the ball collision with multiple balls.
Balls now get disabled upon hitting player's wall.

11-10-2014: 
New system with simulating multiple balls.

11-10-2014:
Bullet speed depends on size of player.

09-10-2014:
Add the feature that player can deselect the exist dot when creating new avatar.
Player can now shoot the ball.

08-10-2014:
implement the movement of player using circular shift.
Add misc module that add some support functions for the game.
 
08-10-2014:
Introduce the module avatar, for which the player can cofigure their avatar using a small cursor and move around when the button is pressed.

06-10-2014:
Introduce the function 'task_stop()' into task module. The task scheduler can exit the entire task anywhere

