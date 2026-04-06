#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Game messages
const char* GAME_START1 = "    Welcome!    ";
const char* GAME_START2 = "ModifiedCangkok!";
const char* GAME_END1 = "   Game Ended.  ";
const char* GAME_END2 = "Score: ";
const char* CONFIRMATION = " OK: ✓ | NO: X ";
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

const uint8_t POWER_PIN = 50; // Defining Pin 50 as our power source

uint8_t marbles_scored = 0;
int total_score = 0;
int colorIdx = 4; // Start on CLEAR

char getButton() {
  while (true) {
    if (digitalRead(PIN_UP) == HIGH) { delay(50); while(digitalRead(PIN_UP) == HIGH); return 'U'; }
    if (digitalRead(PIN_DOWN) == HIGH) { delay(50); while(digitalRead(PIN_DOWN) == HIGH); return 'D'; }
    if (digitalRead(PIN_CONFIRM) == HIGH) { delay(50); while(digitalRead(PIN_CONFIRM) == HIGH); return 'E'; }
    if (digitalRead(PIN_CANCEL) == HIGH) { delay(50); while(digitalRead(PIN_CANCEL) == HIGH); return 'C'; }
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
  Serial.begin(9600);
  
  pinMode(POWER_PIN, OUTPUT);   // Set Pin 50 as an output
  digitalWrite(POWER_PIN, HIGH); // Send 5V to the pin constanty

  pinMode(PIN_UP, INPUT);
  pinMode(PIN_DOWN, INPUT);
  pinMode(PIN_CONFIRM, INPUT);
  pinMode(PIN_CANCEL, INPUT);

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

    while (!confirmed) {
      lcd.clear();
      lcd.print("Marbles: "); lcd.print(marbles_scored);
      lcd.setCursor(0, 1); lcd.print("Up/Dn or Enter");

      char countAction = getButton();
      if (countAction == 'U' && marbles_scored < 9) marbles_scored++;
      if (countAction == 'D' && marbles_scored > 0) marbles_scored--;

      if (countAction == 'C') {
        break;              // Exits the "while(!confirmed)" loop
      }
      
      if (countAction == 'E') {
        lcd.clear();
        lcd.print(MARBLES_SCORED); lcd.print(marbles_scored*2);
        lcd.setCursor(0, 1); lcd.print(CONFIRMATION);
        
        if (getButton() == 'E') {
          total_score += marbles_scored*2;
          confirmed = true; 
          marbles_scored = 0;

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