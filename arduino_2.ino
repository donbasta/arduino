#include <LiquidCrystal.h>

//char bufferToA1[5] = "1";
char bufferFromA1[6] = "0000";
char bufferlul[6];
const char peopleExit[6] = "decrA";

const int kPinPIR = 13;

int speedDC = 0;

int bufferFromA1ToInt() {
  if (bufferlul[4] == 'A') {
  	return speedDC;
  }
	int ret = 0;
  	int pw = 1;
  	for (int i = 0; i < 4; i++) {
        int dig = (int) (bufferlul[i] - '0');
        //Serial.println(dig);
  		ret = ret + pw * dig;
    	pw = pw * 10;
  	}
  	return ret;
}

void setup() {
    Serial.begin(9600);         
}

void sendToA1() {
    Serial.write(peopleExit, 5);
}

void recvFromA1() {
	Serial.readBytes(bufferlul, 5);
    speedDC = bufferFromA1ToInt();
  	//Serial.print("current buffer: ");
    //Serial.println(bufferlul);
    //Serial.print("current retrieved speed: ");
    //Serial.println(speedDC);
}

void loop() {
  recvFromA1();
  if (digitalRead(kPinPIR) == HIGH) {
    //Serial.println("tesuto");
    sendToA1();
  }
  delay(3000);
}

