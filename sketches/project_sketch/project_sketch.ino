#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* LOGIC HERE
4 buttons: up, down, enter, cancel
Each round, arduino will prompt p2 the color for the round
P2 will log no. of marbles they put in their hole that round using "up" and "down", before "enter"
Arduino will prompt "Marbles scored this turn: <points> ✓/✗"
P2 will "enter" if correct, "cancel" if wrong to return to prev prompt "Marbles scored this turn: __"
To log end of game to arduino, p2 will press "cancel"
Arduino will prompt "The game has ended. ✓/✗", to which p2 can "enter" or "cancel".
Once confirmed, Arduino shows "Game ended. Total Points: <total points>".
P2 can start a new game by 
*/

// setting up the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// declare const here
// game messages to 16 chars since LCD screen only prints 16 chars
const char* GAME_START1 = "Welcome!"; // print 4 chars in
const char* GAME_START2 = "ModifiedCangkok!";
const char* GAME_END1 = "Game Ended." // print 3 chars in
const char* GAME_END2 = "TotalPoints: ";
const char* CONFIRMATION = "✓ | ✗";
const char* MARBLES_SCORED = "Points Scored: ";   // add CONFIRMATION for confirmations
const char* GAME_END_CONFIRMATION = "End Game?";  // rmb to add CONFIRMATION at the end
const char* OBJECTIVE = "Color this turn:";
const char* RED = "RED";
const char* BLUE = "BLUE";
const char* GREEN = "GREEN";
const char* YELLOW = "YELLOW";
const char* CLEAR = "CLEAR";

// confirm / cancel buttons
const bool CONFIRM = 1;
const bool CANCEL = 0;

// pin numbers
const uint8_t PIN_UP;
const uint8_t PIN_DOWN;
const uint8_t PIN_CONFIRM;
const uint8_t PIN_CANCEL;

uint8_t marbles_scored;
uint8_t total_score;


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
}