//------------- PINS --------------

#define pinSW 2
#define pinX A0
#define pinY A1
#define pinV0 6
#define buzzerPin 3
#define contrastPin 6
#define brightnessPin 10
#define dinPin 12
#define clockPin 11
#define loadPin 8
#define matrixSize 8
#define rows 8
#define cols 8

#define POS_ABOUT 2
#define POS_HIGHSCORES 1
#define SET_CONTRAST 4
#define SET_BRIGHTNESS 5
#define SET_INTENSITY 6
#define SET_DIFICULTY 7
#define SET_SOUND 3
#define POS_START_GAME 0
#define DELAY_EFFECTS 3000
#define SPEED_RATIO_MEDIUM 25

//---------MAGIC NUMBERS-----------

#define LCD_COLUMNS 16
#define LCD_ROWS 2
#define START_POS_BEGIN_MESSAGE 6
#define POS_ARROW1 7
#define POS_ARROW2 8
#define ROWS_ON_LCD 2
#define RESTART_FOOD_VALUE -1
#define STATIONARY 0
#define EASY_RATIO 1
#define MEDIUM_RATIO 2
#define HARD_RATIO 3
#define MAX_SCORE_EASY 64
#define MAX_SCORE_MEDIUM 128
#define MAX_SCORE_HARD 192
#define INITIAL_SNAKE_SPEED 1
#define INITIAL_SNAKE_LENGTH 3
//#define RESET_SCORE 0
#define LEN_SNAKE_MEDIUM 40
#define LEN_SNAKE_EASY 24
#define LEN_SNAKE_HARD 24
#define LEN_SNAKE_MESSAGE 40
#define MIN_THRESHOLD 400
#define MAX_THRESHOLD 600
#define LEN_NAME 7
#define DEBOUNCE_INTERVAL 250
#define LEN_ABOUT 8
#define SCROLL_INTERVAL 300
#define POS_GITHUB 4
#define POS_LINK_GITHUB 5
#define POS_ARROW_GITHUB 10
#define MAX_SPEED_VALUE 900

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define MOVE_INTERVAL 400
#define INTERVAL_BLINK 500

//---------- BUZZER -----------

#define BUZZER_TONE 1000
#define BUZZER_TONE_FOOD 31
#define BUZZER_TONE_MENU 1000
#define BUZZER_TONE_NAME 1000

//---------- EEPROM -----------

#define ADRESS_CONTRAST 0
#define ADDRESS_BRIGHTNESS 1
#define ADDRESS_INTENSITY 2 
#define adressDificulty 3
#define adressSound 4
#define addressName1 20
#define addressName2 30
#define addressName3 40
#define addressScore1 50
#define addressScore2 51
#define addressScore3 52
#define addressHighScore 53

bool joyMoved2 = false;
int posDificulty = 0;

//-------- States -------------

bool highScoreState = false;
bool State = false;
bool lostGameState = false;
bool nextHighScore = false;
bool okSetName = false;
bool gameState = false;
bool pushedOk = false;
bool winGameState = false;
bool pushedWinGame = false;
bool setNameState = false;


bool editContrast = false;
bool editBrightness = false;
bool editIntensity = false;
bool editDificulty = false;
bool editSound = false;


//----Matrix ------------

int xPos = 3;
int yPos = 3;

int matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};

//-------------Game -------------

int snakeSpeed = 1;

byte snakeDirection = 0;

int i = 0;

int snakeLength = 3;

bool foodBlink = false;
bool pushedBackHighScore = false;

int foodX = -1;
int foodY = -1;

bool winGame = false;
bool lostGame = false;

byte score = 0;
byte highScore = 0;

unsigned long lastMoved = 0;

bool joyMovedX = false;
bool joyMovedY = false;

unsigned long startTime = 0;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

//---------Joystick------------

int xValue = 0;
int yValue = 0;


bool joyMoved = false;
bool joyMoved1 = false;

//--------Set Name---------------

bool switchLetter = true;
bool switchLetter1 = false;
bool switchLetter2 = false;
bool switchLetter3 = false;
bool switchLetter4 = false;
bool switchLetter5 = false;
bool switchLetter6 = false;
bool switchLetter7 = false;

char letter = 'a';
char letterLimit = 'z';
 
char nameLetters[LEN_NAME] = {
  '-', '-', '-', '-', '-', '-', '-'
};

byte pozY = 9;
byte posLetter = 0;

String namePlayer = "";
String namePlayer1 = "";
String namePlayer2 = "";
String namePlayer3 = "";

int scorePlayer1 = 0;
int scorePlayer2 = 0;
int scorePlayer3 = 0;

bool pushedBackAbout = false;
bool clearHelper = true;

//------------Menu-----------------

bool switchMenu = false;

int posMenu = -2;
int posSubmenu = 0;

bool enteredSubmenu = false;
bool pushedButton = false;

bool gameStarted = false;
bool gameStartButton = false;

//-----------EEPROM------------


byte lastContrastValue = 75;
byte contrastValue = 75;

byte brightnessValue = 100;
byte lastBrightnessValue = 100;

byte intensityValue = 10;
byte lastIntensityValue = 10;

int soundValue = 1;
byte lastSoundValue = 1;

String dificultyValue = "Easy";
String lastDificultyValue = "";

byte scoreRatio = 1;

int easySpeed = 900;
int hardSpeed = 600;
int speedValue = 900;

String dificultyValues[3] = {
  "Easy", "Med", "Hard"
};


//-------------Custom chars-----------

byte arrow[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
};


byte arrowUp[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
};


unsigned long currentAMillis = 0;
