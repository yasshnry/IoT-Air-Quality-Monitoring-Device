
/*
 DHT 11 to Arduino
 *GND - GND 
 *VCC - 3.3V 
 *DAT - 7
 
 ST7920 to Arduino
 *GND - GND
 *VCC - 5V
 *RS  - 10
 *R/W - 11
 *E   - 13
 *PSB - GND
 *RST - 8
 *BLA - 3.3V
 *BLK - GND
 
 Relay to arduino
 *vcc - 5v
 *gnd - gnd
 *in  - a5
 
 Relay to fan
 *com - 12v(b) fan(r)
 *nc  - fan (b)
*/

#include <DHT.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#define DHTTYPE    DHT11
#define RELAY_FAN_PIN A5 // Arduino pin connected to relay which connected to fan


char temperature [5];
char humidity [5];
const char DEGREE_SYMBOL[] = { 0xB0, '\0' };
const int TEMP_THRESHOLD_UPPER = 33; // upper threshold of temperature, change to your desire value
const int TEMP_THRESHOLD_LOWER = 31; // lower threshold of temperature, change to your desire value
const int HUM_THRESHOLD_UPPER = 74; // upper threshold of temperature, change to your desire value
const int HUM_THRESHOLD_LOWER = 72; // lower threshold of temperature, change to your desire value
int mq135 = A0;           //AQI sensor is connected with the analog pin A0
int data = 0;

DHT dht(7, DHTTYPE);

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);

void setup() 
{  

  dht.begin();
  pinMode(RELAY_FAN_PIN, OUTPUT); // initialize digital pin as an output
  Serial.begin(9600); 
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_helvB10_tf); 
  u8g2.setColorIndex(1);  
 
}

void loop() 
{  
  u8g2.firstPage();
  do 
  {   
    draw();
  } while( u8g2.nextPage() );

  // wait a few seconds between measurements.
  delay(2000);

  float temperature;    // temperature in Celsius
  float humidity;    // temperature in percentage
  temperature = dht.readTemperature(); // read temperature in Celsius
  humidity = dht.readHumidity();  // read temperature in Cels
  
  if (isnan(temperature && humidity)) 
  {
    Serial.println("Failed to read from DHT sensor!");
  } 
  else
  {
    if(temperature > TEMP_THRESHOLD_UPPER | humidity > HUM_THRESHOLD_UPPER)
    {
      Serial.println("The fan is turned on");
      digitalWrite(RELAY_FAN_PIN, HIGH); // turn on
    } 
    else if(temperature < TEMP_THRESHOLD_LOWER | humidity < HUM_THRESHOLD_LOWER)
    {
      Serial.println("The fan is turned off");
      digitalWrite(RELAY_FAN_PIN, LOW); // turn on
    }
  }

}

void draw()
{

  readTemperature();
  readHumidity();

  
  u8g2.drawFrame(0,0,128,64);         
  
  u8g2.drawStr( 16, 20, "AIR QUALITY");      //tittle size
  
  u8g2.drawStr( 11, 40, "Temp :");            //temp
  u8g2.drawStr( 66, 40, temperature);        // bacaan tempt
  u8g2.drawUTF8(96, 40, DEGREE_SYMBOL);
  u8g2.drawUTF8(101, 40, "C"); 

  u8g2.drawStr(12,56, "Hum  :");              //humid  
  u8g2.drawStr( 66, 56, humidity);           // bacaan humidity
  u8g2.drawStr(101,56, "%");  

}

void readTemperature()
{
  float t = dht.readTemperature();
  dtostrf(t, 3, 1, temperature);
}

void readHumidity()
{
  float h = dht.readHumidity();
  dtostrf(h, 3, 1, humidity);

}
