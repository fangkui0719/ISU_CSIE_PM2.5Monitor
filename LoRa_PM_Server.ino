#include <SPI.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4);

// Singleton instance of the radio driver
RH_RF95 rf95;
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
//RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95 

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

int led = 13;
int count = 1;

void setup() 
{
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
  mySerial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
  // Failure to do that will result in extremely low transmit powers.
  // driver.setTxPower(14, true);
}

void loop()
{
  char data[50];
  
  // Send a message to rf95_server
  sprintf(data, "%d", count);
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();

  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  Serial.print("Send to ");
  Serial.print(count);
  Serial.print(" -----> ");
    if (rf95.waitAvailableTimeout(3000))
    { 
      // Should be a reply message for us now
      // get message : station_number, pm, temp, hum
      if (rf95.recv(buf, &len))
     {
        digitalWrite(led, HIGH);
        
        sprintf(data, "01,0%s\n", buf);
        mySerial.print(data);
        Serial.print("Send to LinkIt: ");
        Serial.println(data);
        sprintf(data, "%s", buf);
        Serial.print("Station");
        Serial.print(count);
        Serial.print(", ");
        Serial.print("Get Data: ");
        Serial.println(data);
        delay(5000); 
      }
      else
      {
        Serial.println("Recv Failed");       
        sprintf(data, "0,%d\n", count);
        //mySerial.print(data);
        Serial.print("Send to LinkIt: ");
        Serial.println(data);
      }
    }
    else
    {
      Serial.println("No Reply");
      
      sprintf(data, "0,%d\n", count);
      
      //mySerial.print(data);
      Serial.print("Send to LinkIt: ");
      Serial.println(data);
      //delay(5000);
    }
    count++;
    if(count==4){
      count = 1;
    }
}

