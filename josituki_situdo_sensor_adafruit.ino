//2015.7.15 nokia5110液晶にsht11から取得した温度を表示する。
//それと同時にPWMに湿度に比例した電圧を出力する。
//これにより、日立除湿機RD-10NXの湿度センサの機能を置き換える。
//Arduino pro miniで動作する


#include <SPI.h>
#include <Sensirion.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

const uint8_t dataPin  =  4;
const uint8_t clockPin =  7;

float temperature;
float humidity;
float dewpoint;
float p;
Sensirion tempSensor = Sensirion(dataPin, clockPin);

//LCD nokia5110関係のヘッダ
#define PIN_SCE   6 //Pin 3 on LCD
#define PIN_RESET 5 //Pin 4 on LCD
#define PIN_DC    8 //Pin 5 on LCD
#define PIN_SDIN  11 //Pin 6 on LCD
#define PIN_SCLK  13 //Pin 7 on LCD

//Adafruit_PCD8544(int8_t SCLK, int8_t DIN, int8_t DC, int8_t CS, int8_t RST);
//Adafruit_PCD8544 display=   Adafruit_PCD8544(13,11,8,6,5);


  Adafruit_PCD8544 display = Adafruit_PCD8544(8, 6, 5);
//  Adafruit_PCD8544(int8_t DC, int8_t CS, int8_t RST);


#define LED_PIN 10
bool blinkState = false;
int y=0;
int val=0;

void setup()
{
  	Serial.begin(115200);
    pinMode(10, OUTPUT); 
//    TCCR1B &= B11111000;
//  TCCR1B |= B00000001;v
  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

  // draw a single pixel
  display.drawPixel(10, 10, BLACK);
  display.display();
  delay(2000);
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(2000);

//	Serial.print("Welcome to sensirion sensor.\r\n");
}


void loop()
{
	char buf[128];  //文字列出力用
     
	//LED点滅
//	blinkState = !blinkState;
//	digitalWrite(LED_PIN, blinkState);
  if (Serial.available() > 0) { // シリアル通信でデータが送られてくるまで待つ。
    char c = Serial.read(); // 一文字分データを取り出す。
    if (c == '1')val+=10;
    if(c=='2')val-=20;
    analogWrite(10,val);   
    Serial.println(val);
    
  }

        tempSensor.measure(&temperature, &humidity, &dewpoint);
  Serial.print(millis()/1000);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",Humidity[%],");
  Serial.print(humidity);
  Serial.print(", Dewpoint[C],");
  Serial.print(dewpoint);
  Serial.print("\r\n");
  delay(1000);

	byte i;

  //LCDへの温度の表示
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  dtostrf(temperature, 5, 1, buf);
  display.print(buf);display.println("C");

  
  //LCDへの湿度の表示
//  display.setCursor(0,1);
  dtostrf(humidity, 5, 1, buf);
  display.print(buf);display.println("%");

  
   //LCDへの露点温度の表示
//  display.setCursor(0,2);
  dtostrf(dewpoint, 5, 1, buf);
  display.print(buf);display.println("C");
  display.display();
 
        blinkState = !blinkState;
	digitalWrite(LED_PIN, blinkState);


}




