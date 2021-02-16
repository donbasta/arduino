#include <LiquidCrystal.h>

char bufferToA1[5] = "1";
char bufferFromA1[5] = "0000";
char bufferlul[5];

int speedDC = 0;

int bufferFromA1ToInt() {
	int ret = 0;
  	int pw = 1;
  	for (int i = 0; i < 4; i++) {
        int dig = (int) (bufferlul[i] - '0');
        Serial.println(dig);
  		ret = ret + pw * dig;
    	pw = pw * 10;
  	}
  	return ret;
}

void setup() {
    Serial.begin(9600);         
}

void sendToA1() {
    Serial.write(bufferToA1, 1);
}

void recvFromA1() {
	Serial.readBytes(bufferlul, 4);
    speedDC = bufferFromA1ToInt();
  	Serial.print("current buffer: ");
    Serial.println(bufferlul);
    Serial.print("current retrieved speed: ");
    Serial.println(speedDC);
}

void loop() {
  delay(1000);
	recvFromA1();

}

