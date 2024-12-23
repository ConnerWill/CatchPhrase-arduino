# CatchPhrase Game on Arduino

This project implements the popular party game CatchPhrase on an Arduino microcontroller. Players can select categories, guess words, and keep track of scores using physical buttons, an LCD display, and a speaker for sound effects. The first team to reach 7 points wins.

## Features

- **Multiple Categories**: Players can choose from multiple categories of words.
- **Random Word Selection**: Words are randomly selected within the chosen category.
- **Score Tracking**: Keeps score for Team 1 and Team 2, displaying it on the LCD screen.
- **Winning Condition**: The first team to score 7 points wins.
- **Sound Effects**: Countdown timer and winner sound effects are played via a connected speaker.
- **Button Controls**:
  - `Go/Stop`: Start or pause the game.
  - `Next Word`: Display the next word.
  - `Team 1 Point`: Increment Team 1's score.
  - `Team 2 Point`: Increment Team 2's score.

## Hardware Requirements

- Arduino board (e.g., Uno, Nano)
- LCD display (16x2 or similar)
- Push buttons (4 total)
- Speaker
- Resistors for button pull-up configuration (if necessary)
- Connecting wires and breadboard (optional for prototyping)

## Pin Connections

| Component         | Arduino Pin |
|-------------------|-------------|
| Go/Stop Button    | 2           |
| Next Word Button  | 3           |
| Team 1 Point      | 4           |
| Team 2 Point      | 5           |
| Speaker           | 6           |
| LCD RS            | 7           |
| LCD E             | 8           |
| LCD D4            | 9           |
| LCD D5            | 10          |
| LCD D6            | 11          |
| LCD D7            | 12          |

## Software Requirements

- Arduino IDE (latest version)
- LiquidCrystal library (pre-installed in Arduino IDE)

## Setup and Installation

1. **Hardware Assembly**:
   - Connect the LCD display to the specified pins.
   - Attach the push buttons to the appropriate pins with pull-up resistors if needed.
   - Connect the speaker to pin 6.
2. **Code Upload**:
   - Open the provided `.ino` file in the Arduino IDE.
   - Adjust the LCD dimensions if necessary by modifying `lcd.begin(16, 2)` in the `setup()` function.
   - Upload the code to your Arduino board.
3. **Power the Arduino**:
   - Use a USB cable or external power supply to power the Arduino.

### Schematics

![Schematics](../schematics/schematics.png)

*Schematics file can be found in the [`schematics`](../schematics) directory.*

## How to Play

1. Power on the Arduino. The LCD will display a welcome message and available categories.
2. Use the `Go/Stop` button to start the game.
3. Use the `Next Word` button to generate a random word within the selected category.
4. Teams take turns guessing the displayed word.
5. Press `Team 1 Point` or `Team 2 Point` to award points to the respective team.
6. The first team to score 7 points wins. The winner sound will play, and the LCD will display the winning message.
7. Reset the game by pausing and restarting using the `Go/Stop` button.

## Customization

- **Add Categories**:
  - Edit the `categories` array and add new word lists in the code.
  - Update the `wordLists` array and `wordCounts` array with the new data.
- **Adjust Winning Score**:
  - Modify the `winningScore` constant.
- **Sound Effects**:
  - Customize the `playCountdownSound` and `playWinnerSound` functions to change the sound effects.

## Notes

- Ensure all components are properly connected and the buttons are debounced for reliable input.
- Use a compatible LCD size and adjust the code if required.
- Use `PROGMEM` for efficient memory usage on the Arduino.
