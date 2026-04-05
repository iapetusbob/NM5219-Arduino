#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Setting up the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Game messages (Fixed syntax and added padding for centering)
const char* GAME_START1 = "    Welcome!    ";
const char* GAME_START2 = "ModifiedCangkok!";
const char* GAME_END1 = "   Game Ended.  ";
const char* GAME_END2 = "Total Points: ";
const char* CONFIRMATION = "  ✓ | ✗  ";
const char* MARBLES_SCORED = "Points: ";
const char* GAME_END_CONFIRMATION = "   End Game?    ";
const char* OBJECTIVE = "Color this turn:";

const char* COLORS[] = {"RED", "BLUE", "GREEN", "YELLOW", "CLEAR"};

// Pin numbers
const uint8_t PIN_UP = 2;
const uint8_t PIN_DOWN = 3;
const uint8_t PIN_CONFIRM = 4;
const uint8_t PIN_CANCEL = 5;

// Variables
uint8_t marbles_scored = 0;
int total_score = 0;
int colorIdx = 0;

// Helper to wait for a button press and return which one
char getButton() {
  while (true) {
    if (digitalRead(PIN_UP) == LOW) { delay(200); while(digitalRead(PIN_UP) == LOW); return 'U'; }
    if (digitalRead(PIN_DOWN) == LOW) { delay(200); while(digitalRead(PIN_DOWN) == LOW); return 'D'; }
    if (digitalRead(PIN_CONFIRM) == LOW) { delay(200); while(digitalRead(PIN_CONFIRM) == LOW); return 'E'; }
    if (digitalRead(PIN_CANCEL) == LOW) { delay(200); while(digitalRead(PIN_CANCEL) == LOW); return 'C'; }
  }
}

void setup() {
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_CONFIRM, INPUT_PULLUP);
  pinMode(PIN_CANCEL, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  
  // Initial Welcome
  lcd.setCursor(0, 0); lcd.print(GAME_START1);
  lcd.setCursor(0, 1); lcd.print(GAME_START2);
  getButton(); // Press any button to start
}

void loop() {
  // 1. Show the Color Objective
  lcd.clear();
  lcd.print(OBJECTIVE);
  lcd.setCursor(0, 1);
  lcd.print(COLORS[colorIdx]);

  char action = getButton();

  // 2. Logic for Enter (Start counting marbles) or Cancel (Check if game over)
  if (action == 'E') {
    bool confirmed = false;
    marbles_scored = 0;

    while (!confirmed) {
      // Counting Sub-menu
      lcd.clear();
      lcd.print("Marbles: "); lcd.print(marbles_scored);
      lcd.setCursor(0, 1); lcd.print("Up/Dn or Enter");

      char countAction = getButton();
      if (countAction == 'U') marbles_scored++;
      if (countAction == 'D' && marbles_scored > 0) marbles_scored--;
      
      if (countAction == 'E') {
        // Confirmation Sub-menu
        lcd.clear();
        lcd.print(MARBLES_SCORED); lcd.print(marbles_scored);
        lcd.setCursor(0, 1); lcd.print(CONFIRMATION);
        
        if (getButton() == 'E') {
          total_score += marbles_scored;
          colorIdx = (colorIdx + 1) % 5; // Move to next color
          confirmed = true; 
        } 
        // If 'Cancel' is pressed here, 'confirmed' remains false and it loops back to Counting
      }
    }
  } 
  
  else if (action == 'C') {
    // End Game Confirmation logic
    lcd.clear();
    lcd.print(GAME_END_CONFIRMATION);
    lcd.setCursor(0, 1); lcd.print(CONFIRMATION);

    if (getButton() == 'E') {
      // Final Score Screen
      lcd.clear();
      lcd.print(GAME_END1);
      lcd.setCursor(0, 1);
      lcd.print(GAME_END2); lcd.print(total_score);
      
      getButton(); // Wait for press to reset
      total_score = 0;
      colorIdx = 0;
    }
    // If Cancel is pressed, it just exits this 'if' and returns to the current objective
  }
}