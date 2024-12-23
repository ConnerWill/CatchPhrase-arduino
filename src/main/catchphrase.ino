#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

// Pin assignments
const int buttonGoStop = 2;
const int buttonNextWord = 3;
const int buttonTeam1Point = 4;
const int buttonTeam2Point = 5;
const int speakerPin = 6;

// Initialize LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Game variables
int team1Score = 0;
int team2Score = 0;
const int winningScore = 7;
bool gameActive = false;
int currentCategory = 0;
char currentWord[16]; // Current word to guess

// Categories and words (PROGMEM to save memory)
const char category1[] PROGMEM = "Animals";
const char category2[] PROGMEM = "Movies";
const char category3[] PROGMEM = "Food";
const char *const categories[] PROGMEM = {category1, category2, category3};

const char *const wordsAnimals[] PROGMEM = {"Lion", "Elephant", "Tiger", "Bear", "Giraffe"};
const char *const wordsMovies[] PROGMEM = {"Inception", "Titanic", "Avatar", "Jaws", "Gladiator"};
const char *const wordsFood[] PROGMEM = {"Pizza", "Burger", "Sushi", "Pasta", "Salad"};

const char **const wordLists[] PROGMEM = {wordsAnimals, wordsMovies, wordsFood};
const int wordCounts[] = {5, 5, 5}; // Number of words in each category

// Helper function to get a random word from a category
void getRandomWord(int categoryIndex, char *buffer) {
  const char *const *wordList = (const char *const *)pgm_read_ptr(&wordLists[categoryIndex]);
  int wordCount = wordCounts[categoryIndex];
  const char *word = (const char *)pgm_read_ptr(&wordList[random(wordCount)]);
  strcpy_P(buffer, word);
}

// Sounds
void playCountdownSound() {
  for (int i = 1000; i > 200; i -= 100) {
    tone(speakerPin, i, 100);
    delay(100);
  }
}

void playWinnerSound() {
  for (int i = 0; i < 3; i++) {
    tone(speakerPin, 1000, 200);
    delay(300);
    tone(speakerPin, 1500, 200);
    delay(300);
  }
}

// Display functions
void displayScores() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T1:");
  lcd.print(team1Score);
  lcd.print(" T2:");
  lcd.print(team2Score);
  lcd.setCursor(0, 1);
  lcd.print(currentWord);
}

void displayWinner(int team) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Team ");
  lcd.print(team);
  lcd.print(" Wins!");
}

// Button handling
void handleButtons() {
  if (digitalRead(buttonGoStop) == LOW) {
    delay(200); // Debounce
    gameActive = !gameActive;
    if (gameActive) {
      getRandomWord(currentCategory, currentWord);
      displayScores();
    } else {
      lcd.clear();
      lcd.print("Game Paused");
    }
  }

  if (gameActive) {
    if (digitalRead(buttonNextWord) == LOW) {
      delay(200); // Debounce
      getRandomWord(currentCategory, currentWord);
      displayScores();
    }

    if (digitalRead(buttonTeam1Point) == LOW) {
      delay(200); // Debounce
      team1Score++;
      if (team1Score >= winningScore) {
        playWinnerSound();
        displayWinner(1);
        resetGame();
      } else {
        displayScores();
      }
    }

    if (digitalRead(buttonTeam2Point) == LOW) {
      delay(200); // Debounce
      team2Score++;
      if (team2Score >= winningScore) {
        playWinnerSound();
        displayWinner(2);
        resetGame();
      } else {
        displayScores();
      }
    }
  }
}

// Reset game
void resetGame() {
  team1Score = 0;
  team2Score = 0;
  gameActive = false;
}

void setup() {
  // Set up LCD
  lcd.begin(16, 2); // Adjust for your LCD dimensions

  // Set up pins
  pinMode(buttonGoStop, INPUT_PULLUP);
  pinMode(buttonNextWord, INPUT_PULLUP);
  pinMode(buttonTeam1Point, INPUT_PULLUP);
  pinMode(buttonTeam2Point, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);

  // Seed random number generator
  randomSeed(analogRead(A0));

  // Welcome message
  lcd.print("CatchPhrase!");
  delay(2000);
  lcd.clear();
  lcd.print("Select Category:");

  // Display categories
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(0, 1);
    lcd.print(i + 1);
    lcd.print(".");
    lcd.print((const char *)pgm_read_ptr(&categories[i]));
    delay(2000);
    lcd.clear();
    lcd.print("Select Category:");
  }

  // Default category
  currentCategory = 0;
}

void loop() {
  handleButtons();
}
