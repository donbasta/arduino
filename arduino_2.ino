char bufferFromA1[6];
const char peopleExit[6] = "decrA";

const int kPinPIR = 13;

int speedDC = 100;

const int kPinDC = 10;

int bufferFromA1ToInt() {
    if (bufferFromA1[4] != 'B') {
        return speedDC;
    }
	int ret = 0;
  	int pw = 1;
  	for (int i = 0; i < 4; i++) {
        int dig = (int) (bufferFromA1[i] - '0');
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
	Serial.readBytes(bufferFromA1, 5);
    speedDC = bufferFromA1ToInt();
}

void loop() {
    recvFromA1();
    if (digitalRead(kPinPIR) == HIGH) {
        analogWrite(kPinDC, speedDC);
        delay(1000);
        analogWrite(kPinDC, LOW);
        while (digitalRead(kPinPIR) == HIGH);
        sendToA1();
    }
    delay(500);
}