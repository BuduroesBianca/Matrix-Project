#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#include "constants.h"
#include <stdlib.h>
#include <time.h>

LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

LedControl lc = LedControl(DIN_PIN, CLOCK_PIN, LOAD_PIN, NUMBERS_OF_MAX7219);

/* 
   Loop(): all of the menu states are handled
   Game(): handles the game and game states
*/

void setup() {

  //initialize saved settings
  contrastValue = EEPROM.read(ADRESS_CONTRAST);
  intensityValue = EEPROM.read(ADDRESS_INTENSITY);
  brightnessValue = EEPROM.read(ADDRESS_BRIGHTNESS);
  soundValue = EEPROM.read(ADDRESS_SOUND);

  //initialize names for highScore list
  namePlayer1 = readNameFromEEPROM(ADDRESS_NAME1);
  namePlayer2 = readNameFromEEPROM(ADDRESS_NAME2);
  namePlayer3 = readNameFromEEPROM(ADDRESS_NAME3);

  scorePlayer1 = EEPROM.read(ADDRESS_SCORE1);
  scorePlayer2 = EEPROM.read(ADDRESS_SCORE2);
  scorePlayer3 = EEPROM.read(ADDRESS_SCORE3);

  analogWrite(CONTRAST_PIN, contrastValue);
  analogWrite(BRIGHTNESS_PIN, brightnessValue);

  lc.shutdown(0, false);
  lc.setIntensity(0, intensityValue);
  lc.clearDisplay(0);

  pinMode(PIN_SW, INPUT_PULLUP);
  pinMode(PIN_X, INPUT);
  pinMode(PIN_Y, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_SW), enterSubmenu, FALLING);

  lcd.clear();
  lcd.createChar(0, arrow);
  lcd.createChar(1, arrowUp);

  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.setCursor(0, 0);

  showSnakeMessage(); // S N A K E

  lcd.clear();
  lcd.setCursor(START_POS_BEGIN_MESSAGE, 0);
  lcd.print("MENU");
  lcd.setCursor(POS_ARROW1, 1);
  lcd.write(byte(0));
  lcd.setCursor(POS_ARROW2, 1);
  lcd.write(byte(0));

  srand((int)time(0));
}

String about[LEN_ABOUT] = {
  "Game: ", "ssSnake 2021", "Creator: ", "Bianca Buduroes", "Github: ", "https://github.com/BuduroesBianca", "Enjoy Playing", "> Back"
};

String content[LEN_MENU] = {
  "> Start Game", "> High Scores", "> About", "> Sound Options", "> Set Contrast", "> Set Brightness", "> Led Intensity", "> Set Dificulty", "> Back"
};

char * link = "github.com/BuduroesBianca   ";

void loop() {

  if (switchMenu == true) {
    enteredSubmenu = true;

    if (posMenu == POS_ABOUT) {
      if (clearHelper) {
        lcd.clear();
        clearHelper = false;
      }
      showAbout();
    }

    else if (posMenu == POS_HIGHSCORES) {
      if (clearHelper) {
        lcd.clear();
        clearHelper = false;
      }
      showHighScores();
    }

    else if (posMenu == SET_CONTRAST) {
      lcd.clear();
      set("Contrast");
    }

    else if (posMenu == SET_BRIGHTNESS) {
      lcd.clear();
      set("Brightness");
    }

    else if (posMenu == SET_INTENSITY) {
      lcd.clear();
      set("Intensity");
    }

    else if (posMenu == SET_DIFICULTY) {
      lcd.clear();
      set("Dificulty");
    }

    else if (posMenu == SET_SOUND) {
      lcd.clear();
      set("Sound");
    }

    if (posMenu == POS_START_GAME && highScoreState == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("> Start Game");
      lcd.setCursor(0, 1);
      lcd.print("> Back");
      yAxisSubmenu(1);
    }

    if (posMenu == POS_START_GAME && highScoreState == true && lostGameState == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HIGH SCORE");
      lcd.setCursor(0, 1);
      lcd.print("> Next");
      yAxisSubmenu(1);
    }

    if (posMenu == POS_START_GAME && lostGameState == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Oh.. You killed");
      lcd.setCursor(0, 1);
      lcd.print("the ssSnake :(");

      delay(DELAY_EFFECTS); // delay at end of the game for effect

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Final Score: " + String(score));
      lcd.setCursor(0, 1);
      lcd.print("Dificulty: " + String(dificultyValue));

      delay(DELAY_EFFECTS); // delay at end of the game for effect

      lcd.clear();
      switchMenu = false;
      enteredSubmenu = false;
      posSubmenu = FIRST_LINE_SUBMENU;
      lostGameState = false;
      posMenu = POS_START_GAME;

    }

    if (posMenu == POS_START_GAME && winGameState == true && lostGameState == false) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Oh WOW..");
      lcd.setCursor(0, 1);
      lcd.print("You win the game");

      delay(DELAY_EFFECTS); // delay at end of the game for effect

      lcd.clear();
      switchMenu = false;
      enteredSubmenu = false;
      posSubmenu = FIRST_LINE_SUBMENU;
      winGameState = false;
      posMenu = POS_START_GAME;

    }

    if (nextHighScore == true) {
      posSubmenu = FIRST_LINE_SUBMENU;
      setNameState = true;
      nextHighScore = false;
      highScoreState = false;
    }

    if (setNameState == true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set name:");
      lcd.setCursor(0, 1);
      lcd.print("> OK");

      setName();

      lcd.clear();
      setNameState = false;
      highScoreState = false;
      switchMenu = false;
      enteredSubmenu = false;
      posSubmenu = FIRST_LINE_SUBMENU;
      pushedOk = false;
      posMenu = FIRST_LINE_MENU;
    }

    if (pushedButton == true && posSubmenu == SECOND_LINE_SUBMENU) {
      lcd.clear();
      switchMenu = false;
      enteredSubmenu = false;
      posSubmenu = FIRST_LINE_SUBMENU;
      pushedButton = false;
      posMenu = FIRST_LINE_MENU;
    }

    else if (pushedBackAbout == true && posSubmenu == BACK_BUTTON_ABOUT) {
      lcd.clear();
      switchMenu = false;
      enteredSubmenu = false;
      posSubmenu = FIRST_LINE_SUBMENU;
      pushedBackAbout = false;
      posMenu = POS_START_GAME;
      indexAbout = 0;
      clearHelper = true;
    }

    else if (pushedBackHighScore == true && posSubmenu == BACK_BUTTON_HIGH_SCORES) {
      lcd.clear();
      switchMenu = false;
      enteredSubmenu = false;
      posSubmenu = FIRST_LINE_SUBMENU;
      pushedBackHighScore = false;
      posMenu = FIRST_LINE_MENU;
      clearHelper = true;
    }
  }

  if (gameStartButton == true && gameState == false) {
    gameState = true;
    game();
    gameStartButton = false;
  }

  else if (enteredSubmenu == false && switchMenu == false) {
    showMenu();
    yAxisMenu();
  }

  delay(DELAY_LOOP); // delay of 150
}

void enterSubmenu() {

  static unsigned long lastDebounceTime = 0;

  if (enteredSubmenu == false) {
    if (millis() - lastDebounceTime > DEBOUNCE_INTERVAL) {
      if (soundValue) {
        tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
      }
      switchMenu = !switchMenu;
    }
  }
  if (enteredSubmenu == true) {
    if (millis() - lastDebounceTime > DEBOUNCE_INTERVAL) {
      if (soundValue) {
        tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
      }
      if (posSubmenu == SECOND_LINE_SUBMENU && highScoreState == false) {
        pushedButton = !pushedButton;
      }

      else if (posSubmenu == BACK_BUTTON_ABOUT && posMenu == POS_ABOUT) {
        pushedBackAbout = !pushedBackAbout;
      }

      else if (posSubmenu == BACK_BUTTON_HIGH_SCORES && posMenu == POS_HIGHSCORES) {

        pushedBackHighScore = !pushedBackHighScore;
      }

      else if (posSubmenu == FIRST_LINE_SUBMENU && posMenu == POS_START_GAME && highScoreState == false && setNameState == false) {
        gameStartButton = true;
      }

      else if (posSubmenu == FIRST_LINE_SUBMENU && posMenu == SET_CONTRAST) {
        editContrast = !editContrast;
      }

      else if (posSubmenu == FIRST_LINE_SUBMENU && posMenu == SET_SOUND) {
        editSound = !editSound;
      }

      else if (posSubmenu == FIRST_LINE_SUBMENU && posMenu == SET_BRIGHTNESS) {
        editBrightness = !editBrightness;
      }

      else if (posSubmenu == FIRST_LINE_SUBMENU && posMenu == SET_INTENSITY) {
        editIntensity = !editIntensity;
      }

      else if (posSubmenu == FIRST_LINE_SUBMENU && posMenu == SET_DIFICULTY) {
        editDificulty = !editDificulty;

      }
      if (posMenu == POS_START_GAME && posSubmenu == FIRST_LINE_SUBMENU && pozY == POS_FIRST_LETTER && setNameState == true) {
        switchLetter1 = !switchLetter1;

      }
      if (posSubmenu == FIRST_LINE_SUBMENU && pozY == POS_SECOND_LETTER && setNameState == true) {
        switchLetter2 = !switchLetter2;

      }
      if (posSubmenu == FIRST_LINE_SUBMENU && pozY == POS_THIRD_LETTER && setNameState == true) {
        switchLetter3 = !switchLetter3;

      }
      if (posSubmenu == FIRST_LINE_SUBMENU && pozY == POS_4_LETTER && setNameState == true) {
        switchLetter4 = !switchLetter4;

      }
      if (posSubmenu == FIRST_LINE_SUBMENU && pozY == POS_5_LETTER && setNameState == true) {
        switchLetter5 = !switchLetter5;

      }
      if (posSubmenu == FIRST_LINE_SUBMENU && pozY == POS_6_LETTER && setNameState == true) {
        switchLetter6 = !switchLetter6;

      }
      if (posSubmenu == FIRST_LINE_SUBMENU && pozY == POS_7_LETTER && setNameState == true) {
        switchLetter7 = !switchLetter7;

      }
      if (posSubmenu == NEXT_BUTTON && posMenu == POS_START_GAME && highScoreState == true) {
        nextHighScore = !nextHighScore;

      }
      if (posSubmenu == SECOND_LINE_SUBMENU && posMenu == POS_START_GAME && setNameState == true && highScoreState == false) {
        pushedOk = !pushedOk;
      }
    }
  }

  lastDebounceTime = millis();
}

void showMenu() {

  if (posMenu < NEXT_MENU_PAGE && posMenu % 2 == 0) {
    lcd.setCursor(0, 0);
    lcd.print(content[posMenu]);
    lcd.setCursor(0, 1);
    lcd.print(content[posMenu + 1]);
  }
  if (posMenu < LAST_LINE_MENU && joyMoved && posMenu % 2 == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(content[posMenu]);
    lcd.setCursor(0, 1);
    lcd.print(content[posMenu + 1]);
  }
}

void showAbout() {

  if (posSubmenu < THIRD_LINE_SUBMENU && posSubmenu % 2 == 0) {
    lcd.setCursor(0, 0);
    lcd.print(about[posSubmenu]);
    lcd.setCursor(0, 1);
    lcd.print(about[posSubmenu + 1]);
  }
  if (posSubmenu < LEN_ABOUT && joyMoved && posSubmenu % 2 == 0) {
    if (posSubmenu != POS_GITHUB && posSubmenu != POS_LINK_GITHUB) {
      lcd.clear();
      indexAbout = 0;
    } else if (posSubmenu == POS_GITHUB) {
      lcd.setCursor(CURSOR_GITHUB, 0);
      lcd.print("         ");
    }

    lcd.setCursor(0, 0);
    lcd.print(about[posSubmenu]);
    if (posSubmenu == POS_GITHUB) {

      lcd.setCursor(POS_ARROW_GITHUB, 0);
      lcd.write(byte(1));
      lcd.setCursor(POS_ARROW_GITHUB + 1, 0);
      lcd.write(byte(1));

      if (indexAbout <= strlen(link) - LCD_COLUMNS) {
        if (millis() - currentMillis > SCROLL_INTERVAL) {
          showLettersLinkUp(0, indexAbout);
          currentMillis = millis();
          indexAbout += 1;
        }
      }
      else {
        indexAbout = 0;
      }
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print(about[posSubmenu + 1]);
    }
  }
  yAxisSubmenu(LEN_ABOUT - 1);
}

void showLettersLinkUp(int start, int startLetter) {
  lcd.setCursor(start, 1);
  for (int letter = startLetter; letter <= startLetter + LCD_COLUMNS - 1; letter++) {
    lcd.print(link[letter]);
  }
}

//universal function that handles all settings

void set(String setting) {

  lcd.setCursor(0, 0);
  lcd.print(setting + ": ");
  lcd.setCursor(0, 1);

  lcd.print(content[8]);

  if (setting == "Contrast") {
    int lastLastContrastValue = lastContrastValue;

    while (editContrast) {
      lcd.setCursor(setting.length() + SPACING, 0);
      lcd.print(contrastValue);
      yAxisContrast();

      if (lastContrastValue != contrastValue) {
        lcd.setCursor(setting.length() + SPACING, 0);
        lcd.print("      ");
        lcd.setCursor(setting.length() + SPACING, 0);
        lcd.print(contrastValue);
        lastContrastValue = contrastValue;
      }
      analogWrite(CONTRAST_PIN, contrastValue);
    }

    lcd.setCursor(setting.length() + SPACING1, 0);
    if (lastLastContrastValue != lastContrastValue) {
      String message = "Saved";
      lcd.print(message);
      delay(DELAY_SAVED); //delay used for effect
    }

    EEPROM.update(ADRESS_CONTRAST, contrastValue);

  }
  else if (setting == "Brightness") {
    int lastLastBrightnessValue = lastBrightnessValue;

    while (editBrightness) {
      lcd.setCursor(setting.length() + SPACING, 0);
      lcd.print(brightnessValue);

      yAxisBrightness();
      if (lastBrightnessValue != brightnessValue) {
        lcd.setCursor(setting.length() + SPACING, 0);
        lcd.print("      ");
        lcd.setCursor(setting.length() + SPACING, 0);
        lcd.print(brightnessValue);
        lastBrightnessValue = brightnessValue;
        Serial.println(brightnessValue);
      }
      analogWrite(BRIGHTNESS_PIN, brightnessValue);
    }
    lcd.setCursor(setting.length() + SPACING1, 0);
    if (lastLastBrightnessValue != lastBrightnessValue) {
      String message = "Saved";
      lcd.print(message);
      delay(DELAY_SAVED); // delay used for effect
    }

    EEPROM.update(ADDRESS_BRIGHTNESS, brightnessValue);
  }
  else if (setting == "Intensity") {

    int lastLastIntensityValue = lastIntensityValue;
    while (editIntensity) {
      showMatrix();
      lcd.setCursor(setting.length() + SPACING, 0);
      lcd.print(String(intensityValue));
      yAxisIntensity();
      if (lastIntensityValue != intensityValue) {
        lcd.setCursor(setting.length() + SPACING, 0);
        lcd.print(String(intensityValue));
        lastIntensityValue = intensityValue;
      }

      lc.setIntensity(0, intensityValue);
    }
    lc.clearDisplay(0);
    lcd.setCursor(setting.length() + SPACING, 0);
    if (lastLastIntensityValue != lastIntensityValue) {
      String message = "Saved";
      lcd.print(message);
      delay(DELAY_SAVED); //delay used for effect
    }

    EEPROM.update(ADDRESS_INTENSITY, intensityValue);

  }
  else if (setting == "Sound") {

    int lastlastSoundValue = lastSoundValue;
    while (editSound) {
      lcd.setCursor(setting.length() + SPACING, 0);
      if (soundValue == ON) {
        lcd.print(" On");
      }
      else {
        lcd.print("Off");
      }
      yAxisSound();
      if (lastSoundValue != soundValue) {
        lcd.setCursor(setting.length() + SPACING, 0);
        if (soundValue == 1) {
          lcd.print(" On");
        }
        else {
          lcd.print("Off");
        }
        lastSoundValue = soundValue;
      }
    }

    lcd.setCursor(setting.length() + SPACING, 0);
    if (lastlastSoundValue != lastSoundValue) {
      String message = "Saved";
      lcd.print(message);
      delay(DELAY_SAVED); // delay used for effect
    }

    EEPROM.write(ADDRESS_SOUND, soundValue);

  }
  else if (setting == "Dificulty") {
    String lastLastDificultyValue = lastDificultyValue;

    while (editDificulty) {
      lcd.setCursor(setting.length() + SPACING, 0);
      lcd.print(dificultyValue);

      yAxisDificulty();
      if (lastDificultyValue != dificultyValue) {
        lcd.setCursor(setting.length() + SPACING, 0);
        lcd.print("      ");
        lcd.setCursor(setting.length() + SPACING, 0);
        lcd.print(dificultyValue);
        lastDificultyValue = dificultyValue;
      }
    }
    lcd.setCursor(setting.length() + SPACING, 0);
    if (lastLastDificultyValue != lastDificultyValue) {
      String message = "Saved";
      lcd.print(message);
      delay(DELAY_SAVED); // delay used for effect
    }
  }
  yAxisSubmenu(1);
}

void showMatrix() {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      lc.setLed(0, i, j, 1);
    }
  }
}

// show highScores list (name + score) from EEPROM

void showHighScores() {
  if (posSubmenu < THIRD_LINE_SUBMENU && posSubmenu % 2 == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(readNameFromEEPROM(ADDRESS_NAME1));
    lcd.setCursor(POS_SCORE, 0);
    lcd.print("Score:" + String(EEPROM.read(ADDRESS_SCORE1)));
    lcd.setCursor(0, 1);
    lcd.print(readNameFromEEPROM(ADDRESS_NAME2));
    lcd.setCursor(POS_SCORE, 1);
    lcd.print("Score:" + String(EEPROM.read(ADDRESS_SCORE2)));
  }
  else if (posSubmenu > FIRST_LINE_SUBMENU && posSubmenu < LAST_LINE_SUBMENU_HIGH_SCORES && joyMoved && posSubmenu % 2 == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(readNameFromEEPROM(ADDRESS_NAME3));
    lcd.setCursor(POS_SCORE, 0);
    lcd.print("Score:" + String(EEPROM.read(ADDRESS_SCORE3)));
    lcd.setCursor(0, 1);
    lcd.print("> Back");
  }

  // navigate in the submenu
  yAxisSubmenu(LEN_HIGH_SCORES);
}


// Set each letter by first selecting the position of the letter
// the positions are between 9-15 on LCD. Click on the position and
// select the letter by moving the joystick up and down and then click
// again to save it

void setName() {

  while (!pushedOk) {
    if (switchLetter) {
      xAxisLetters();
      while (switchLetter1) {
        yAxisLetters();
        lcd.setCursor(POS_FIRST_LETTER, 0);
        lcd.print(letter);
      }
      while (switchLetter2) {
        yAxisLetters();
        lcd.setCursor(POS_SECOND_LETTER, 0);
        lcd.print(letter);
      }
      while (switchLetter3) {
        yAxisLetters();
        lcd.setCursor(POS_THIRD_LETTER, 0);
        lcd.print(letter);
      }
      while (switchLetter4) {
        yAxisLetters();
        lcd.setCursor(POS_4_LETTER, 0);
        lcd.print(letter);
      }
      while (switchLetter5) {
        yAxisLetters();
        lcd.setCursor(POS_5_LETTER, 0);
        lcd.print(letter);
      }
      while (switchLetter6) {
        yAxisLetters();
        lcd.setCursor(POS_6_LETTER, 0);
        lcd.print(letter);
      }
      while (switchLetter7) {
        yAxisLetters();
        lcd.setCursor(POS_7_LETTER, 0);
        lcd.print(letter);
      }
      yAxisSubmenu(LEN_SET_NAME);
    }
  }

  for (int i = 0; i < LEN_NAME; i++) {
    if (nameLetters[i] != '-') {
      namePlayer += String(nameLetters[i]);
    }
  }

  for (int i = 0; i < LEN_NAME; i++) {
    nameLetters[i] = '-';
  }

  pozY = POS_FIRST_LETTER;
  posLetter = 0;

  updateHighScoreList(highScore, namePlayer);
  namePlayer = "";
}

void writeNameToEEPROM(int address, String namePlayer)
{
  byte len = namePlayer.length();
  EEPROM.update(address, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.update(address + 1 + i, namePlayer[i]);
  }
}

String readNameFromEEPROM(int address)
{
  int newNameLen = EEPROM.read(address);
  char newName[newNameLen + 1];
  for (int i = 0; i < newNameLen; i++)
  {
    newName[i] = EEPROM.read(address + 1 + i);
  }
  newName[newNameLen] = '\0';
  return String(newName);
}

void yAxisLetters() {
  yValue = analogRead(PIN_Y);

  if (nameLetters[posLetter] == '-') {
    letter = 'a';
  }
  if (yValue > MAX_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    letter--;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_NAME, TIME_SOUND_MENU);
    }

    if (letter < 'a') {
      letter = 'z';
    }
  }

  if (yValue < MIN_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    letter++;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_NAME, TIME_SOUND_MENU);
    }

    if (letter > 'z') {
      letter = 'a';
    }
  }

  nameLetters[posLetter] = letter;

  if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
    joyMoved1 = false;
  }
}

void xAxisLetters() {
  lcd.setCursor(pozY, 0);
  lcd.blink();

  xValue = analogRead(PIN_X);

  if (xValue > MAX_THRESHOLD && joyMoved2 == false) {
    joyMoved2 = true;
    pozY++;
    posLetter++;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (pozY > POS_7_LETTER) {
      pozY = POS_FIRST_LETTER;
    }
    if (posLetter > LEN_NAME - 1) {
      posLetter = 0;
    }
  }

  if (xValue < MIN_THRESHOLD && joyMoved2 == false) {
    joyMoved2 = true;
    pozY--;
    posLetter--;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (pozY < POS_FIRST_LETTER) {
      pozY = POS_7_LETTER;
    }
    if (posLetter < 0) {
      posLetter = LEN_NAME - 1;
    }
  }

  if (xValue > MIN_THRESHOLD && xValue < MAX_THRESHOLD) {
    joyMoved2 = false;
  }
}


void yAxisContrast() {
  yValue = analogRead(PIN_Y);

  if (yValue > MAX_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    contrastValue -= CONTRAST_RATIO;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (contrastValue < MIN_CONTRAST_VALUE) {
      contrastValue = MIN_CONTRAST_VALUE;
    }
  }

  if (yValue < MIN_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    contrastValue += CONTRAST_RATIO;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (contrastValue > MAX_XONTRAST_VALUE) {
      contrastValue = MAX_XONTRAST_VALUE;
    }
  }

  if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
    joyMoved1 = false;
  }
}

void yAxisDificulty() {
  yValue = analogRead(PIN_Y);

  if (yValue > MAX_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    posDificulty -= 1;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (posDificulty < 0) {
      posDificulty = LEN_DIFICULTY_LIST - 1;
    }
  }

  if (yValue < MIN_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    posDificulty += 1;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (posDificulty > LEN_DIFICULTY_LIST - 1) {
      posDificulty = 0;
    }
  }

  dificultyValue = dificultyValues[posDificulty];

  if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
    joyMoved1 = false;
  }
}

void yAxisBrightness() {
  yValue = analogRead(PIN_Y);
  if (yValue > MAX_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    brightnessValue -= BRIGHTNESS_RATIO;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (brightnessValue < MIN_BRIGHTNESS_VALUE) {
      brightnessValue = MIN_BRIGHTNESS_VALUE;
    }
  }

  if (yValue < MIN_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    brightnessValue += BRIGHTNESS_RATIO;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (brightnessValue > MAX_BRIGHTNESS_VALUE) {
      brightnessValue = MAX_BRIGHTNESS_VALUE;
    }
  }

  if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
    joyMoved1 = false;
  }
}

void yAxisIntensity() {
  yValue = analogRead(PIN_Y);

  if (yValue > MAX_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    intensityValue -= INTENSITY_RATIO;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (intensityValue < MIN_INTENSITY_VALUE) {
      intensityValue = MAX_INTENSITY_VALUE;
    }
  }

  if (yValue < MIN_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    intensityValue += INTENSITY_RATIO;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (intensityValue > MAX_INTENSITY_VALUE) {
      intensityValue = MIN_INTENSITY_VALUE;
    }
  }

  if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
    joyMoved1 = false;
  }
}

void yAxisSound() {
  yValue = analogRead(PIN_Y);
  if (yValue > MAX_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    soundValue -= SOUND_RATIO;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (soundValue < OFF) {
      soundValue = ON;
    }
  }

  if (yValue < MIN_THRESHOLD && joyMoved1 == false) {
    joyMoved1 = true;
    soundValue += SOUND_RATIO;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (soundValue > ON) {
      soundValue = OFF;
    }
  }

  if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
    joyMoved1 = false;
  }
}

void yAxisMenu() {

  lcd.setCursor(0, posMenu % ROWS_ON_LCD);
  lcd.blink();
  yValue = analogRead(PIN_Y);
  if (yValue > MAX_THRESHOLD && joyMoved == false) {
    joyMoved = true;
    posMenu++;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (posMenu > LAST_LINE_MENU) {
      posMenu = FIRST_LINE_MENU;
    }
  }

  if (yValue < MIN_THRESHOLD && joyMoved == false) {
    joyMoved = true;
    posMenu--;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (posMenu < FIRST_LINE_MENU) {
      posMenu = LAST_LINE_MENU;
    }
  }

  if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
    joyMoved = false;
  }
}


void yAxisSubmenu(int elements) {

  lcd.setCursor(0, posSubmenu % ROWS_ON_LCD);
  lcd.blink();

  yValue = analogRead(PIN_Y);

  if (yValue > MAX_THRESHOLD && joyMoved == false) {
    joyMoved = true;
    posSubmenu++;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (posSubmenu > elements) {
      posSubmenu = elements;
    }
  }

  if (yValue < MIN_THRESHOLD && joyMoved == false) {
    joyMoved = true;
    posSubmenu--;

    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_MENU, TIME_SOUND_MENU);
    }

    if (posSubmenu < FIRST_LINE_SUBMENU) {
      posSubmenu = FIRST_LINE_SUBMENU;
    }
  }

  if (yValue > MIN_THRESHOLD && yValue < MAX_THRESHOLD) {
    joyMoved = false;
  }
}

//------------------------------------ GAME -------------------------------------------

void game() {
  lcd.clear();
  lc.clearDisplay(0);

  winGame = false;
  lostGame = false;

  foodX = RESTART_FOOD_VALUE;
  foodY = RESTART_FOOD_VALUE;

  snakeDirection = STATIONARY;
  snakeLength = INITIAL_SNAKE_LENGTH;

  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      matrix[i][j] = 0;
    }
  }
  if (dificultyValue == "" || dificultyValue == "Easy") {
    scoreRatio = EASY_RATIO;
    speedValue = easySpeed;
    showEasyMessage();
  }
  else if (dificultyValue == "Med") {
    scoreRatio = MEDIUM_RATIO;
    speedValue = easySpeed;
    showMediumMessage();
  }
  else if (dificultyValue == "Hard") {
    scoreRatio = HARD_RATIO;
    speedValue = hardSpeed;
    showHardMessage();
  }

  while (!lostGame && gameState == true && !winGame) {

    if (dificultyValue == "Med") {
      if (speedValue > hardSpeed) {
        speedValue = easySpeed - SPEED_RATIO_MEDIUM * (snakeLength - INITIAL_SNAKE_LENGTH);
      }
    }

    placeFood();

    score = (snakeLength - INITIAL_SNAKE_LENGTH) * scoreRatio;
    showScore();

    generateFood();

    joyStickMovement();
    calculateSnakeDirection();
    snakeMovement();

    if ((dificultyValue == "" || dificultyValue == "Easy") && score == MAX_SCORE_EASY) {
      winGame = true;
    }
    else if (dificultyValue == "Med" && score == MAX_SCORE_MEDIUM) {
      winGame = true;
    }
    else if (dificultyValue == "Hard" && score == MAX_SCORE_HARD) {
      winGame = true;
    }
  }

  if (soundValue) {
    tone(BUZZER_PIN, BUZZER_TONE_FOOD, TIME_SOUND_GAME);
  }

  if (lostGame) {
    if (calculateHighScore()) {
      highScoreState = true;
      setNameState = false;
      highScore = score;
    }
    else if (!calculateHighScore()) {
      lostGameState = true;
      highScoreState = false;
    }
  }
  else if (winGame) {
    winGameState = true;
  }

  gameState = false;

  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      lc.setLed(0, j, i, false);
      delay(DELAY_END_OF_GAME); // delay used at end of the game for cleaning the matrix effect
    }
  }
  lc.clearDisplay(0);
}

void showScore() {
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.setCursor(POS_SCORE, 0);
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("Dificulty:");
  lcd.setCursor(POS_DIFICULTY_VALUE, 1);
  if (dificultyValue == "") {
    lcd.print("Easy");
  }
  else {
    lcd.print(dificultyValue);
  }
}

void showCongrats() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HIGH SCORE");
  lcd.setCursor(0, 1);
  lcd.print("> Next");

  yAxisSubmenu(NEXT_BUTTON);
}

// verify if the current score is highscore by comparing it with top 3 highScores from EEPROM

bool calculateHighScore() {
  return (score > EEPROM.read(ADDRESS_SCORE1) || score > EEPROM.read(ADDRESS_SCORE2) || score > EEPROM.read(ADDRESS_SCORE3));
}

void generateFood() {

  if (foodX == RESTART_FOOD_VALUE || foodY == RESTART_FOOD_VALUE) {
    do {
      foodX = rand() % (MATRIX_SIZE - 1);
      foodY = rand() % (MATRIX_SIZE - 1);
    } while (matrix[foodX][foodY] > 0);

    placeFood();
  }
}

void placeFood() {

  if (millis() - previousMillis >= INTERVAL_BLINK) {
    previousMillis = millis();
    foodBlink = !foodBlink;
  }
  lc.setLed(0, foodX, foodY, foodBlink);
}

// map function for float numbers

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void joyStickMovement() {

  int lastSnakeDirection = snakeDirection;

  long timestamp = millis();

  while (millis() < timestamp + snakeSpeed) {

    // map the speed that is maximum 900 so we can use it as a timer
    float raw = mapf(speedValue, 0, MAX_SPEED_VALUE, 0, 1);
    snakeSpeed = mapf(pow(raw, 3.5), 0, 1, 10, 1000);

    if (snakeSpeed == STATIONARY) {
      snakeSpeed = INITIAL_SNAKE_SPEED;
    }

    int xValue = analogRead(PIN_X);
    int yValue = analogRead(PIN_Y);

    if (xValue < MIN_THRESHOLD) {
      snakeDirection = DOWN;
    }
    if (xValue > MAX_THRESHOLD) {
      snakeDirection = UP;
    }
    if (yValue < MIN_THRESHOLD) {
      snakeDirection = LEFT;
    }
    if (yValue > MAX_THRESHOLD) {
      snakeDirection = RIGHT;
    }

    // if snake is going UP we don't consider de next direction as DOWN
    // also if the snake is going rigth we don't consider the next direction as LEFT

    if (snakeDirection + 2 == lastSnakeDirection && lastSnakeDirection != STATIONARY) {
      snakeDirection = lastSnakeDirection;
    }
    if (snakeDirection - 2 == lastSnakeDirection && lastSnakeDirection != STATIONARY) {
      snakeDirection = lastSnakeDirection;
    }
  }
}

void calculateSnakeDirection() {

  if (snakeDirection == UP) {
    xPos--;
    teleportSnake();
    lc.setLed(0, xPos, yPos, 1);
  }
  else if (snakeDirection == RIGHT) {
    yPos--;
    teleportSnake();
    lc.setLed(0, xPos, yPos, 1);
  }
  else if (snakeDirection == DOWN) {
    xPos++;
    teleportSnake();
    lc.setLed(0, xPos, yPos, 1);
  }
  else if (snakeDirection == LEFT) {
    yPos++;
    teleportSnake();
    lc.setLed(0, xPos, yPos, 1);
  }
  else {
    return;
  }

  if (matrix[xPos][yPos] > 1 && snakeDirection != STATIONARY) {
    lostGame = true;
    return;
  }
}

void snakeMovement() {

  // if the food is found

  if (xPos == foodX && yPos == foodY) {
    foodX = RESTART_FOOD_VALUE;
    foodY = RESTART_FOOD_VALUE;

    // make a specific sound
    if (soundValue) {
      tone(BUZZER_PIN, BUZZER_TONE_FOOD, TIME_SOUND_MENU);
    }

    // increase kength of the snake
    snakeLength++;

    // add 1 from head to tail so we can have value 2 at tail
    for (int i = 0; i < MATRIX_SIZE; i++) {
      for (int j = 0; j < MATRIX_SIZE; j++) {
        if (matrix[i][j] > 0 ) {
          matrix[i][j]++;
        }
      }
    }
  }

  // new head of the snake
  matrix[xPos][yPos] = snakeLength + 1;

  // decrement 1 from head to tail;
  // if the food was eaten tail will now be 1, if not will be 0

  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      if (matrix[i][j] > 0 ) {
        matrix[i][j]--;
      }

      // show the snake on the matrix
      if (matrix[i][j] == 0) {
        lc.setLed(0, i, j, 0);
      }
      else {
        lc.setLed(0, i, j, 1);
      }
    }
  }
}

// snake can go through the matrix walls

void teleportSnake() {
  if (yPos < 0) {
    yPos += MATRIX_SIZE;
  }
  if (yPos > MATRIX_SIZE - 1) {
    yPos -= MATRIX_SIZE;
  }
  if (xPos < 0) {
    xPos += MATRIX_SIZE;
  }
  if (xPos > MATRIX_SIZE - 1) {
    xPos -= MATRIX_SIZE;
  }
}

// not used yet

bool isSnakeOnEdge() {
  if (gameState == true) {
    if (xPos == 0) {
      return true;
    }
    else if (xPos == MATRIX_SIZE - 1) {
      return true;
    }
    else if (yPos == 0) {
      return true;
    }
    else if (yPos == MATRIX_SIZE - 1) {
      return true;
    }
  }
}

// Update list of High Score by verifying first which highscore was beated and then Update it accordingly

void updateHighScoreList(int highScore, String player) {

  if (highScore > EEPROM.read(ADDRESS_SCORE1)) {
    namePlayer3 = namePlayer2;
    namePlayer2 = namePlayer1;
    namePlayer1 = player;

    writeNameToEEPROM(ADDRESS_NAME1, namePlayer1);
    writeNameToEEPROM(ADDRESS_NAME2, namePlayer2);
    writeNameToEEPROM(ADDRESS_NAME3, namePlayer3);

    scorePlayer3 = scorePlayer2;
    scorePlayer2 = scorePlayer1;
    scorePlayer1 = highScore;

    EEPROM.write(ADDRESS_SCORE1, scorePlayer1);
    EEPROM.write(ADDRESS_SCORE2, scorePlayer2);
    EEPROM.write(ADDRESS_SCORE3, scorePlayer3);
  }

  else if (highScore > EEPROM.read(ADDRESS_SCORE2)) {
    namePlayer3 = namePlayer2;
    namePlayer2 = namePlayer;

    writeNameToEEPROM(ADDRESS_NAME2, namePlayer2);
    writeNameToEEPROM(ADDRESS_NAME3, namePlayer3);

    scorePlayer3 = scorePlayer2;
    scorePlayer2 = highScore;

    EEPROM.write(ADDRESS_SCORE2, scorePlayer2);
    EEPROM.write(ADDRESS_SCORE3, scorePlayer3);
  }

  else if (highScore > EEPROM.read(ADDRESS_SCORE3)) {
    namePlayer3 = namePlayer;

    writeNameToEEPROM(ADDRESS_NAME3, namePlayer3);

    scorePlayer3 = highScore;

    EEPROM.write(ADDRESS_SCORE3, scorePlayer3);

  }
}

// -------------------------- INTROS ------------------------------

const PROGMEM bool snakeStart[LEN_SNAKE_MESSAGE + MATRIX_SIZE][MATRIX_SIZE] = {

  {0, 0, 0, 0, 0, 0, 0, 0}, // E
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, // K
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 1, 0, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, // A
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 1, 0},
  {0, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {1, 1, 1, 1, 1, 1, 1, 0}, // N
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, //S
  {1, 1, 1, 1, 0, 1, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 0, 1, 1, 1, 1, 0},
  {1, 1, 0, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};


const PROGMEM bool snakeEasy[LEN_SNAKE_EASY + MATRIX_SIZE][MATRIX_SIZE] = {


  {0, 0, 0, 0, 0, 0, 0, 0}, //Y
  {0, 0, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, //S
  {1, 1, 1, 1, 0, 1, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 0, 1, 1, 1, 1, 0},
  {1, 1, 0, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, // A
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 1, 0},
  {0, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, // E
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
const PROGMEM bool snakeMedium[LEN_SNAKE_MEDIUM + MATRIX_SIZE][MATRIX_SIZE] = {

  {0, 0, 0, 0, 0, 0, 0, 0}, //M
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},


  {0, 0, 0, 0, 0, 0, 0, 0}, // U
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, //I
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0},//D
  {0, 1, 1, 1, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, // E
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, //M
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0}
};
const PROGMEM bool snakeHard[LEN_SNAKE_HARD + MATRIX_SIZE][MATRIX_SIZE] = {

  {0, 0, 0, 0, 0, 0, 0, 0},//D
  {0, 1, 1, 1, 1, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},


  {0, 0, 0, 0, 0, 0, 0, 0},// R
  {1, 0, 0, 0, 1, 1, 0, 0},
  {1, 1, 0, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 0, 0, 1, 0},
  {0, 0, 1, 1, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},


  {0, 0, 0, 0, 0, 0, 0, 0}, // A
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 0, 0, 1, 0},
  {0, 0, 0, 1, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},

  {0, 0, 0, 0, 0, 0, 0, 0}, // H
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}

};

void showSnakeMessage() {
  [&] {

    lcd.print("Welcome to");
    lcd.setCursor(0, 1);
    lcd.print("ssSnake Game");

    for (int messageSize = LEN_SNAKE_MESSAGE; messageSize > 0; messageSize--) {
      for (int j = MATRIX_SIZE; j >= 0; j--) {
        delay(DELAY_MESSAGES); // delay for effect
        for (int i = 0; i < MATRIX_SIZE + 1; i++) {
          // this reads the byte from the PROGMEM and displays it on the screen
          lc.setLed(0, i, j, pgm_read_byte(&(snakeStart[i + messageSize][j])));
        }
      }
    }
  }();

  lc.clearDisplay(0);
}

void showEasyMessage() {
  [&] {

    lcd.setCursor(0, 0);
    lcd.print("ssSnake EASY");

    for (int messageSize = LEN_SNAKE_EASY; messageSize > 0; messageSize--) {
      for (int j = MATRIX_SIZE; j >= 0; j--) {
        delay(DELAY_MESSAGES); // delay for effect
        for (int i = 0; i < MATRIX_SIZE + 1; i++) {
          lc.setLed(0, i, j, pgm_read_byte(&(snakeEasy[i + messageSize][j])));
        }
      }
    }
  }();

  lc.clearDisplay(0);
  lcd.clear();
}

void showMediumMessage() {
  [&] {

    lcd.setCursor(0, 0);
    lcd.print("ssSnake MEDIUM");

    for (int messageSize = LEN_SNAKE_MEDIUM; messageSize > 0; messageSize--) {
      for (int j = MATRIX_SIZE; j >= 0; j--) {
        delay(DELAY_MESSAGES); // delay for effect
        for (int i = 0; i < MATRIX_SIZE + 1; i++) {
          lc.setLed(0, i, j, pgm_read_byte(&(snakeMedium[i + messageSize][j])));
        }
      }
    }
  }();

  lc.clearDisplay(0);
  lcd.clear();
}

void showHardMessage() {
  [&] {

    lcd.setCursor(0, 0);
    lcd.print("ssSnake HARD");

    for (int messageSize = LEN_SNAKE_HARD; messageSize > 0; messageSize--) {
      for (int j = MATRIX_SIZE; j >= 0; j--) {
        delay(DELAY_MESSAGES); // delay for effect
        for (int i = 0; i < MATRIX_SIZE + 1; i++) {
          lc.setLed(0, i, j, pgm_read_byte(&(snakeHard[i + messageSize][j])));
        }
      }
    }
  }();

  lc.clearDisplay(0);
  lcd.clear();
}
