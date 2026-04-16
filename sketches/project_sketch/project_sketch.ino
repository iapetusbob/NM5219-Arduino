// the repo is available at https://github.com/iapetusbob/NM5219-Arduino

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Game messages
const char* GAME_START1 = " Kenangan";
const char* GAME_START2 = "     Singapura!";
const char* GAME_END1 = "   Game Ended.  ";
const char* GAME_END2 = "Score: ";
const char* CONFIRMATION = "Confirm | Cancel";
const char* MARBLES_SCORED = "Points: ";
const char* GAME_END_CONFIRMATION = "   End Game?    ";
const char* OBJECTIVE = "Color: ";

const char* COLORS[] = {"RED", "BLUE", "GREEN", "YELLOW", "CLEAR"};
int colorPool[] = {0, 1, 2, 3}; 
int stepInCycle = 0; 

const uint8_t PIN_UP = A0;
const uint8_t PIN_DOWN = A1;
const uint8_t PIN_CONFIRM = A2;
const uint8_t PIN_CANCEL = A3;

const uint8_t POWER_PIN = 50; // Defining Pin 50 as our power source

uint8_t marbles_scored = 0;
int total_score = 0;
int colorIdx = 4; // Start on CLEAR

int debounceDelay = 50;
int lastDebounceTime = 0;

char getButton() {
  while (true) {
    // Serial.print(analogRead(PIN_UP)); Serial.print(" UP ");
    // Serial.print(analogRead(PIN_DOWN)); Serial.print(" DOWN ");
    // Serial.print(analogRead(PIN_CONFIRM)); Serial.print(" ENTER ");
    // Serial.print(analogRead(PIN_CANCEL)); Serial.println(" CANCEL ");

    if (analogRead(PIN_UP) > 900) { delay(50); while(analogRead(PIN_UP) > 900); return 'U'; }
    if (analogRead(PIN_DOWN) > 900) { delay(50); while(analogRead(PIN_DOWN) > 900); return 'D'; }
    if (analogRead(PIN_CONFIRM) > 900) { delay(50); while(analogRead(PIN_CONFIRM) > 900); return 'E'; }
    if (analogRead(PIN_CANCEL) > 900) { delay(50); while(analogRead(PIN_CANCEL) > 900); return 'C'; }
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

  if (millis() - lastDebounceTime > debounceDelay) {

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
  lastDebounceTime = millis();
}