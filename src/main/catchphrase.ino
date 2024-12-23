#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

// Pin assignments
const int buttonGoStop = 2; // Pin for the Go/Stop button
const int buttonNextWord = 3; // Pin for the Next Word button
const int buttonTeam1Point = 4; // Pin for Team 1 Point button
const int buttonTeam2Point = 5; // Pin for Team 2 Point button
const int speakerPin = 6; // Pin for the speaker

// Initialize LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Connects the LCD to these specific pins

// Game variables
int team1Score = 0; // Current score of Team 1
int team2Score = 0; // Current score of Team 2
const int winningScore = 7; // Points required to win the game
bool gameActive = false; // Tracks whether the game is active or paused
int currentCategory = 0; // Index of the currently selected category
char currentWord[16]; // Current word to guess, max 16 characters

// Categories and words (PROGMEM to save memory)
const char category1[] PROGMEM = "Animals"; // Category name: Animals
const char category2[] PROGMEM = "Movies"; // Category name: Movies
const char category3[] PROGMEM = "Food"; // Category name: Food
const char *const categories[] PROGMEM = {category1, category2, category3}; // List of all categories

const char *const wordsAnimals[] PROGMEM = {"Lion", "Elephant", "Tiger", "Bear", "Giraffe"}; // Words in Animals category
const char *const wordsMovies[] PROGMEM = {"Inception", "Titanic", "Avatar", "Jaws", "Gladiator"}; // Words in Movies category
const char *const wordsFood[] PROGMEM = {"Pizza", "Burger", "Sushi", "Pasta", "Salad"}; // Words in Food category

const char **const wordLists[] PROGMEM = {wordsAnimals, wordsMovies, wordsFood}; // List of all word arrays
const int wordCounts[] = {5, 5, 5}; // Number of words in each category

// Helper function to get a random word from a category
void getRandomWord(int categoryIndex, char *buffer) {
  const char *const *wordList = (const char *const *)pgm_read_ptr(&wordLists[categoryIndex]); // Get the word list for the selected category
  int wordCount = wordCounts[categoryIndex]; // Get the number of words in the category
  const char *word = (const char *)pgm_read_ptr(&wordList[random(wordCount)]); // Select a random word
  strcpy_P(buffer, word); // Copy the word into the provided buffer
}

// Sounds
void playCountdownSound() {
  // Plays a series of tones descending in pitch for the countdown
  for (int i = 1000; i > 200; i -= 100) {
    tone(speakerPin, i, 100); // Generate tone at specified frequency
    delay(100); // Wait between tones
  }
}

void playWinnerSound() {
  // Plays a celebratory sound sequence
  for (int i = 0; i < 3; i++) {
    tone(speakerPin, 1000, 200); // First tone
    delay(300); // Pause
    tone(speakerPin, 1500, 200); // Second tone
    delay(300); // Pause
  }
}

// Display functions
void displayScores() {
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0); // Move to the first line
  lcd.print("T1:"); // Display Team 1 label
  lcd.print(team1Score); // Display Team 1 score
  lcd.print(" T2:"); // Display Team 2 label
  lcd.print(team2Score); // Display Team 2 score
  lcd.setCursor(0, 1); // Move to the second line
  lcd.print(currentWord); // Display the current word
}

void displayWinner(int team) {
  lcd.clear(); // Clear the LCD screen
  lcd.setCursor(0, 0); // Move to the first line
  lcd.print("Team "); // Display "Team "
  lcd.print(team); // Display the winning team number
  lcd.print(" Wins!"); // Display " Wins!"
}

// Button handling
void handleButtons() {
  if (digitalRead(buttonGoStop) == LOW) {
    delay(200); // Debounce the button press
    gameActive = !gameActive; // Toggle game state
    if (gameActive) {
      getRandomWord(currentCategory, currentWord); // Get the first word for the game
      displayScores(); // Show the initial scores and word
    } else {
      lcd.clear(); // Clear the LCD
      lcd.print("Game Paused"); // Indicate the game is paused
    }
  }

  if (gameActive) {
    if (digitalRead(buttonNextWord) == LOW) {
      delay(200); // Debounce the button press
      getRandomWord(currentCategory, currentWord); // Get the next word
      displayScores(); // Update the display
    }

    if (digitalRead(buttonTeam1Point) == LOW) {
      delay(200); // Debounce the button press
      team1Score++; // Increment Team 1 score
      if (team1Score >= winningScore) {
        playWinnerSound(); // Play winner sound
        displayWinner(1); // Display Team 1 as the winner
        resetGame(); // Reset the game
      } else {
        displayScores(); // Update the display
      }
    }

    if (digitalRead(buttonTeam2Point) == LOW) {
      delay(200); // Debounce the button press
      team2Score++; // Increment Team 2 score
      if (team2Score >= winningScore) {
        playWinnerSound(); // Play winner sound
        displayWinner(2); // Display Team 2 as the winner
        resetGame(); // Reset the game
      } else {
        displayScores(); // Update the display
      }
    }
  }
}

// Reset game
void resetGame() {
  team1Score = 0; // Reset Team 1 score
  team2Score = 0; // Reset Team 2 score
  gameActive = false; // Set game state to inactive
}

void setup() {
  // Set up LCD
  lcd.begin(16, 2); // Initialize LCD with dimensions 16x2 (adjust if needed)

  // Set up pins
  pinMode(buttonGoStop, INPUT_PULLUP); // Configure Go/Stop button as input with pull-up resistor
  pinMode(buttonNextWord, INPUT_PULLUP); // Configure Next Word button as input with pull-up resistor
  pinMode(buttonTeam1Point, INPUT_PULLUP); // Configure Team 1 Point button as input with pull-up resistor
  pinMode(buttonTeam2Point, INPUT_PULLUP); // Configure Team 2 Point button as input with pull-up resistor
  pinMode(speakerPin, OUTPUT); // Configure speaker pin as output

  // Seed random number generator
  randomSeed(analogRead(A0)); // Use analog pin for random seed

  // Welcome message
  lcd.print("CatchPhrase!"); // Display the game title
  delay(2000); // Wait for 2 seconds
  lcd.clear(); // Clear the LCD
  lcd.print("Select Category:"); // Prompt user to select a category

  // Display categories
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(0, 1); // Move to the second line
    lcd.print(i + 1); // Display category number
    lcd.print("."); // Display separator
    lcd.print((const char *)pgm_read_ptr(&categories[i])); // Display category name
    delay(2000); // Wait for 2 seconds before showing the next category
    lcd.clear(); // Clear the LCD
    lcd.print("Select Category:"); // Re-prompt user
  }

  // Default category
  currentCategory = 0; // Set default category to the first one
}

void loop() {
  handleButtons(); // Continuously check button inputs and update game state
}
