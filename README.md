# 2D Bubble Game Project

## Team Members
- Diego Rodriguez Milagro  
- Lluis Itarte Sahun  

## Note
- In the **binary executable**, the game sound does not work due to directory issues. To play the game with sound, run the following file:  
  `\projecte\2DGame\02-Bubble\Release\02-Bubble.exe`.  
  We apologize for the inconvenience.  

## Implemented Features
- **Basic structure of 4 screens**:
  - Main menu
  - Play
  - Instructions
  - Credits (displayed after losing all lives or completing all 3 levels)
- **Three levels** with requested configurations.
- **Bubbles** affected by gravity, bouncing off the environment, blocks, and barriers. If a bubble touches the player, the player loses a life.
- **Shootable hook** that destroys bubbles and breakable barriers.
- **Three power-ups** obtained by destroying bubbles:
  - **Sticky**: The weapon sticks to non-destructible blocks.
  - **Double**: The player can shoot a second hook while the first is still in use.
  - **Stop Time**: Bubbles freeze for 4 seconds. During the last second, the bubbles blink.
- **Food items** that the player can collect for 1000 or 2000 points.
- **Combo system** for popping bubbles and collecting food.
- The player can **climb up and down ladders**.
- **Level timer**: Each level must be completed in 100 seconds; otherwise, the player loses a life.
- **Game over**: When the player runs out of lives, the credits screen is displayed.
- **Graphical interface**:
  - Displays remaining lives, remaining time, level name, active power-ups (except Stop Time, indicated by blinking bubbles), and whether God Mode is active.
- **Shortcut keys** for skipping content, gaining power-ups, and toggling God Mode:
  - `0`: Main menu
  - `1`: Level 1
  - `2`: Level 2
  - `3`: Level 3
  - `4`: Credits
  - `T`: Sticky power-up
  - `Y`: Double power-up
  - `U`: Stop Time power-up
  - `G`: Toggle God Mode
- **Sounds and music**.
- **Interactions** with game entities:
  - Bubble explosion.
  - End of Stop Time power-up effect.
  - Destruction of breakable blocks.

## Game Instructions
- The game executable is located in the **binary** folder (as requested). Upon opening, the main menu is displayed.
- Press `S` to navigate to the instructions screen.
- Wait for 10 seconds or press `S` again to start the first level.
- **Controls**:
  - Move left and right: Arrow keys (left, right).
  - Climb ladders: Arrow keys (up, down).
  - Shoot: `S`.
- **Progression**:
  - Destroy all bubbles to advance to the next level.
  - Completing level 1 progresses to level 2; completing level 2 progresses to level 3.
  - After completing level 3, the credits screen is shown.
- From the credits screen, press `S` to return to the main menu.

## Known Issues
- Due to time constraints, some bugs and mechanics are not fully functional. We prioritized implementing features. Known issues include:
  - **Bubble collisions**: Occasionally fail, causing bubbles to unexpectedly change direction or leave the map, making levels impossible to complete.
  - **Weapon mechanics**: The weapon's top part starts from the player's head, but the bottom part always starts from the bottom. While this does not affect levels 1 and 2, it makes level 3 very easy, as the player can destroy all bubbles from the safety of the ladders.
