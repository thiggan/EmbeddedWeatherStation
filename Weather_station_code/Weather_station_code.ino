#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <U8g2lib.h>
#include <SPL06-007.h>
#include <Wire.h>
#include <U8x8lib.h>

// variables that store ascii formatted data
// which gets written to the lcd
const char DEGREE_SYMBOL[] = {0xB0, '\0'};
char lcd_dht_temperature[5];
char lcd_dht_humidity[5];

// gps stuff
//
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

#define DHTTYPE DHT11
DHT dht(7, DHTTYPE);

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/10, /* reset=*/8);

void setup()
{
  Serial.begin(9600);

    setup_lcd();
    setup_dht();
    setup_pressure();
    setup_gps();
}

void loop()
{
   write_lcd();
   write_serial();
}

// code to setup the various devices
//
void setup_lcd()
{
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_helvB10_tf);
  u8g2.setColorIndex(1);
}
void setup_dht()
{
  dht.begin();
}
void setup_pressure()
{
  Wire.begin(); // begin Wire(I2C)
  // Serial.begin(9600); // begin Serial
  // Serial.println("\nGoertek-SPL06-007 Demo\n");
  SPL_init(); // Setup initial SPL chip registers
}
void setup_gps()
{
  //  Serial.begin(9600);
  ss.begin(GPSBaud);
  // Serial.println(F("DeviceExample.ino"));
  // Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  // Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  // Serial.println(F("by Mikal Hart"));
  // Serial.println();
}

// logic to write the device data to the attached lcd
//
void write_lcd()
{
  u8g2.firstPage();
  do
  {
    lcd_read_dht();
    lcd_read_gps();
    lcd_read_spl06();

    u8g2.drawFrame(0,0,128,31);
    u8g2.drawFrame(0,33,128,31);

    // temperature
    u8g2.drawStr(15, 13, "Temperature");
    u8g2.drawStr(35, 28, lcd_dht_temperature);
    u8g2.drawUTF8(70, 28, DEGREE_SYMBOL);
    u8g2.drawUTF8(76, 28, "C");

    // humidity
    u8g2.drawStr(30, 46, "Humidity");
    u8g2.drawStr(37, 61, lcd_dht_humidity);
    u8g2.drawStr(75, 61, "%");

    // gps

    // spl06

  } while (u8g2.nextPage());
}

void lcd_read_dht()
{
  float t = dht.readTemperature();
  dtostrf(t, 3, 1, lcd_dht_temperature);

  float h = dht.readHumidity();
  dtostrf(h, 3, 1, lcd_dht_humidity);
}
void lcd_read_gps()
{
  // todo read values from gps into lcd_gps_xxx
}
void lcd_read_spl06()
{
  // todo read values from gps into lcd_spl06_xxx
}

// logic to write the device data in CSV format to the serial port
//
void write_serial()
{


   write_dht_serial();
   write_gps_serial();
   //write_spl06_serial();

  Serial.println();
}

void write_dht_serial()
{
  float t = dht.readTemperature();
  Serial.print(t);
  Serial.print(",");

  float h = dht.readHumidity();
  Serial.print(h);
  Serial.print(",");
}
void write_gps_serial()
{
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      write_gps_serial_O();
    }
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected: check wiring."));
      while(true);
    }
  }
}
void write_gps_serial_O()
{
  printInt2(gps.satellites.value(), gps.satellites.isValid());
  Serial.print(",");
  printFloat2(gps.hdop.hdop(), gps.hdop.isValid(), 1);
  Serial.print(",");
  printFloat2(gps.location.lat(), gps.location.isValid(), 6);
  Serial.print(",");
  printFloat2(gps.location.lng(), gps.location.isValid(), 6);
  Serial.print(",");
  // printInt2(gps.location.age(), gps.location.isValid());
  // Serial.print(",");
  // printDateTime(gps.date, gps.time);
  // Serial.print(",");
  // printFloat2(gps.altitude.meters(), gps.altitude.isValid(), 2);
  // Serial.print(",");
  // printFloat2(gps.course.deg(), gps.course.isValid(), 2);
  // Serial.print(",");
  // printFloat2(gps.speed.kmph(), gps.speed.isValid(), 2);
  // Serial.print(",");
  // printStr2(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "", 1);
  // Serial.print("");

  //Serial.println();
  smartDelay(500);
}
void write_spl06_serial()
{
  // ---- Temperature Values ----------------

  //Serial.print("Temperature C");
  Serial.print(get_temp_c());
  Serial.print(",");

  //Serial.print("Temperature F");
  Serial.print(get_temp_f());
  Serial.print(",");

  // ---- Pressure Values ----------------
  
  //Serial.print("Measured Air Pressure");
  Serial.print(get_pressure());
  Serial.print(",");
  //Serial.println(" mb");

}

// borrowed from https://github.com/mikalhart/TinyGPSPlus/blob/master/examples/FullExample/FullExample.ino
//

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

// used chatgpt to rewrite this so it doenst pad
static void printFloat2(float val, bool valid, int prec)
{
  if (!valid)
  {
    Serial.print("");
  }
  else
  {
    Serial.print(val, prec);
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

// using chatgpt to change this so it doesnt pad
static void printInt2(unsigned long val, bool valid)
{
  char sz[32];
  if (valid)
    sprintf(sz, "%lu", val);
  else
    sprintf(sz, "");
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F(""));
  }
  else
  {
    char sz[32];
    // sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    sprintf(sz, "%02d-%02d-%02d ", d.year(), d.month(), d.day());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F(""));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

// the origonal version
static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}

// using chat gpt to rewrite this so it doesnt padd
static void printStr2(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < slen && i < len; ++i)
    Serial.print(str[i]);
  smartDelay(0);
}