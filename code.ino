// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Include keypad code:
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {A5, A4, A3, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 6, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//IR variables
int ir_s = 3;
int ir_t = 4;

//Variables
String mode;
int number = 0;
int count = 0;
int total = 0;
String action;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Start");
  delay(400);
  lcd.clear();

  // Setup indicator LEDs
  pinMode(A0, OUTPUT); //Start counting
  pinMode(A1, OUTPUT); //Start circuit

  //IR sensors
  pinMode(ir_s, INPUT);
  pinMode(ir_t, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(A1, HIGH);
  while (true) {
    lcd.print("* Small objects");
    lcd.setCursor(0, 1);
    lcd.print("# Tall objects  ");
    char key1 = keypad.getKey();
    if (key1 == '*' || key1 == '#') {
      Serial.println(key1);
      if (key1 == '*') {
        mode = "small";
        lcd.clear();
        lcd.print("Small: ");
      }
      else if (key1 == '#') {
        mode = "tall";
        lcd.clear();
        lcd.print("Tall: ");
      }
      break;
    }
  }
  Serial.print("Selected"); Serial.println(mode);

  while (true) {
    char key2 = keypad.getKey();
    if (key2 == '1' || key2 == '2' || key2 == '3' || key2 == '4' || key2 == '5' || key2 == '6' || key2 == '7' || key2 == '8' || key2 == '9' || key2 == '0') {
      Serial.println(key2);
      lcd.print(key2);
      if (key2 == '1') {
        number = 1;
      }
      else if (key2 == '2') {
        number = 2;
      }
      else if (key2 == '3') {
        number = 3;
      }
      else if (key2 == '4') {
        number = 4;
      }
      else if (key2 == '5') {
        number = 5;
      }
      else if (key2 == '6') {
        number = 6;
      }
      else if (key2 == '7') {
        number = 7;
      }
      else if (key2 == '8') {
        number = 8;
      }
      else if (key2 == '9') {
        number = 9;
      }
      else if (key2 == '0') {
        number = 0;
      }
      break;
    }
  }

  while (true) {
    char key3 = keypad.getKey();
    if (key3 == '*' || key3 == '#') {
      Serial.println("select action");
      lcd.setCursor(0, 1);
      if (key3 == '#') {
        action = "C";
        lcd.print("Start counting");
        delay(500);
      }
      else if (key3 == '*') {
        action = "B";
        lcd.print("Back");
      }
      delay(500);
      break;
    }
  }
  Serial.println(action);
  /*
    while (true) {
      if (action == "B") {
        lcd.clear();
        break;
      }
    }*/
  // Small counting
  if (action == "C") {
    lcd.clear();
    if (mode == "small") {
      lcd.setCursor(0, 0);
      lcd.print("Small count: ");
      lcd.print(count);
      delay(1000);
      while (count < number) {
        digitalWrite(A0, HIGH); //On sorting LED
        int small_ir = digitalRead(ir_s);
        int tall_ir = digitalRead(ir_t);
        if (small_ir == 1 and tall_ir == 0) {
          count++;
          lcd.clear(); lcd.setCursor(0, 0);
          lcd.print("Small count: "); lcd.print(count);
          lcd.setCursor(0, 1);
          int boxes = ((count * 100) / number);
          lcd.print(boxes); lcd.print("% found");
          delay(1000);
        }
        if (small_ir == 1) {
          total++;
          delay(1000);
        }
      }
      if (count == number) {
        digitalWrite(A0, LOW);

        lcd.clear();
        lcd.print("Completed..!");
        lcd.setCursor(0, 1);
        int result = ((number * 100) / total);
        lcd.print(result); lcd.print("%  of Total");
        delay(1000000);
      }
    }
    // Sorting tall objects
    else if (mode == "tall") {
      lcd.setCursor(0, 0);
      lcd.print("Tall count: ");
      lcd.print(count);
      delay(1000);
      while (count < number) {
        digitalWrite(A0, HIGH); //On sorting LED
        int small_ir = digitalRead(ir_s);
        int tall_ir = digitalRead(ir_t);
        if (small_ir == 1 and tall_ir == 1) {
          count++;
          lcd.clear(); lcd.setCursor(0, 0);
          lcd.print("Tall count: "); lcd.print(count);
          lcd.setCursor(0, 1);
          int boxes = ((count * 100) / number);
          lcd.print(boxes); lcd.print("% found");
          delay(1000);
        }
        if (small_ir == 1) {
          total++;
          delay(1000);
        }
      }
      if (count == number) {
        digitalWrite(A0, LOW);

        lcd.clear();
        lcd.print("Completed..!");
        lcd.setCursor(0, 1);
        int result = ((number * 100) / total);
        lcd.print(result); lcd.print("%  of Total");
        delay(1000000);
      }
    }
  }
}
