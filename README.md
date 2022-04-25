# BulletHell
Bullet-hell game

Functionalities:

1. Preparation of graphics for the game: stage view, a place for displaying additional information: time elapsed from the start of the level.
2. Implementation of control using the following keys:
  a. Esc: exit the program - the game shuts down immediately;
  b. n: new game;
  c. arrows - movement
  d. m - menu
3. Implementation of one level of the game. The stage several times wider and higher than the displayed view. Traversal displays the visible part of the stage.
4. Elapsed time for a level
5. Implementation of one, static enemy shooting in some simple pattern.

Extra functionalities:

1. Advanced enemies:
  a. 3 types of enemies with different shooting patterns made of simple bullets
    i. bullets form a shape on the screen e.g. circles expanding in waves or a spiral
    ii. a shooting pattern of an enemy is not be static through the whole level
    iii. one type of an enemy is moving
  b. 3 stages which demonstrate different configurations (one enemy per stage)
2. Shootout:
  a. the player character can fight back and shoot as well
  b. implemented hitboxes for both sides of the conflict
  c. after being hit the player cannot be hit again for a brief period of time (invincibility frames)
  d. tracking number of hits for each part
3. Health, death and menu:
  a. health is displayed in a graphic format
  b. menu allows the player to navigate through the implemented options:
    i. start a new game on a given level
    ii. quit game
  c. depleting health of the player displays a game over and asks for another try or leads back to menu
  d. depleting health of the enemy prompts the player to access the next level
4. Keep track of the score:
  a. the score increases when the enemy is hit and depletes when the player character is hit
    i. multiple hits in a row increases the magnitude of change
  b. bonus points:
    i. find and collect bonus health appearing randomly in the level
    
    
    
