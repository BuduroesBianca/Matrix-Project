# ssSnake Game

## Game Description

This is a reinterpreted version of the iconic Snake Game. 
  
### How to play

The goal of the player is to gather as much food as possible. The game ends when the ssSnake reaches the position of eating itself (lost state) or when it occupies the whole map (win state).
  - Dificulty levels
     * Easy - the snake has a relatively low speed that is constant throughout the game. One food is worth 1 point.
     * Medium - the snake speed is increasing in easy speed - hard speed interval after each food eaten. One food is worth 2 points.
     * Hard - the snake has a high speed that is constant throughout the game. One food is worth 3 points.

Dificulty level is not saved in EEPROM, so when you play for the first time you will start playing at the easy level. You can change it from the Menu -> Set Dificulty.

### Game States

- Lost Game
  * Lost Game and isn't High Score -> message "Oh.. You killed the ssSnake :(" will be shown on the LCD Display and then redirected to Menu.
  * Lost Game and is High Score -> on line 1 will be shown "HIGH SCORE" and on line 2 the button "> Next".When button "> Next" is clicked, the player has to set his name by first clicking the letter position (chosen by moving the joystick left-right) and then select the letter (by moving the joystick up-down); the name can have maximum 7 letters and if it is for example something like this "n me" the value saved will be "nme". When the name is complete, the user must press "> OK" and it will be redirected to Menu and the name and high score will be saved in high score list.
- Win Game
  * If the user selected level Easy and he has a score of 64 points
  * If the user selected level Medium and he has a score of 128 point
  * If the user selected level Hard and he has a score of 192 points

## Hardware Components

- 16x2 LCD Display (to display the menu)
- 8x8 Led Matrix (to display the game)
- Joystick (to navigate through menu and to controll the ssSnake)
- Buzzer (sounds)
- Arduino UNO Board

## MENU

1. Start game 
    - Start game -> The game will start after the intro, the player must move the joystick in order to start playing.
    - Back -> This option is here in case that you clicked by accident on Start Game.
2. High Scores 
    - Top 3 High Scores will be shown here.
3. About
    - The player will found here details about the creator and about the game
4. Set Sound
  - The player has the option to set sound on or off and it will be saved to EEPROM.
5. Set Contrast -> set and save to EEPROM values between 
6. Set Brightness -> set and save to EEPROM
7. Set Led Intensity -> set Led Intensity on Game Matrix. When this setting is accesed, the whole matrix will light up so the player can set it easily. Values between 1-6
8. Set dificulty -> EASY/MEDIUM/HARD and this value won't pe saved to EEPROM so new users start at level Easy

## Implementation Details

- Menu
  * Different states are declared and changed when the player is navigating through Menu and depending on these states the corresponding pages are displayed.
  *  The button on the joystick has attached an interrupt function to change between states.
  *  The current state can be indentified by looking at posMenu and posSubemnu.

- Game
  * Food is random generated and then placed on the map
  * The snake direction is calculated
  * Snake movement is calculated

- Snake Movement:
he logic behind this is that we constantly light up the new head and turn off the tail of the snake.
At the beginning of the game, this is how our snake of length 3 will look on the matrix: 3(head) - 2 - 1(tail). If we move the joystick we have these next cases:
  * Case 1 - The food is not found:  the current position on the matrix will be the new head, so we replace it with snakeLength + 1. We have now something like this: 4(new head) - 3 - 2 - 1(tail). Immediately after that, we substract each part of the snake => 3(new head) - 2 - 1(new tail) - 0(old tail). 
  * Case 2 - The food is found: we first add 1 by each body part of the snake => 4(head) - 3 - 2(tail) and increment snakeLength. After that we repeat the steps from Case 1. The result 4(head) - 3 - 2 - 1(tail).


### Video showcasing the functionality

Video can be found [here](https://youtu.be/ze4zzD_XR7A).



