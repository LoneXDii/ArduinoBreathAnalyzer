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

double ConvertAlcohol(int s0_val, int s1_val) {
  int medium = (s0_val + s1_val) / 2;
  double promille = (double)medium * 0.00749805 - 1.06199;
  Serial.println(promille);
  return promille;
}

void printData(int s0_val, int s1_val) {
  display.clear();
  display.setScale(2);

  double alco = ConvertAlcohol(s0_val, s1_val);

  if(alco < 0){
    alco = 0;
  }

  display.setCursorXY(10, 20);
  display.print("Alco: ");
  display.print(alco);

  String alco_status = "";
  String driving_status = "Don't drive a car!!!";
  if(alco < 0.3){
    alco_status = "You are sober";
    driving_status = "You can drive a car";
  }
  else if (alco >= 0.3 && alco < 1){
    alco_status = "Light intoxitation";
  }
  else if (alco >= 1 && alco < 2){
    alco_status = "Medium intoxitation";
  }
  else if (alco >= 2 && alco < 3){
    alco_status = "Hard intoxitation";
  }
  else{
    alco_status = "You are dead";
  }

  display.setScale(1);
  display.setCursorXY(10, 40);
  display.print(alco_status);
  display.setCursorXY(10, 50);
  display.print(driving_status);
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

  bool err_values = errValues();

  if(err_values){
    err_code = "3";
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

bool errValues(){
  int v0 = analogRead(0);
  int v1 = analogRead(1);
  int diff = v0 - v1;

  return diff < -150 || diff > 150;
}

void printError(){
  display.setScale(1);
  display.setCursorXY(10, 0);
  display.print("Err code: ");
  display.print(err_code);
}