#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"
#include <Wire.h>
#include <SeeedOLED.h>
#include <string.h>

// Wi-Fi Setting
#define _SSID "kuei"
#define _KEY  "0933271647"

// MCS Setting
MCSDevice mcs("DEzf43Yr", "L7002JZvADAwiQGE");
MCSDisplayFloat s1_status("s1_status");
MCSDisplayFloat s1_pm("s1_pm");
MCSDisplayFloat s1_temp("s1_temp");
MCSDisplayFloat s1_hum("s1_hum");

MCSDisplayFloat s2_status("s2_status");
MCSDisplayFloat s2_pm("s2_pm");
MCSDisplayFloat s2_temp("s2_temp");
MCSDisplayFloat s2_hum("s2_hum");

MCSDisplayFloat s3_status("s3_status");
MCSDisplayFloat s3_pm("s3_pm");
MCSDisplayFloat s3_temp("s3_temp");
MCSDisplayFloat s3_hum("s3_hum");

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(9600);
  while(!Serial1);
  Serial1.setTimeout(30000);

  // setup Wifi connection
    while(WL_CONNECTED != WiFi.status())
    {
      Serial.print("WiFi Begin......");
      WiFi.begin(_SSID, _KEY);
    }
    Serial.println("Connected");
    Wire.begin();
    SeeedOled.init();
    SeeedOled.clearDisplay();
    SeeedOled.setNormalDisplay();
    SeeedOled.setPageMode();
    SeeedOled.setTextXY(0,0);
    SeeedOled.putString("ISU CSIE IoT");

    mcs.addChannel(s1_status);
    mcs.addChannel(s1_pm);
    mcs.addChannel(s1_temp);
    mcs.addChannel(s1_hum);

    mcs.addChannel(s2_status);
    mcs.addChannel(s2_pm);
    mcs.addChannel(s2_temp);
    mcs.addChannel(s2_hum);

    mcs.addChannel(s3_status);
    mcs.addChannel(s3_pm);
    mcs.addChannel(s3_temp);
    mcs.addChannel(s3_hum);
}

int i = 0;
int j = 0;
int a = 0;
int count = 0;
int s_count = 0;
char  incoming_byte;
char  buf[50];
char  split_status[1];
char  split_station[1];
char  split_pm[10];
char  split_temp[10];
char  split_hum[10];

String str="";

void loop() {
  if (Serial1.available() > 0) {
    // read the incoming byte:
    incoming_byte = Serial1.read();
    buf[i++] = incoming_byte;
    if(incoming_byte=='\n') { // a line is completed
      Serial.print("Get from LoRa: ");
      Serial.println(buf);

     /* while(buf[count]=='\n'){
        if(buf[count]==','){
          count++;
        }
        else{
          if(s_count==0){
            split_status[j] = buf[count];
            s_count++;
          }
          else if(s_count==1){
            split_station[j] = buf[count];
            s_count++;
          }
          else if(s_count==2){
            split_pm[j] = buf[count];
            s_count++;
          }
          else if(s_count==3){
            split_temp[j] = buf[count];
            s_count++;
          }
          else if(s_count==4){
            split_hum[j] = buf[count];
            s_count++;
          }
          j++;
          count++;
        }
      }
      str = split_status;
      Serial.println(split_status);
      str = split_station;
      Serial.println(split_station);
      str = split_pm;
      Serial.println(split_pm);
      str = split_temp;
      Serial.println(split_temp);
      str = split_hum;
      Serial.println(split_hum);
      count = 0;
      j = 0;
      s_count = 0;*/
      int *ptr=0;                  //注意星號
      ptr=str_split(buf,",");  //逗號可以任意修改成你要的形式
      // ptr[1] = status
      // ptr[2] = station number
      // ptr[3] = pm2.5
      // ptr[4] = temp
      // ptr[5] = hum
      Serial.println(ptr[0]);
      Serial.println(ptr[1]);
      Serial.println(ptr[2]);
      Serial.println(ptr[3]);
      Serial.println(ptr[4]);
      Serial.println(ptr[5]);
      // check station
      str = String(ptr[2]);
      a = str.toInt();
      i = 0;
      if(a==1){
         // check status
         str = String(ptr[1]);
         a = str.toInt();
         if(a==0){
           // no reply
           s1_status.set(0);
           SeeedOled.setTextXY(3,0);          //Set the cursor to Xth Page, Yth Column
           SeeedOled.putString("S1: Disconnect");
         }
         else{                    
           Serial.print("Upload Station 1......");
           SeeedOled.setTextXY(3,0);          //Set the cursor to Xth Page, Yth Column
           SeeedOled.putString("S1: ");
           SeeedOled.putNumber(ptr[3]); //Print the String
           SeeedOled.putString("/");
           SeeedOled.putNumber(ptr[4]); //Print the String
           SeeedOled.putString("/");
           SeeedOled.putNumber(ptr[5]); //Print the String
           //SeeedOled.putString("S1: Upload    ");   
           Serial.println("Finish");
           delay(1000);
           s1_status.set(1);
           s1_pm.set(ptr[3]);
           s1_temp.set(ptr[4]);
           s1_hum.set(ptr[5]);
         }
         strcpy(buf,"");
         delay(1000);
      }
      else if(a==2){
         // check status
         str = String(ptr[1]);
         a = str.toInt();
         if(a==0){
           // no reply
           s1_status.set(0);
           SeeedOled.setTextXY(4,0);          //Set the cursor to Xth Page, Yth Column
           SeeedOled.putString("S2: Disconnect");
         }
         else{
           Serial.print("Upload Station 2......");
         SeeedOled.setTextXY(4,0);          //Set the cursor to Xth Page, Yth Column
           SeeedOled.putString("S2: ");
           SeeedOled.putNumber(ptr[3]); //Print the String
           SeeedOled.putString("/");
           SeeedOled.putNumber(ptr[4]); //Print the String
           SeeedOled.putString("/");
           SeeedOled.putNumber(ptr[5]); //Print the String
           //SeeedOled.putString("S2: Upload    ");
           Serial.println("Finish");
          delay(1000);
           s2_status.set(1);
           s2_pm.set(ptr[3]);
           s2_temp.set(ptr[4]);
           s2_hum.set(ptr[5]);
           }
           strcpy(buf,"");
           delay(1000);
           }
      
      else if(a==3){
         // check status
         str = String(ptr[1]);
         a = str.toInt();
         
         if(a==0){
           // no reply
           s1_status.set(0);
           SeeedOled.setTextXY(5,0);          //Set the cursor to Xth Page, Yth Column
           SeeedOled.putString("S3: Disconnect");
         }
         else{
           Serial.print("Upload Station 3......");
           SeeedOled.setTextXY(5,0);          //Set the cursor to Xth Page, Yth Column
           SeeedOled.putString("S3: ");
           SeeedOled.putNumber(ptr[3]); //Print the String
           SeeedOled.putString("/");
           SeeedOled.putNumber(ptr[4]); //Print the String
           SeeedOled.putString("/");
           SeeedOled.putNumber(ptr[5]); //Print the String
           //SeeedOled.putString("S3: Upload    ");           
           Serial.println("Finish");
           delay(1000);
           s3_status.set(1);
           s3_pm.set(ptr[3]);
           s3_temp.set(ptr[4]);
           s3_hum.set(ptr[5]);
         }
         strcpy(buf,"");
         delay(1000);
      }
    }
  }
}

/***** Split String Function *****/
int* str_split(const char* str, const char* sp){
    int *ptr;
    char *temp;

    // 計算字串內總共有幾個數
    int len=1;
    for (int i = 0; i < strlen(str); ++i){
        if ( str[i] == sp[0])
            len++;
    }

    // 根據長度配置記憶體
    ptr = (int*)malloc(sizeof(int)*(len+1));
    *ptr = (int)(len);
    
    // 開始切割
    int tempflag=0,index=1;
    temp = (char*)malloc(sizeof(char));
    for (int i = 0; i <= strlen(str); ++i){
        if ( str[i] == sp[0] || i==strlen(str)){
            len++;
            tempflag=0;
            // printf("temp=%c%c\n",temp[0],temp[1] );
            sscanf(temp, "%d", (ptr+index));
            index++;
            free(temp);
            temp = (char*)malloc(sizeof(char));
        }
        else{
            temp[tempflag++]=str[i];
        }
    }
    free(temp);
    return ptr;
}


