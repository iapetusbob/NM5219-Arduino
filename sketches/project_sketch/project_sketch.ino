#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Game messages
const char* GAME_START1 = "    Welcome!    ";
const char* GAME_START2 = "ModifiedCangkok!";
const char* GAME_END1 = "   Game Ended.  ";
const char* GAME_END2 = "Score: ";
const char* CONFIRMATION = "  OK:v | NO:x  ";
const char* MARBLES_SCORED = "Points: ";
const char* GAME_END_CONFIRMATION = "   End Game?    ";
const char* OBJECTIVE = "Color: ";

const char* COLORS[] = {"RED", "BLUE", "GREEN", "YELLOW", "CLEAR"};
int colorPool[] = {0, 1, 2, 3}; 
int stepInCycle = 0; 

const uint8_t PIN_UP = 2;
const uint8_t PIN_DOWN = 3;
const uint8_t PIN_CONFIRM = 4;
const uint8_t PIN_CANCEL = 5;

uint8_t marbles_scored = 0;
int total_score = 0;
int colorIdx = 4; // Start on CLEAR

char getButton() {
  while (true) {
    if (digitalRead(PIN_UP) == LOW) { delay(50); while(digitalRead(PIN_UP) == LOW); return 'U'; }
    if (digitalRead(PIN_DOWN) == LOW) { delay(50); while(digitalRead(PIN_DOWN) == LOW); return 'D'; }
    if (digitalRead(PIN_CONFIRM) == LOW) { delay(50); while(digitalRead(PIN_CONFIRM) == LOW); return 'E'; }
    if (digitalRead(PIN_CANCEL) == LOW) { delay(50); while(digitalRead(PIN_CANCEL) == LOW); return 'C'; }
  }
}

void shufflePool() {
  for (int i = 3; i > 0; i--) {
    int j = random(0, i + 1);
    int temp = colorPool[i];
    colorPool[i] = colorPool[j];
    colorPool[j] = temp;
  }
}

void setup() {
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_CONFIRM, INPUT_PULLUP);
  pinMode(PIN_CANCEL, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0); lcd.print(GAME_START1);
  lcd.setCursor(0, 1); lcd.print(GAME_START2);
  getButton(); 

  randomSeed(analogRead(0)); 
  shufflePool(); 
  colorIdx = 4;   
  stepInCycle = 0;
}

void loop() {
  lcd.clear();
  lcd.print(OBJECTIVE);
  lcd.setCursor(0, 1);
  lcd.print(COLORS[colorIdx]);

  char action = getButton();

  if (action == 'E') {
    bool confirmed = false;
    marbles_scored = 0;

    while (!confirmed) {
      lcd.clear();
      lcd.print("Marbles: "); lcd.print(marbles_scored);
      lcd.setCursor(0, 1); lcd.print("Up/Dn or Enter");

      char countAction = getButton();
      if (countAction == 'U' && marbles_scored < 9) marbles_scored++;
      if (countAction == 'D' && marbles_scored > 0) marbles_scored--;
      
      if (countAction == 'E') {
        lcd.clear();
        lcd.print(MARBLES_SCORED); lcd.print(marbles_scored);
        lcd.setCursor(0, 1); lcd.print(CONFIRMATION);
        
        if (getButton() == 'E') {
          total_score += marbles_scored;
          confirmed = true; 

          // Update Cycle Logic
          stepInCycle++;
          if (stepInCycle >= 5) {
            stepInCycle = 0; 
          }

          if (stepInCycle == 0) {
            colorIdx = 4; // Back to CLEAR
            shufflePool(); 
          } else {
            colorIdx = colorPool[stepInCycle - 1];
          }
        }
      }
    }
  } 
  else if (action == 'C') {
    lcd.clear();
    lcd.print(GAME_END_CONFIRMATION);
    lcd.setCursor(0, 1); lcd.print(CONFIRMATION);

    if (getButton() == 'E') {
      lcd.clear();
      lcd.print(GAME_END1);
      lcd.setCursor(0, 1);
      lcd.print(GAME_END2); lcd.print(total_score);
      
      getButton(); 
      
      // RESET EVERYTHING FOR NEW GAME
      total_score = 0;
      colorIdx = 4; 
      stepInCycle = 0;
      shufflePool();
    }
  }
}