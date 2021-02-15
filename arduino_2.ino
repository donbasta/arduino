#include <LiquidCrystal.h>

char bufferA1[5] = "1";

void setup()
{
    pinMode(backLight, OUTPUT);
    digitalWrite(backLight, HIGH);
    lcd.begin(16,2);             
    lcd.clear();    
    Serial.begin(9600);         
}

void sendToA1() {
    Serial.write(bufferA1, 1);
}

void loop()
{


}

