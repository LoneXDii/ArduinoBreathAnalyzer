#include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> display;

String err_code = "";

void setup() {
  display.init(); 
  display.clear();
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(2));

  handleError();

  if (err_code != ""){
    printError();
  }

  int s0 = readAlcohol(0);
  int s1 = readAlcohol(1);
  
  printData(s0, s1);

  delay(100);
}

int readAlcohol(int pin) {
  int val = 0;
  int val1;
  int val2;
  int val3; 
 
  val1 = analogRead(pin) - 40; 
  delay(10);
  val2 = analogRead(pin) - 40; 
  delay(10);
  val3 = analogRead(pin) - 40;
 
  val = (val1+val2+val3)/3;
  return val;
}

void printData(int s0_val, int s1_val) {
  display.clear();
  display.setScale(2);

  display.setCursorXY(10, 20);
  display.print("S0: ");
  display.print(s0_val);

  display.setCursorXY(10, 45);
  display.print("S1: ");
  display.print(s1_val);
}

void handleError(){
  bool s0_noPower = noPower(0);
  bool s1_noPower = noPower(1);

  if(s0_noPower && s1_noPower){
    err_code = "1_2";
    return;
  }
  else if (s0_noPower){
    err_code = "1_0";
    return;
  }
  else if (s1_noPower){
    err_code = "1_1";
    return;
  }

  bool s0_noInput = noInput(0);
  bool s1_noInput = noInput(1);

  if(s0_noInput && s1_noInput){
    err_code = "2_2";
    return;
  }
  else if (s0_noInput){
    err_code = "2_0";
    return;
  }
  else if (s1_noInput){
    err_code = "2_1";
    return;
  }

  err_code = "";
}

bool noPower(int pin){
  int val = analogRead(pin);
  return val <= 50 ? true : false;
}

bool noInput(int pin){
  int val = analogRead(pin);
  return val >= 1020 ? true : false;
}

void printError(){
  display.setScale(1);
  display.setCursorXY(10, 0);
  display.print("Err code: ");
  display.print(err_code);
}