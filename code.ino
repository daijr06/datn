#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo servo1, servo2, servo3, servo4;

#define servoPin1  4
#define servoPin2  5
#define servoPin3  6
#define servoPin4  7
#define ENB  11
#define reset 8

const int CBBT = 2;
const int IN3 = 12;
const int IN4 = 10;
int vitrigoc[]  = {25, 13, 32};
int vitricho[]  = {25, 62, 105};
int vitricho2[] = {110, 62, 105};
int vitriGap[]  = {25, 89, 129};
int vitriA[] = {116, 86, 107};
int vitriB[] = {106, 86, 107};
int vitriC[] = {106, 77, 85};
int vitriD[] = {116, 77, 85};

bool isrunning = false;
int ttcbbt = 0, demsp = 0 , demttcb = 0;
int gtreset ;

void setup() {
  pinMode(reset , INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.clear(); 
  servo1.attach(servoPin1,500,2500);
  servo2.attach(servoPin2,500,2500);
  servo3.attach(servoPin3,500,2500);
  servo4.attach(servoPin4,500,2500);
  pinMode(CBBT, INPUT);
  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  moveToPosition(vitrigoc, "GOC");
  servo4.write(40);
}

void loop() {
  gtreset = digitalRead(reset);
  if(gtreset == 0)
  {
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("RESET.......");
    delay(1000);
    resetBoard();
   }
  ttcbbt = digitalRead(CBBT);

  if (ttcbbt == LOW && !isrunning) {
    isrunning = true;
    demsp += 1;
    demttcb = 1;
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
  
  if (ttcbbt == HIGH && isrunning) {
    isrunning = false;
  }
  
  if (demsp > 0 && isrunning) {
    executeMove(demsp);
    if (demsp == 4){
      demsp = 0;  // Reset count after D position
      moveToPosition(vitrigoc, "GOC");
    } 
    demttcb = 0;
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 255);
  }
}

void updateLCD(int count, String position) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SP So");
  lcd.setCursor(2, 1);
  lcd.print(count);
  lcd.setCursor(9, 0);
  lcd.print("Vi Tri");
  lcd.setCursor(11, 1);
  lcd.print(position);
}

void executeMove(int step) {
  switch (step) {
    case 1:
      performPickAndPlace(vitriGap, vitricho, vitriA, vitricho2, "A");
      break;
    case 2:
      performPickAndPlace(vitriGap, vitricho, vitriB,vitricho2, "B");
      break;
    case 3:
      performPickAndPlace(vitriGap, vitricho, vitriC,vitricho2, "C");
      break;
    case 4:
      performPickAndPlace(vitriGap, vitricho, vitriD,vitricho2, "D");
      break;
  }
}

void performPickAndPlace(int pickPos[], int waitPos1[], int placePos[], int waitPos2[], String posName) {
  moveToPosition(pickPos, "Gap");
  delay(500);
  servo4.write(90);  // Close gripper
  delay(1000);
  
  moveToPosition(waitPos1, posName);
  
  moveToPosition(waitPos2, posName);
  
  moveToPosition(placePos, posName);
  delay(500);
  servo4.write(32);    // Open gripper
  delay(1000);

  moveToPosition(waitPos2, "Cho");

  moveToPosition(waitPos1, "Cho");
  //moveToPosition(vitrigoc, "GOC"); // Return to home position
}

void moveToPosition(int pos[], String position) {
  int currentPos1 = servo1.read();
  int currentPos2 = servo2.read();
  int currentPos3 = servo3.read();

  // Cập nhật LCD khi bắt đầu chuyển động
  updateLCD(demsp, position);

  for (int i = 0; i <= 100; i++) {
    int nextPos1 = currentPos1 + (pos[0] - currentPos1) * i / 100;
    int nextPos2 = currentPos2 + (pos[1] - currentPos2) * i / 100;
    int nextPos3 = currentPos3 + (pos[2] - currentPos3) * i / 100;

    servo1.write(nextPos1);
    servo2.write(nextPos2);
    servo3.write(nextPos3);

    // Chỉ cập nhật LCD ở các điểm chính
    if (i == 50  || i == 100) {
      updateLCD(demsp, position);
    }
    delay(10);
  }
}
void resetBoard()
{
  asm volatile( "jmp 0" );
}

