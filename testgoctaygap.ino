#include <Servo.h>

Servo servo1, servo2, servo3, servo4;  // Tạo 4 đối tượng servo để điều khiển 4 servo

int potpin1 = A0;  // Chân analog cho biến trở 1
int potpin2 = A1;  // Chân analog cho biến trở 2
int potpin3 = A2;  // Chân analog cho biến trở 3
int potpin4 = A3;  // Chân analog cho biến trở 4

int val1, val2, val3, val4;  // Các biến để lưu giá trị đọc từ các biến trở

void setup() {
  servo1.attach(4,500,2500);    // servo gần servo kẹp
  servo2.attach(5,500,2500);   // servo gần servo xoay
  servo3.attach(6,500,2500);   // servo xoay
  servo4.attach(7,500,2500);   // servo kẹp
  Serial.begin(9600);
}

void loop() {
  // Đọc giá trị từ biến trở và điều chỉnh servo tương ứng
  val1 = analogRead(potpin1);
  val1 = map(val1, 0, 1023, 0, 180);
  servo1.write(val1);

  val2 = analogRead(potpin2);
  val2 = map(val2, 0, 1023, 0, 180);
  servo2.write(val2);

  val3 = analogRead(potpin3);
  val3 = map(val3, 0, 1023, 0, 180);
  servo3.write(val3);

  val4 = analogRead(potpin4);
  val4 = map(val4, 0, 1023, 0, 180);
  servo4.write(val4);
  Serial.print("Servo 1 : "); Serial.print(val1);
  Serial.print(" | Servo 2 : "); Serial.print(val2);
  Serial.print(" | Servo 3 : "); Serial.print(val3);
  Serial.print(" | Servo 4 : "); Serial.println(val4);
  delay(15);  // Chờ để servo di chuyển đến vị trí mới
}
