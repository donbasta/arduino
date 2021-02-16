#include <LiquidCrystal.h>

#define SIAP_BUKA 1
#define TIDAK_SIAP_BUKA 0
#define MAX_ALLOWED_TEMPERATURE 37
#define MAX_ALLOWED_PEOPLE 10
#define DISTANCE_THRESHOLD 100

const int kRS = 12;
const int kRW = 11;
const int kEnable = 8;
const int kLCD = 13;
const int kD4 = 5;
const int kD5 = 4;
const int kD6 = 3;
const int kD7 = 2;

const int trigpin = 7;
const int echopin = 6;

const int kPinSpeaker = 9;

const int kPinDC = 10;
const int kPinPotentiometer = A1;

LiquidCrystal lcd(kRS, kRW, kEnable, kD4, kD5, kD6, kD7);

const int kPinTemp = A0;
const int backLight = 13;

const int kPinPIR = A2;

int currentNumberOfPeople = 0;
int statusPintu = TIDAK_SIAP_BUKA;
char bufferToA2[6];
char bufferFromA2[6];

const char msg[6] = "decrA";

int adaOrangDiDepanPintu = 0; // toggled by rangefinder/ultrasonic sensor
int pintuDibuka = 0; // toggled by motor DC sensor (?)

void setup()
{
  	pinMode(kPinSpeaker, OUTPUT);
  	pinMode(trigpin, OUTPUT);
  	pinMode(echopin, INPUT);
    pinMode(backLight, OUTPUT);
    digitalWrite(backLight, HIGH);
    lcd.begin(16,2);             
    lcd.clear();    
    Serial.begin(9600);         
}

void setTemperatureToLCD(float temp) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Suhu anda:");
    lcd.print(temp);
    lcd.print("C");
    lcd.setCursor(0,1);
  if (temp <= MAX_ALLOWED_TEMPERATURE) {
  	lcd.print("Suhu aman!");
  } else {
    lcd.print("Tidak boleh!");
  }
}

void printPenuhToLCD() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Penuh");
}

void LCDPrintJagaJarak() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tunggu!");
    lcd.setCursor(0,1);
    lcd.print("Jaga jarak");
}

void LCDPrintSilakanCekSuhu() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Silakan Maju,");
    lcd.setCursor(0,1);
    lcd.print("dan cek suhu");
}

float getTemperatureC() {
    int reading = analogRead(kPinTemp);
    float voltage = (reading * 5.0) / 1024;
    return (voltage - 0.5) * 100;
}

void recvFromA2() {
    Serial.readBytes(bufferFromA2, 5);
  	if (strcmp(msg, bufferFromA2) == 0) {
      if (currentNumberOfPeople > 0) {
        currentNumberOfPeople -= 1;
      }
      for (int i = 0; i < 5; i++) {
        bufferFromA2[i] = '0';
      }
  	}
}

void sendToA2(int a) {
  	int idx = 0;
    while (idx < 4) {
      bufferToA2[idx] = ('0' + (a % 10));
      idx++;
      a = a / 10;
    }
  	bufferToA2[idx] = 'B';
    //Serial.println(bufferToA2);
	Serial.write(bufferToA2,5);
}

void LCDPrintCapacity(int currentNum) {
    int cap = MAX_ALLOWED_PEOPLE - currentNum;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Kapasitas: ");
    lcd.print(cap);
    lcd.setCursor(0,1);
    if (cap > 0) {
      lcd.print("Welcome!");
    } else {
    	lcd.print("!!Penuh!!");
    }
}

void buzzerOn(int k_timeDelay) {
	tone(kPinSpeaker, 440, k_timeDelay);
  delay(100);
}

int readDCSpeedFromPotentiometer () {
	return map(analogRead(kPinPotentiometer), 0, 1023, 0, 255);
}

int getDistanceObjectInCm() {
    digitalWrite(trigpin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);
  
    long tmp = pulseIn(echopin, HIGH) * 1723 / 100000;
    return tmp;
}

int DCSpeed = 0;

void loop()
{
  	recvFromA2();	
  
  	DCSpeed = readDCSpeedFromPotentiometer();
  	//sendToA2(DCSpeed);
  	adaOrangDiDepanPintu = 0;

    long cm = getDistanceObjectInCm();
    if (cm < DISTANCE_THRESHOLD) {
        adaOrangDiDepanPintu = 1;
    }

    // detect apakah ada orang di depan pintu, pake ultrasonic sensor
    if (adaOrangDiDepanPintu) {
        float temperatureC = getTemperatureC();
        setTemperatureToLCD(temperatureC);
        if (temperatureC <= MAX_ALLOWED_TEMPERATURE && currentNumberOfPeople < MAX_ALLOWED_PEOPLE) {
            statusPintu = SIAP_BUKA;
            delay(100);
        } else {
        	buzzerOn(100);
        }
      	
      if(statusPintu == SIAP_BUKA) {
      	int sensorState = LOW;
        while (sensorState == LOW) {
          delay(100);
          sensorState = digitalRead(kPinPIR);
        }
        if (sensorState == HIGH && currentNumberOfPeople < MAX_ALLOWED_PEOPLE) {
            analogWrite(kPinDC, DCSpeed);  
          	statusPintu = TIDAK_SIAP_BUKA;
          	currentNumberOfPeople += 1;
            if (currentNumberOfPeople == MAX_ALLOWED_PEOPLE) {
                printPenuhToLCD();
            }
            delay(100);
            analogWrite(kPinDC, LOW);
        }
      }
      
        // detect gerakan dari pintu, DC Motor or apapun
		//int sensorState = digitalRead(kPinPIR);
        //if (sensorState == HIGH && statusPintu == SIAP_BUKA && currentNumberOfPeople < MAX_ALLOWED_PEOPLE) {
        //  	//int speedDC = readDCSpeedFromPotentiometer();
        //	analogWrite(kPinDC, DCSpeed);  
        //  	statusPintu = TIDAK_SIAP_BUKA;
        //  	currentNumberOfPeople += 1;
        //    if (currentNumberOfPeople == MAX_ALLOWED_PEOPLE) {
        //        printPenuhToLCD();
        //    }
        //    delay(100);
        //    analogWrite(kPinDC, LOW);
        //}
        LCDPrintJagaJarak();
        delay(300);
        LCDPrintSilakanCekSuhu();
        delay(50);
    }
    LCDPrintCapacity(currentNumberOfPeople);
}
