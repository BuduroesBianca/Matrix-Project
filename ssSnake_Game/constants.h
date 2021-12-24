//------------- PINS --------------

#define PIN_SW 2
#define PIN_X A0
#define PIN_Y A1
#define BUZZER_PIN 3
#define CONTRAST_PIN 6
#define BRIGHTNESS_PIN 10
#define DIN_PIN 12
#define CLOCK_PIN 11
#define LOAD_PIN 8
#define RS 13
#define ENABLE 9
#define D4 5
#define D5 4
#define D6 1
#define D7 7

//---------------- CONSTANTS -----------------

#define NUMBERS_OF_MAX7219 1
#define MATRIX_SIZE 8
#define FIRST_LINE_SUBMENU 0
#define LAST_LINE_SUBMENU_HIGH_SCORES 4
#define SPACING 2
#define SPACING1 1
#define DELAY_SAVED 1000
#define POS_FIRST_LETTER 9
#define POS_SECOND_LETTER 10
#define POS_THIRD_LETTER 11
#define POS_4_LETTER 12
#define POS_5_LETTER 13
#define POS_6_LETTER 14
#define POS_7_LETTER 15
#define NEXT_BUTTON 1
#define SECOND_LINE_SUBMENU 1
#define THIRD_LINE_SUBMENU 2
#define CURSOR_GITHUB 7
#define BACK_BUTTON_ABOUT 7
#define BACK_BUTTON_HIGH_SCORES 3
#define FIRST_LINE_MENU 0
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
#define MAX_XONTRAST_VALUE 150
#define MIN_CONTRAST_VALUE 40
#define CONTRAST_RATIO 10
#define LEN_DIFICULTY_LIST 3
#define MAX_BRIGHTNESS_VALUE 255
#define MIN_BRIGHTNESS_VALUE 30
#define BRIGHTNESS_RATIO 30
#define MAX_INTENSITY_VALUE 6
#define MIN_INTENSITY_VALUE 1
#define INTENSITY_RATIO 1
#define OFF 0
#define ON 1
#define LAST_LINE_MENU 7
#define NEXT_MENU_PAGE 2
#define DELAY_END_OF_GAME 25
#define POS_DIFICULTY_VALUE 11
#define DELAY_LOOP 150
#define TIME_SOUND_GAME 500
#define TIME_SOUND_MENU 100
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
#define DELAY_MESSAGES 8
#define POS_SCORE 7
#define LEN_MENU
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4
#define MOVE_INTERVAL 400
#define INTERVAL_BLINK 500
#define LEN_HIGH_SCORES 3
#define LEN_SET_NAME 1
#define SOUND_RATIO 1

//---------- BUZZER -----------

#define BUZZER_TONE 1000
#define BUZZER_TONE_FOOD 31
#define BUZZER_TONE_MENU 1000
#define BUZZER_TONE_NAME 1000

//---------- EEPROM -----------

#define ADRESS_CONTRAST 0
#define ADDRESS_BRIGHTNESS 1
#define ADDRESS_INTENSITY 2 
#define ADDRESS_DIFICULTY 3
#define ADDRESS_SOUND 4
#define ADDRESS_NAME1 20
#define ADDRESS_NAME2 30
#define ADDRESS_NAME3 40
#define ADDRESS_SCORE1 50
#define ADDRESS_SCORE2 51
#define ADDRESS_SCORE3 52



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

int matrix[MATRIX_SIZE][MATRIX_SIZE] = {
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

int indexAbout = 0;

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
bool joyMoved2 = false;

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

int posDificulty = 0;

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

