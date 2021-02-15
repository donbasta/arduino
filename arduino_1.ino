#include <LiquidCrystal.h>

#define SIAP_BUKA 1
#define TIDAK_SIAP_BUKA 0
#define MAX_ALLOWED_TEMPERATURE 37
#define MAX_ALLOWED_PEOPLE 10

const int kRS = 12;
const int kRW = 11;
const int kEnable = 10;
const int kLCD = 13;
const int kD4 = 5;
const int kD5 = 4;
const int kD6 = 3;
const int kD7 = 2;

LiquidCrystal lcd(kRS, kRW, kEnable, kD4, kD5, kD6, kD7);

const int kPinTemp = A0;
int backLight = 13;

int currentNumberOfPeople = 0;
int statusPintu = TIDAK_SIAP_BUKA;
char bufferA2[5];

void setup()
{
    pinMode(backLight, OUTPUT);
    digitalWrite(backLight, HIGH);
    lcd.begin(16,2);             
    lcd.clear();    
    Serial.begin(9600);         
}

void setTemperatureToLCD(float temp) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(temp);
    lcd.print("C");
    lcd.setCursor(0,1);
    delay(1000);
}

void printPenuhToLCD() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Penuh");
    lcd.setCursor(0,1);
}

float getTemperatureC() {
    int reading = analogRead(kPinTemp);
    float voltage = (reading * 5.0) / 1024;
    return (voltage - 0.5) * 100;
}

void recvFromA2() {
    Serial.readBytes(bufferA2, 1);
    if (bufferA2[0] == '1') {
        assert (currentNumberOfPeople);
        currentNumberOfPeople -= 1;
    }
}

int adaOrangDiDepanPintu = 0; // toggled by rangefinder/ultrasonic sensor
int pintuDibuka = 0; // toggled by motor DC sensor (?)

void loop()
{

    recvFromA2();

    // detect apakah ada orang di depan pintu, pake ultrasonic sensor
    if (adaOrangDiDepanPintu) {
        float temperatureC = getTemperatureC();
        setTemperatureToLCD(temperatureC);
        if (temperatureC <= MAX_ALLOWED_TEMPERATURE) {
            statusPintu = SIAP_BUKA;
        }
        // detect gerakan dari pintu, DC Motor or apapun
        if (pintuDibuka && currentNumberOfPeople < MAX_ALLOWED_PEOPLE) {
            currentNumberOfPeople += 1;
            if (currentNumberOfPeople == MAX_ALLOWED_PEOPLE) {
                printPenuhToLCD();
            }
        }
    }
}

