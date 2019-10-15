/***** LCD Setting *****/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/***** rf95 *****/
#include <SPI.h>
#include <RH_RF95.h>

/***** dht11 *****/
#include <dht.h>
#define dht_apin A2 // Analog Pin sensor is connected to
dht DHT;

// Singleton instance of the radio driver
RH_RF95 rf95;
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
//RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95 

/***** Sensor - Sharp PM2.5 *****/
int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int led = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2
  
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
  
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

int str2int = 0;
String str = "";

int temp = 0;
int hum = 0;

  
void setup(){
    Serial.begin(9600);
    pinMode(led,OUTPUT);
    // rf 95
    while (!Serial) ; // Wait for serial port to be available
    if (!rf95.init()){
      Serial.println("init failed");
    }     

    rf95.setTxPower(23, false);
        
    lcd.init();
    // Print a message to the LCD.
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Station 2");
    //lcd.clear();
    lcd.home();
}
  
void loop(){
    //digitalWrite(ledPower,LOW); // power on the LED
    //delayMicroseconds(samplingTime);
    
    voMeasured = analogRead(measurePin); // read the dust value
    
    //delayMicroseconds(deltaTime);
    //digitalWrite(ledPower,HIGH); // turn the LED off
    //delayMicroseconds(sleepTime);
    
    // 0 - 5V mapped to 0 - 1023 integer values
    // recover voltage
    calcVoltage = voMeasured * (5.0 / 1024.0);
    
    // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
    // Chris Nafis (c) 2012
    dustDensity = 0.17 * calcVoltage;
    
    //Serial.print("Raw Signal Value (0-1023): ");
    //Serial.print(voMeasured);
    
    //Serial.print(" - Voltage: ");
    //Serial.print(calcVoltage);
    
    //Serial.print(" - Dust Density: ");
    //Serial.print(dustDensity * 1000); // 這裡將數值呈現改成較常用的單位( ug/m3 )
    //Serial.println(" ug/m3 ");
    
    int pm = dustDensity * 1000;

    DHT.read11(dht_apin);
    temp = DHT.temperature;
    hum = DHT.humidity;

    if (rf95.available())
    {
      // Now wait for a reply
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (rf95.recv(buf, &len))
      {
          Serial.print("Receive: ");
          Serial.println((char*)buf);
          char data[50];
          //Serial.println((char*)buf);
          str = String((char*)buf);
          str2int = str.toInt();
          if(str2int==2){
            digitalWrite(led, HIGH);
            Serial.println("Return data");
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("PM/Temp/Hum");
            lcd.setCursor(0,1);
            lcd.print(pm);
            lcd.print("/");
            lcd.print(temp);
            lcd.print("/");
            lcd.print(hum);
            delay(1000);
            sprintf(data, "2,%d,%d,%d", pm, temp, hum);
            rf95.send(data, 20);
            

            
            digitalWrite(led, LOW);
          } 
      }
      else
      {
        Serial.println("recv failed");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Recv Failed");
      }
  }
}
