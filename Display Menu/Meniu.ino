#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#include <stdlib.h>
#include<time.h>

const int pinSW = 2; 
const int pinX = A0; 
const int pinY = A1;
const int pinV0 = 6;

LiquidCrystal lcd(13, 9, 5, 4, 3, 7);

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int rows = 8;
const int cols = 8;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

const int matrixSize = 8;

bool matrixUpdate = true;

byte xPos = 0;
byte yPos = 0;

byte xLastPos = 0;
byte yLastPos = 0;

int score = 0;
int highScore = 0;
String way = " ";
int foodI = 0;
int foodJ = 0;

const int intervalBlink = 500;
const int moveInterval = 400;
unsigned int lastMoved = 0;


bool matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};

int xValue = 0;
int yValue = 0;

bool joyMoved = false;
int minThreshold= 400;
int maxThreshold= 600;

const int adressContrast = 0;
const int adressBrightness = 1;
const int adressIntensity = 2;
const int adressDificulty = 3;

unsigned long startTime = 0;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

int posMenu = -2;
int posSubmenu = 0;

bool enteredSubmenu = false;
bool pushedButton = false;

bool gameStarted = false;
bool gameStartButton = false;

int pageNumberMenu = -1;

int lastContrastValue = 75;
int contrastValue = 75;

int brightnessValue = 100;
int lastBrightnessValue = 100;

int intensityValue = 10;
int lastIntensityValue = 10;

String dificultyValue = "Easy";
String lastDificultyValue = "Easy";

//bool enteredSettingsSubmenu = false;
int posSettings = 0;

bool joyMoved1 = false;

bool editContrast = false;
bool editBrightness = false;
bool editIntensity = false;
bool editDificulty = false;

String dificultyValues[3] = {
  "Easy", "Med", "Hard"
};

//----------------------------

int switchValue;
bool switchLetter = true;
bool switchLetter1 = false;
bool switchLetter2 = false;
bool switchLetter3 = false;
bool switchLetter4 = false;
bool switchLetter5 = false;
bool switchLetter6 = false;
bool switchLetter7 = false;
int pos = 0;

char letter = 'a';
char letterLimit = 'z';

const int lenName = 7; 
char nameLetters[lenName] = {
  ' ', ' ', ' ', ' ', ' ', ' ', ' '
};

int lettersAdress[lenName] = {
  10, 11, 12, 13, 14, 15, 16
};

int pozY = 9;

int posLetter = 0;

String namePlayer = " ";

//-----------------------------------

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

String content[9] = {
  "> Start Game", "> High Scores", "> Set Name", "> About","> Set Contrast", "> Set Brightness", "> Matrix Intensity", "> Set Dificulty", "> Back"
};


bool switchMenu = false;

int lastDebounceTimer = 0;
const int debounceInterval = 50;


void setup() {

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  pinMode(pinSW, INPUT_PULLUP); //activate pull-up resistor on the
  pinMode(pinX,INPUT);
  pinMode(pinY,INPUT);

  attachInterrupt(digitalPinToInterrupt(pinSW), enterSubmenu, FALLING);
  
  Serial.begin(9600);
  lcd.createChar(0,arrow);
  lcd.begin(16, 2);  
  lcd.setCursor(6,0);
  lcd.print("MENU");
  lcd.setCursor(8,1);
  lcd.write(byte(0));
  lcd.setCursor(7,1);
  lcd.write(byte(0));

  contrastValue = EEPROM.read(adressContrast);
  intensityValue = EEPROM.read(adressIntensity);
  brightnessValue = EEPROM.read(adressBrightness);

  lc.shutdown(0, false); 
  lc.setIntensity(0, intensityValue);
  lc.clearDisplay(0);
  srand((int)time(0));
  spawnFood();
}


void loop() {
  
    
    if (switchMenu == true) {
    enteredSubmenu = true;
    if(posMenu == 4){
      lcd.clear();
      setContrast();
    }
    else if(posMenu == 5){
      lcd.clear();
      setBrightness();
    }
    else if(posMenu == 6){
      lcd.clear();
      setMatrixIntensity();
    }
    else if(posMenu == 7){
      lcd.clear();
      setDificulty();
    }
    else if(posMenu == 2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set Name:");
      lcd.setCursor(0,1);
      lcd.print("> Back");
      setName();
    }
    else if(posMenu == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("> Start Game");
      lcd.setCursor(0,1);
      lcd.print("> Back");
      xAxis2();
      if(gameStartButton){
        game();
      }
    }
    
    
    if(pushedButton == true && posSubmenu == 1){
      switchMenu = false;
      enteredSubmenu = false;
      posSubmenu = 0;
      pushedButton = false;
      posMenu = 0;
    }
  
    
  }
  if(gameStartButton == true && posSubmenu == 0){
      game();
    }
  
  else if(enteredSubmenu == false && switchMenu == false){
    showMenu();
    xAxis1();
  }
  
  delay(150);
}

void setName() {
  if(switchLetter){
      yAxis();
      if(switchLetter1){
        xAxisLetters();
        lcd.setCursor(9,0);
        lcd.print(letter);
      }
      else if(switchLetter2){
        xAxisLetters();
        lcd.setCursor(10,0);
        lcd.print(letter);
      }
      else if(switchLetter3){
        xAxisLetters();
        lcd.setCursor(11,0);
        lcd.print(letter);
      }
      else if(switchLetter4){
        xAxisLetters();
        lcd.setCursor(12,0);
        lcd.print(letter);
      }
      else if(switchLetter5){
        xAxisLetters();
        lcd.setCursor(13,0);
        lcd.print(letter);
      }
      else if(switchLetter6){
        xAxisLetters();
        lcd.setCursor(14,0);
        lcd.print(letter);
      }
      else if(switchLetter7){
        xAxisLetters();
        lcd.setCursor(15,0);
        lcd.print(letter);
      }
    }
    
    else{
      xAxis2();
    }

    delay(150);
}

void setContrast() {
  lcd.setCursor(0,0);
  lcd.print("Contrast:");
  lcd.setCursor(0,1);
  lcd.print(content[8]);
  int lastLastContrastValue = lastContrastValue;
  while(editContrast){
      lcd.setCursor(9,0);
      if(contrastValue > 99){
        lcd.print("<" + String(contrastValue) + ">"); 
      }
      else if(contrastValue > 9){
        lcd.print("< " + String(contrastValue) + ">"); 
      }
      else if(contrastValue < 10){
        lcd.print("<  " + String(contrastValue) + ">"); 
      }
       
      xAxisContrast();
      
      if(lastContrastValue != contrastValue){
        lcd.setCursor(9,0);
      if(contrastValue > 99){
        lcd.print("<" + String(contrastValue) + ">"); 
      }
      else if(contrastValue > 9){
        lcd.print("< " + String(contrastValue) + ">"); 
      }
      else if(contrastValue < 10){
        lcd.print("<  " + String(contrastValue) + ">"); 
      }
      lastContrastValue = contrastValue;
    }   
  }
  lcd.setCursor(9,0);
  if(lastLastContrastValue != lastContrastValue){
  String message = "Saved";
  lcd.print(message);
  delay(1000);
  }

  EEPROM.update(adressContrast, contrastValue);
  
  xAxis2();
    
}

void game() {
  
  while(score < 2 && !pushedButton){
    placeFood();
  
    if(xPos == foodI && yPos == foodJ){
      spawnFood();
      placeFood();
      score++;
    }
  
    if (millis() - lastMoved > moveInterval) {
      // game logic
      updatePositions();
      lastMoved = millis();
    }
  
    if (matrixUpdate == true) {
      updateDisplay();
      matrixUpdate = false;
    }
    
  }

  lc.clearDisplay(0);
  showCongrats();
  
  
}

void showCongrats() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HIGH SCORE");
  lcd.setCursor(0,1);
  lcd.print("Congratulations");

  delay(2000);
}


void updateDisplay() {
  lc.clearDisplay(0);
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void calculateHighScore(){
  if (score > highScore){
    highScore = score;
  }
}

void spawnFood(){
  foodI = rand()%7;
  foodJ = rand()%7;
}
void placeFood() {
  
  if (millis() - previousMillis >= intervalBlink){
    previousMillis = millis();
    matrix[foodI][foodJ] = !matrix[foodI][foodJ];
    matrixUpdate = true;
  }
}

void updatePositions() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  if(!isMoveDiagonal()){
  xLastPos = xPos;
  yLastPos = yPos;
  }
  if (xValue < minThreshold && joyMoved == false) {
    if (xPos < matrixSize - 1) {
      xPos++;
    } 
    else {
      xPos = 0;
    }
  }
  
  if (xValue > maxThreshold && joyMoved == false) {
    if (xPos > 0) {
      xPos--;
    }
    else {
      xPos = matrixSize - 1;
    }
  }
 
  if (yValue < maxThreshold && joyMoved == false) {
    if (yPos < matrixSize - 1) {
      yPos++;
    } 
    else {
      yPos = 0;
    }
  }
  
  if (yValue > minThreshold && joyMoved == false) {
    if (yPos > 0) {
      yPos--;
    }
    else {
      yPos = matrixSize - 1;
    }
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMoved = false;
  }
  
  if ((xPos != xLastPos || yPos != yLastPos) && !isMoveDiagonal() ) {
    matrix[xPos][yPos] = 1;
    matrix[xLastPos][yLastPos] = 0;
    matrixUpdate = true;
    if(xPos == xLastPos - 1 && yPos == yLastPos){
      way = "up"; 
    } else if (xPos == xLastPos + 1 && yPos == yLastPos) {
        way = "down";
    } else if (xPos == xLastPos && yPos ==yLastPos - 1) {
        way = "right";
    } else if(xPos == xLastPos && yPos == yLastPos + 1) {
        way = "left";
    }
  }
  
}

int isMoveDiagonal(){
  if (xPos == xLastPos - 1 && yPos == yLastPos - 1){
    return 1;
  }
  else if(xPos == xLastPos - 1 && yPos == yLastPos + 1){
    return 1;
  }
  else if(xPos == xLastPos + 1 && yPos == yLastPos - 1){
    return 1;
  }
  else if(xPos == xLastPos + 1 && yPos == yLastPos + 1){
    return 1;
  }
  return 0;
}



void setMatrixIntensity() {
  lcd.setCursor(0,0);
  lcd.print("Intensity:");
  lcd.setCursor(0,1);
  lcd.print(content[8]);
  int lastLastIntensityValue = lastIntensityValue;
   while(editIntensity){
      lcd.setCursor(10,0);
          
          if(intensityValue > 9){
            lcd.print("< " + String(intensityValue) + ">"); 
          }
          else if(intensityValue < 10){
            lcd.print("<  " + String(intensityValue) + ">"); 
          }
       
        xAxisIntensity();
        if(lastIntensityValue != intensityValue){
          lcd.setCursor(10,0);
         
          if(intensityValue > 9){
            lcd.print("< " + String(intensityValue) + ">"); 
          }
          else if(intensityValue < 10){
            lcd.print("<  " + String(intensityValue) + ">"); 
          }
          lastIntensityValue = intensityValue;
        }   
    }
    lcd.setCursor(10,0);
    if(lastLastIntensityValue != lastIntensityValue){
    String message = "Saved";
    lcd.print(message);
    delay(1000);
    }

    EEPROM.update(adressIntensity,intensityValue);
    
    xAxis2();
    
}

void setBrightness() {
  lcd.setCursor(0,0);
  lcd.print("Brightness:");
  lcd.setCursor(0,1);
  lcd.print(content[8]);
  int lastLastBrightnessValue = lastBrightnessValue;
   while(editBrightness){
      lcd.setCursor(11,0);
          if(brightnessValue > 99){
          lcd.print("<" + String(brightnessValue) + ">"); 
          }
          else if(brightnessValue > 9){
            lcd.print("< " + String(brightnessValue) + ">"); 
          }
          else if(brightnessValue < 10){
            lcd.print("<  " + String(brightnessValue) + ">"); 
          }
       
        xAxisBrightness();
        if(lastBrightnessValue != brightnessValue){
          lcd.setCursor(11,0);
          if(brightnessValue > 99){
          lcd.print("<" + String(brightnessValue) + ">"); 
          }
          else if(contrastValue > 9){
            lcd.print("< " + String(brightnessValue) + ">"); 
          }
          else if(brightnessValue < 10){
            lcd.print("<  " + String(brightnessValue) + ">"); 
          }
          lastBrightnessValue = brightnessValue;
        }   
    }
    lcd.setCursor(11,0);
    if(lastLastBrightnessValue != lastBrightnessValue){
    String message = "Saved";
    lcd.print(message);
    delay(1000);
    }  

    EEPROM.update(adressBrightness,brightnessValue);
    
    xAxis2();
}

void setDificulty() {
  lcd.setCursor(0,0);
  lcd.print("Dificulty:");
  lcd.setCursor(0,1);
  lcd.print(content[8]);
  String lastLastDificultyValue = lastDificultyValue;
   while(editDificulty){
      lcd.setCursor(10,0);
      lcd.print(dificultyValue);
       
      xAxisDificulty();
      if(lastDificultyValue != dificultyValue){
          lcd.setCursor(11,0);
          lcd.print("      ");
          lcd.setCursor(11,0);
          lcd.print(dificultyValue);
          lastDificultyValue = dificultyValue;
        }   
    }
    lcd.setCursor(10,0);
    if(lastLastDificultyValue != lastDificultyValue){
    String message = "Saved";
    lcd.print(message);
    delay(1000);
    }  
    
    xAxis2();
}


void enterSubmenu() {
  static unsigned long lastDebounceTime = 0;
  if(enteredSubmenu == false){
  
  if (millis() - lastDebounceTime > debounceInterval) {
    switchMenu = !switchMenu;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 1){
    if (millis() - lastDebounceTime > debounceInterval) {
    pushedButton = !pushedButton;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && posMenu == 0){
    if (millis() - lastDebounceTime > debounceInterval) {
    gameStartButton = !gameStartButton;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && posMenu == 4){
    if (millis() - lastDebounceTime > debounceInterval) {
    editContrast = !editContrast;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && posMenu == 5){
    if (millis() - lastDebounceTime > debounceInterval) {
    editBrightness = !editBrightness;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && posMenu == 6){
    if (millis() - lastDebounceTime > debounceInterval) {
    editIntensity = !editIntensity;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && posMenu == 7){
    if (millis() - lastDebounceTime > debounceInterval) {
    editDificulty = !editDificulty;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && pozY == 9){
    if (millis() - lastDebounceTime > debounceInterval) {
      switchLetter1 = !switchLetter1;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && pozY == 10){
    if (millis() - lastDebounceTime > debounceInterval) {
      switchLetter2 = !switchLetter2;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && pozY == 11){
    if (millis() - lastDebounceTime > debounceInterval) {
      switchLetter3 = !switchLetter3;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && pozY == 12){
    if (millis() - lastDebounceTime > debounceInterval) {
      switchLetter4 = !switchLetter4;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && pozY == 13){
    if (millis() - lastDebounceTime > debounceInterval) {
      switchLetter5 = !switchLetter5;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && pozY == 14){
    if (millis() - lastDebounceTime > debounceInterval) {
      switchLetter6 = !switchLetter6;
    }
  }
  else if(enteredSubmenu == true && posSubmenu == 0 && pozY == 15){
    if (millis() - lastDebounceTime > debounceInterval) {
      switchLetter7 = !switchLetter7;
    }
  }
  
  lastDebounceTime = millis();
}

void xAxisLetters(){
  xValue = analogRead(pinX);
   if(xValue > maxThreshold && joyMoved1 == false){
    joyMoved1 = true;
    letter++;
    if(letter > 'z'){
      letter = 'a';
    }

    nameLetters[posLetter] = letter;
  }
  
  if(xValue < minThreshold && joyMoved1 == false){
    joyMoved1 = true;
    letter--;
    
    if(letter < 'a'){
      letter = 'z';
    }

    nameLetters[posLetter] = letter;
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMoved1 = false;
  }
}

void yAxis(){
  lcd.setCursor(pozY,0);
  lcd.blink();
  yValue = analogRead(pinY);
   if(yValue > maxThreshold && joyMoved == false){
    joyMoved = true;
    pozY++;
    posLetter++;
    if(pozY > 15){
      pozY = 9;
    }
    if(posLetter > 6){
      posLetter = 0;
    }
    
  }
  
  if(yValue < minThreshold && joyMoved == false){
    
    joyMoved = true;
    pozY--;
    posLetter--;
    if(pozY < 9){
      pozY = 15;
    }
    if(posLetter < 0){
      posLetter = 6;
    }
  }

  if(yValue > minThreshold && yValue < maxThreshold){
    joyMoved = false;
  }
}

void showMenu(){

  if(posMenu < 3 && posMenu % 2 == 0){
    lcd.setCursor(0,0);
    lcd.print(content[posMenu]);
    lcd.setCursor(0,1);
    lcd.print(content[posMenu+1]);
  }
  if(posMenu < 7 && joyMoved && posMenu % 2 == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(content[posMenu]);
    lcd.setCursor(0,1);
    lcd.print(content[posMenu+1]);
  }
}

void showSubmenu(){
  lcd.clear();
    if(posMenu == 4){  
      setContrast();
    }
}
void xAxisContrast(){
  xValue = analogRead(pinX);
   if(xValue > maxThreshold && joyMoved1 == false){
    joyMoved1 = true;
    contrastValue += 5;
    if(contrastValue > 255){
      contrastValue = 255;
    }
  }
  
  if(xValue < minThreshold && joyMoved1 == false){
    joyMoved1 = true;
    contrastValue -= 5;
    
    if(contrastValue < 0){
      contrastValue = 0;
    }
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMoved1 = false;
  }
}

int posDificulty = 0;

void xAxisDificulty(){
  
  xValue = analogRead(pinX);
   if(xValue > maxThreshold && joyMoved1 == false){
    joyMoved1 = true;
    posDificulty += 1;
    if(posDificulty > 2){
      posDificulty = 0;
    }
  }
  
  if(xValue < minThreshold && joyMoved1 == false){
    joyMoved1 = true;
    posDificulty -= 1;
    
    if(posDificulty < 0){
      posDificulty = 2;
    }
  }

  dificultyValue = dificultyValues[posDificulty];

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMoved1 = false;
  }
}

void xAxisBrightness(){
  xValue = analogRead(pinX);
   if(xValue > maxThreshold && joyMoved1 == false){
    joyMoved1 = true;
    brightnessValue += 5;
    if(brightnessValue > 255){
      brightnessValue = 255;
    }
  }
  
  if(xValue < minThreshold && joyMoved1 == false){
    joyMoved1 = true;
    brightnessValue -= 5;
    
    if(brightnessValue < 0){
      brightnessValue = 0;
    }
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMoved1 = false;
  }
}

void xAxisIntensity(){
  xValue = analogRead(pinX);
   if(xValue > maxThreshold && joyMoved1 == false){
    joyMoved1 = true;
    intensityValue += 1;
    if(intensityValue > 15){
      intensityValue = 15;
    }
  }
  
  if(xValue < minThreshold && joyMoved1 == false){
    joyMoved1 = true;
    intensityValue -= 1;
    
    if(intensityValue < 0){
      intensityValue = 0;
    }
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMoved1 = false;
  }
}

void xAxis1(){

  lcd.setCursor(0,posMenu%2);
  lcd.blink();
  xValue = analogRead(pinX);
   if(xValue > maxThreshold && joyMoved == false){
    joyMoved = true;
    posMenu--;
    if(posMenu < 0){
      posMenu = 7;
    }
  }
 
  if(xValue < minThreshold && joyMoved == false){
    joyMoved = true;
    posMenu++;
    if(posMenu > 7){
      posMenu = 0;
    }
  }

  if(posMenu >= 2 && posMenu <= 3) {
    pageNumberMenu = 2;
  }
  if(posMenu >= 0 && posMenu <= 1) {
    pageNumberMenu = 1;
  }
 

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMoved = false;
  }
}


void xAxis2(){
  
  lcd.setCursor(0,posSubmenu%2);
  lcd.blink();
  xValue = analogRead(pinX);
   if(xValue > maxThreshold && joyMoved == false){
    joyMoved = true;
    posSubmenu--;
    if(posSubmenu < 0){
      posSubmenu = 0;
    }
  }
  
  if(xValue < minThreshold && joyMoved == false){
    joyMoved = true;
    posSubmenu++;
    if(posSubmenu > 1){
      posSubmenu = 1;
    }
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMoved = false;
  }
}

void readNameFromMemory() {
  for(int i = 10; i < 17; i++) {
    namePlayer += char(EEPROM.read(i));
  }
}

void updateNameInMemory() {
  for(int i = 0, j = 10; i < lenName, j < 17; i++,j++){
    EEPROM.update(nameLetters[i],j);
  }
}
