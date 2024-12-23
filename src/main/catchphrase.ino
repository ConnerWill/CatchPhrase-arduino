#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

// Pin assignments
const int buttonGoStop = 2;     // Pin for the Go/Stop button
const int buttonNextWord = 3;   // Pin for the Next Word button
const int buttonTeam1Point = 4; // Pin for Team 1 Point button
const int buttonTeam2Point = 5; // Pin for Team 2 Point button
const int speakerPin = 6;       // Pin for the speaker

// Initialize LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Connects the LCD to these specific pins

// Game variables
int team1Score = 0;         // Current score of Team 1
int team2Score = 0;         // Current score of Team 2
const int winningScore = 7; // Points required to win the game
bool gameActive = false;    // Tracks whether the game is active or paused
int currentCategory = 0;    // Index of the currently selected category
char currentWord[16];       // Current word to guess, max 16 characters

// Timer variables
const int roundTime = 30; // Round duration in seconds
int timer = roundTime;    // Initialize timer with round time

// Categories and words (PROGMEM to save memory)
const char category1[] PROGMEM  = "Animals";
const char category2[] PROGMEM  = "Movies";
const char category3[] PROGMEM  = "Shows";
const char category4[] PROGMEM  = "Food";
const char category5[] PROGMEM  = "Conner";
const char category6[] PROGMEM  = "Sports";
const char category7[] PROGMEM  = "Music";
const char category8[] PROGMEM  = "Famous People";
const char category9[] PROGMEM  = "Places";
const char category10[] PROGMEM = "Every Day Life";
const char category11[] PROGMEM = "Phrases";
const char category12[] PROGMEM = "Technology";
const char category13[] PROGMEM = "Historical Events";
const char category14[] PROGMEM = "Vehicles";
const char category15[] PROGMEM = "Mythology";
const char category16[] PROGMEM = "Nature";
const char category17[] PROGMEM = "Space";

// Update categories and word lists array
const char *const categories[] PROGMEM = {
  category1, category2, category3, category4, category5,
  category6, category7, category8, category9, category10,
  category11, category12, category13, category14, category15,
  category16, category17
};

const char* const wordsAnimals[] PROGMEM = {
    "Lion", "Elephant", "Tiger", "Bear", "Giraffe", "Zebra", "Kangaroo", "Shark", "Whale", "Rabbit",
    "Penguin", "Koala", "Dolphin", "Otter", "Cheetah", "Buffalo", "Rhino", "Panda", "Jaguar", "Lynx",
    "Wolf", "Fox", "Coyote", "Leopard", "Lemur", "Baboon", "Chimpanzee", "Gorilla", "Orangutan", 
    "Sloth", "Hyena", "Meerkat", "TasmanianDevil", "Wolverine", "Badger", "Raccoon", "Squirrel", 
    "Bison", "Moose", "Caribou", "Anteater", "Porcupine", "Armadillo", "Capybara", "Otterhound", 
    "Marmot", "Weasel", "Mink", "Opossum", "Eagle", "Falcon", "Hawk", "Vulture", "Condor", "Parrot", 
    "Toucan", "Crow", "Raven", "Swan", "Heron", "Pelican", "Albatross", "Seagull", "Goose", "Pigeon", 
    "Peacock", "Flamingo", "Penguin", "Hummingbird", "Woodpecker", "Bat", "Dragonfly", "Moth", "Butterfly", 
    "Bee", "Ant", "Termite", "Ladybug", "Grasshopper", "Cricket", "Beetle", "Cockroach", "Caterpillar", 
    "Worm", "Earthworm", "Leech", "Snail", "Slug", "Scorpion", "Centipede", "Millipede", "Starfish", 
    "Octopus", "Squid", "Jellyfish", "Clam", "Mussel", "Shrimp", "Lobster", "Crab", "Oyster", "Cuttlefish", 
    "Ray", "Turtle", "Crocodile", "Alligator", "Iguana", "Chameleon", "Gecko", "Anole", "MonitorLizard", 
    "Viper", "Python", "Boa", "Rattlesnake", "KingSnake", "CoralSnake", "Adder", "GilaMonster", 
    "KomodoDragon", "Mongoose", "Camel", "Donkey", "Horse", "Sheep", "Goat", "Pig", "Cow", "Chicken", 
    "Duck", "Turkey", "Goose", "Peafowl", "Emu", "Ostrich", "Alpaca", "Llama", "Bison", "Yak", "Reindeer", 
    "Elk", "Gazelle", "Impala", "Springbok", "Antelope", "Ibex", "Kudu", "Eland", "Bongo", "Wildebeest", 
    "Okapi", "KangarooRat", "Wallaby", "Quokka", "Bandicoot", "SugarGlider", "Numbat", "KoalaBear", 
    "TasmanianTiger", "Fossa", "Pangolin", "Marmot", "Platypus", "Wombat", "Tarantula", "Cicada", 
    "Lynx", "Caracal", "Serval", "Cougar", "MountainLion", "WildCat", "Jaguarundi", "JaguarX", "Puma"
};

const char* const wordsMovies[] PROGMEM = {
    "Inception", "Titanic", "Avatar", "Jaws", "Gladiator", "Interstellar", "Matrix", "Batman", "Rocky", "Jumanji",
    "StarWars", "ToyStory", "Frozen", "Up", "TheShining", "Memento", "Goodfellas", "Casablanca", "Shrek", "Alien",
    "TheGodfather", "PulpFiction", "TheDarkKnight", "FightClub", "ForrestGump", "TheMatrix", "JurassicPark", 
    "TheAvengers", "TheLionKing", "SchindlersList", "TheTerminator", "TheWizardOfOz", "GoneWithTheWind", 
    "StarTrek", "TheSocialNetwork", "TheShawshankRedemption", "BladeRunner", "TheSilenceOfTheLambs", "12AngryMen", 
    "DieHard", "BackToTheFuture", "Casablanca", "ThePrincessBride", "TheHungerGames", "MadMax", "PulpFiction", 
    "E.T.", "TheNotebook", "RockyHorror", "Léon", "LaLaLand", "AClockworkOrange", "TheExorcist", "TheBigLebowski", 
    "TheGoonies", "IngloriousBasterds", "TheGreatEscape", "TheUsualSuspects", "DrStrangelove", "Gladiator", 
    "TheSoundOfMusic", "GoodMorningVietnam", "TheGreatestShowman", "SpiderMan", "IndianaJones", "PiratesOfTheCaribbean", 
    "TheHobbit", "ToyStory2", "TheGiver", "FindingNemo", "WALL-E", "Zootopia", "Interstellar", "DjangoUnchained", 
    "Moonlight", "Her", "TheDarkKnightRises", "GuardiansOfTheGalaxy", "JurassicWorld", "AvengersEndgame", 
    "TheIncredibles", "BlackPanther", "IronMan", "CaptainAmerica", "Thor", "Deadpool", "MadMaxFuryRoad", 
    "Gladiator", "ShutterIsland", "TheDeparted", "TheHungerGames", "PacificRim", "TheMartian", "TheRevenant", 
    "ThePrincessDiaries", "AStarIsBorn", "Frozen2", "It", "LaLaLand", "KnivesOut", "Shazam", "JumanjiTheNextLevel",
    "DoctorStrange", "GuardiansOfTheGalaxyVol2", "BladeRunner2049", "WonderWoman", "Logan", "FantasticBeasts", 
    "Kingsman", "Ghostbusters", "TheBigShort", "FuryRoad", "Parasite", "TheWolfOfWallStreet", "TheMatrixReloaded", 
    "IronMan2", "Coco", "Frozen", "AvatarTheWayOfWater", "Tenet", "TheIrrationalMan", "TheKiller", "ABeautifulMind"
};

const char* const wordsShows[] PROGMEM = {
    "Seinfeld", "Friends", "Lost", "Stranger", "Simpsons", "Scrubs", "Dexter", "Mandalorian", "Suits", "BigBang",
    "BreakingBad", "Narcos", "Westworld", "GOT", "Castle", "Office", "PeakyBlinders", "Vikings", "TheCrown", "Fargo",
    "TheWalkingDead", "BlackMirror", "BetterCallSaul", "TheOfficeUS", "HowIMetYourMother", "TheExpanse", "Sherlock",
    "Chernobyl", "MoneyHeist", "Mindhunter", "TheWitcher", "StrangerThings", "TheHandmaidsTale", "TrueDetective", 
    "TheSopranos", "HouseOfCards", "NarcosMexico", "Arrow", "TheFlash", "Supernatural", "Lucifer", "OrangeIsTheNewBlack",
    "TheBoys", "Ozark", "ThePunisher", "BrooklynNineNine", "Community", "RickAndMorty", "MarvelsDaredevil", 
    "TheGoodPlace", "TheUmbrellaAcademy", "BoJackHorseman", "JackRyan", "CastleRock", "TheNightManager", "13ReasonsWhy", 
    "Hannibal", "BetterThings", "FridayNightLights", "GilmoreGirls", "NewGirl", "LostInSpace", "TheMarvelousMrsMaisel",
    "TheFlash", "Supergirl", "Fleabag", "Narcos", "DoctorWho", "AHS", "LovecraftCountry", "TheChillingAdventuresOfSabrina",
    "UnbreakableKimmySchmidt", "TheFosters", "TheMandalorian", "Broadchurch", "StrangerThings2", "Hannibal", "TheBlacklist",
    "TheGoodDoctor", "TheChernobylChronicles", "TheRightStuff", "SiliconValley", "WestWing", "TheStrain", 
    "RaisingDion", "Euphoria", "TheCloneWars", "BigLittleLies", "ThePacific", "Superstore", "TheNightOf", 
    "QueenOfTheSouth", "TedLasso", "AfterLife", "KillingEve", "PennyDreadful", "NarcosColombia", "TheFall", "BetterCallSaul", 
    "ThisIsUs", "TheLeftovers", "TheO.C.", "TheLyingGame", "Shameless", "Supernatural", "TheXFiles", "TheLastOfUs"
};

const char* const wordsFood[] PROGMEM = {
    "Pizza", "Burger", "Sushi", "Pasta", "Salad", "Tacos", "Burritos", "Steak", "Pancakes", "IceCream",
    "SushiRoll", "Curry", "Risotto", "Noodles", "Chicken", "Stew", "Lasagna", "Waffles", "FriedRice", "Fajitas",
    "HotDog", "Ramen", "Spaghetti", "GrilledCheese", "Poutine", "Gyro", "Falafel", "Sandwich", "Kebab", "Moussaka",
    "DimSum", "Paella", "Quiche", "Tiramisu", "Cheesecake", "Muffin", "Brownie", "Croissant", "Bagel", "Ceviche",
    "Clams", "Oysters", "Churros", "Samosa", "PorkBelly", "Shawarma", "BanhMi", "Goulash", "Chili", "CobbSalad",
    "Casserole", "PotPie", "Pho", "SpringRolls", "Kimchi", "ZucchiniFritters", "Focaccia", "Scones", "CheeseCurds",
    "Churro", "LobsterRoll", "PastaSalad", "BakedPotato", "Nachos", "SalisburySteak", "CabbageRolls", "Tempura", 
    "Frittata", "Gumbo", "BangersAndMash", "RisottoBalls", "BeefStew", "SteakFrites", "Tempura", "PekingDuck",
    "FriedChicken", "DuckBreast", "Crepes", "Baklava", "PizzaRolls", "Ravioli", "Tamales", "Empanadas", "PulledPork",
    "CurryWurst", "TunaMelt", "Gravy", "Bangers", "Meatballs", "FriedEgg", "Popcorn", "CinnamonRoll", "EggRolls", 
    "ChiliConCarne", "TofuStirFry", "BBQRibs", "CinnamonToast", "PastaCarbonara", "BakedZiti", "ChickenWings", "Risotto"
};


const char* const wordsConner[] PROGMEM = {
    "Conner", "Lucas", "Sarah", "Ethan", "Will", "Olivia", "Jackson", "Sophia", "Madison", "Noah", 
    "Ava", "Isabella", "Mason", "Liam", "Ella", "Grace", "James", "Zoe", "Mia", "Benjamin"
};

const char* const wordsSports[] PROGMEM = {
    "Soccer", "Basketball", "Tennis", "Football", "Baseball", "Cricket", "Rugby", "Golf", "Boxing", "Hockey",
    "Lacrosse", "Swimming", "Volleyball", "Cycling", "Skiing", "Formula1", "Badminton", "Rowing", "Wrestling", "Gymnastics"
};

const char* const wordsMusic[] PROGMEM = {
    "Guitar", "Piano", "Drums", "Rock", "Jazz", "Pop", "Classical", "Blues", "HipHop", "Reggae", 
    "Violin", "Flute", "Saxophone", "Cello", "Bass", "Trumpet", "Opera", "EDM", "Country", "Folk"
};

const char* const wordsFamousPeople[] PROGMEM = {
    "Einstein", "Shakespeare", "Tesla", "Obama", "Gandhi", "Newton", "Lincoln", "Curie", "Churchill", "Mandela", 
    "Einstein", "Bach", "DaVinci", "Napoleon", "Churchill", "Cervantes", "MarieCurie", "Franklin", "Wright", "Mandela"
};

const char* const wordsPlaces[] PROGMEM = {
    "Paris", "London", "Tokyo", "Sydney", "Rome", "Berlin", "Moscow", "Dubai", "Miami", "Lagos", 
    "Cairo", "Athens", "Istanbul", "Barcelona", "Berlin", "Madrid", "Beijing", "NewYork", "LosAngeles", "Bangkok"
};

const char* const wordsEveryDayLife[] PROGMEM = {
    "Laundry", "Coffee", "Work", "Exercise", "Sleep", "Travel", "Cleaning", "Cooking", "Dishes", "Shopping", 
    "Reading", "Walking", "Writing", "Bathing", "Relaxing", "Brushing", "Eating", "Studying", "Running", "Sleeping"
};

const char* const wordsPhrases[] PROGMEM = {
    "Break a leg", "Piece of cake", "Under the weather", "Once in a blue moon", "Bite the bullet", "Hit the sack", 
    "Spill the beans", "Let the cat out", "Cut to the chase", "Burning the midnight oil", "Jump the gun", 
    "On cloud nine", "Call it a day", "Hit the nail", "Off the hook", "On the ball", "Behind the scenes", 
    "In the same boat", "The ball is in your court", "Under your nose"
};

const char* const wordsTechnology[] PROGMEM = {
    "Computer", "Laptop", "Smartphone", "Cloud", "Blockchain", "AI", "Software", "Robots", "VR", "AR", 
    "Internet", "Data", "Website", "App", "Database", "Server", "Firewall", "Security", "Bluetooth", "IoT"
};

const char* const wordsHistoricalEvents[] PROGMEM = {
    "WWII", "Moon Landing", "Renaissance", "Cold War", "Civil War", "Revolution", "American Rev", "Berlin Wall", "9/11", 
    "Titanic", "French Rev", "Hiroshima", "Vikings", "Crusades", "GreatDepression", "Fall of Berlin", "Apollo11", "Gandhi", "WWI"
};

const char* const wordsVehicles[] PROGMEM = {
    "Car", "Truck", "Bicycle", "Motorcycle", "Helicopter", "Bus", "Train", "Plane", "Boat", "Submarine", 
    "Ship", "Jet", "Rocket", "Scooter", "Tank", "Tractor", "Ambulance", "Taxi", "Tram", "Van"
};

const char* const wordsMythology[] PROGMEM = {
    "Zeus", "Hercules", "Medusa", "Apollo", "Thor", "Athena", "Poseidon", "Hades", "Ares", "Persephone", 
    "Ra", "Osiris", "Bastet", "Anubis", "Hermes", "Freya", "Odin", "Loki", "Boreas", "Prometheus"
};

const char* const wordsNature[] PROGMEM = {
    "Mountain", "River", "Forest", "Desert", "Ocean", "Lake", "Cliff", "Waterfall", "Valley", "Cave", 
    "Tundra", "Marsh", "Swamp", "Meadow", "Riverbed", "Island", "Coral", "Rainforest", "Glacier", "Creek"
};

const char* const wordsSpace[] PROGMEM = {
    "Planet", "Galaxy", "Moon", "Comet", "Rocket", "Star", "Mars", "Nebula", "Black Hole", "Asteroid", 
    "Meteor", "Saturn", "Venus", "Jupiter", "Uranus", "Saturn", "Pluto", "Orbit", "Exoplanet", "Supernova"
};

const char **const wordLists[] PROGMEM = {
  wordsAnimals, wordsMovies, wordsShows, wordsFood, wordsConner,
  wordsSports, wordsMusic, wordsFamousPeople, wordsPlaces, wordsInstruments,
  wordsSuperheroes, wordsTechnology, wordsHistoricalEvents, wordsVehicles, wordsMythology,
  wordsNature, wordsSpace
};

// Calculate number of words in each category
const int wordCounts[] = {
  sizeof(wordsAnimals)          / sizeof(wordsAnimals[0]),
  sizeof(wordsMovies)           / sizeof(wordsMovies[0]),
  sizeof(wordsShows)            / sizeof(wordsShows[0]),
  sizeof(wordsFood)             / sizeof(wordsFood[0]),
  sizeof(wordsConner)           / sizeof(wordsConner[0]),
  sizeof(wordsSports)           / sizeof(wordsSports[0]),
  sizeof(wordsMusic)            / sizeof(wordsMusic[0]),
  sizeof(wordsFamousPeople)     / sizeof(wordsFamousPeople[0]),
  sizeof(wordsPlaces)           / sizeof(wordsPlaces[0]),
  sizeof(wordsEveryDayLife)     / sizeof(wordsEveryDayLife[0]),
  sizeof(wordsPhrases)          / sizeof(wordsPhrases[0]),
  sizeof(wordsTechnology)       / sizeof(wordsTechnology[0]),
  sizeof(wordsHistoricalEvents) / sizeof(wordsHistoricalEvents[0]),
  sizeof(wordsVehicles)         / sizeof(wordsVehicles[0]),
  sizeof(wordsMythology)        / sizeof(wordsMythology[0]),
  sizeof(wordsNature)           / sizeof(wordsNature[0]),
  sizeof(wordsSpace)            / sizeof(wordsSpace[0]),
};

// Helper function to get a random word from a category
void getRandomWord(int categoryIndex, char *buffer) {
  const char *const *wordList = (const char *const *)pgm_read_ptr(&wordLists[categoryIndex]); // Get the word list for the selected category
  int wordCount = wordCounts[categoryIndex]; // Get the number of words in the category
  const char *word = (const char *)pgm_read_ptr(&wordList[random(wordCount)]); // Select a random word
  strcpy_P(buffer, word); // Copy the word into the provided buffer
}

// Sounds
void playTone(int frequency, int duration) {
  tone(speakerPin, frequency, duration);
}

void playCountdownSound() {
  // Play a short descending tone every second during the countdown
  playTone(1000, 100);
}

void playRoundEndSound() {
  playTone(500, 500); // Play a 500Hz tone for 500ms when the round ends
  delay(100);         // Short delay to separate tones
  playTone(500, 500); // Play a 500Hz tone for 500ms when the round ends
}

void playWinnerSound() {
  for (int i = 0; i < 3; i++) {
    playTone(1000, 200); // First tone (1000 Hz)
    delay(300);          // Short delay between tones
    playTone(1500, 200); // Second tone (1500 Hz)
  }
}

// Display functions
void displayScores() {
  lcd.clear();             // Clear the LCD screen
  lcd.setCursor(0, 0);     // Move to the first line
  lcd.print(F("T1:"));     // Display Team 1 label
  lcd.print(team1Score);   // Display Team 1 score
  lcd.print(F(" T2:"));    // Display Team 2 label
  lcd.print(team2Score);   // Display Team 2 score
  lcd.setCursor(0, 1);     // Move to the second line
  lcd.print(currentWord);  // Display the current word
  lcd.print(F(" Time: ")); // Display timer label
  lcd.print(timer);        // Display remaining time
}

void displayWinner(int team) {
  lcd.clear();            // Clear the LCD screen
  lcd.setCursor(0, 0);    // Move to the first line
  lcd.print(F("Team "));  // Display "Team "
  lcd.print(team);        // Display the winning team number
  lcd.print(F(" Wins!")); // Display " Wins!"
}

// Button handling
void handleButtons() {
  if (digitalRead(buttonGoStop) == LOW) {
    delay(200); // Debounce the button press
    gameActive = !gameActive; // Toggle game state
    if (gameActive) {
      getRandomWord(currentCategory, currentWord); // Get the first word for the game
      displayScores(); // Show the initial scores and word
      timer = roundTime; // Reset timer
    } else {
      lcd.clear(); // Clear the LCD
      lcd.print(F("Game Paused")); // Indicate the game is paused
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
        playWinnerSound(); // Play winner sound for Team 1
        displayWinner(1);  // Display Team 1 as the winner
        resetGame(true);   // Reset game
      } else {
        displayScores(); // Update the display
      }
    }

    if (digitalRead(buttonTeam2Point) == LOW) {
      delay(200); // Debounce the button press
      team2Score++; // Increment Team 2 score
      if (team2Score >= winningScore) {
        playWinnerSound(); // Play winner sound for Team 2
        displayWinner(2);  // Display Team 2 as the winner
        resetGame(true);   // Reset game
      } else {
        displayScores(); // Update the display
      }
    }

    // Countdown timer logic
    if (timer > 0) {
      delay(1000); // Wait for 1 second
      timer--; // Decrement the timer
      playCountdownSound(); // Play countdown sound every second
      displayScores(); // Update the display with remaining time
    } else {
      // Timer expired, end round
      gameActive = false; // Deactivate the game
      playRoundEndSound(); // Play sound when round ends
      lcd.clear(); // Clear the screen
      lcd.print(F("Round Over!")); // Display round over message
      delay(2000); // Wait for 2 seconds
      // Don't reset scores, just set gameActive to false and prepare for the next round
    }
  }
}

// Reset game
void resetGame(bool resetScores) {
  if (resetScores) {
    team1Score = 0;     // Reset Team 1 score
    team2Score = 0;     // Reset Team 2 score
  }
  gameActive = false; // Set game state to inactive
}

void setup() {
  // Set up LCD
  lcd.begin(16, 2); // Initialize LCD with dimensions 16x2 (adjust if needed)

  // Set up pins
  pinMode(buttonGoStop, INPUT_PULLUP);     // Configure Go/Stop button as input with pull-up resistor
  pinMode(buttonNextWord, INPUT_PULLUP);   // Configure Next Word button as input with pull-up resistor
  pinMode(buttonTeam1Point, INPUT_PULLUP); // Configure Team 1 Point button as input with pull-up resistor
  pinMode(buttonTeam2Point, INPUT_PULLUP); // Configure Team 2 Point button as input with pull-up resistor
  pinMode(speakerPin, OUTPUT);             // Configure speaker pin as output

  // Seed random number generator
  randomSeed(analogRead(A0)); // Use analog pin for random seed

  // Welcome message
  lcd.print(F("CATCHPHRASE!"));  // Display "CATCHPHRASE!" message
  delay(2000);                   // Wait for 2 seconds
  lcd.clear();                   // Clear the LCD screen
}

void loop() {
  handleButtons(); // Handle button presses and game logic
}
