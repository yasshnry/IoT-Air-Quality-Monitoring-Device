#define BLYNK_TEMPLATE_ID "TMPLqWoeeHxG"
#define BLYNK_DEVICE_NAME "DHT11"

#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial
#define USE_NODE_MCU_BOARD
#include "BlynkEdgent.h"
#include "DHT.h"
#define DHTPIN D2 
#define DHTTYPE DHT11


DHT dht(DHTPIN, DHTTYPE);
float t, h;
int mq135 = A0;           
int data = 0;

void sendSensor()
{
 
 
  h = dht.readHumidity();
  t = dht.readTemperature();  
  Blynk.virtualWrite(V6, h);
  Blynk.virtualWrite(V5, t);
  data = analogRead(mq135); 
  Blynk.virtualWrite(V0, data);
  

  if(t > 32 )
  {
   // Blynk.email/noti
    Blynk.logEvent("temperature_alert","Temperature above 33 degree");
  }
  if( h > 73)
  {
   // Blynk.email/noti
    Blynk.logEvent("humidity_alert","Humidity above 74%");
  }
  if( data > 1000)
  {
   // Blynk.email/noti
    Blynk.logEvent("co2_alert","CO2 above 1000PPM");
  }
}

void setup()
{
  
  Serial.begin(9600);
  dht.begin();
  BlynkEdgent.begin();
  delay(2000); 
  timer.setInterval(1000L, sendSensor); 
}

void loop() 
{
  BlynkEdgent.run();
  timer.run(); 
}
