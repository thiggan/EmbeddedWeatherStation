#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <U8g2lib.h>
#include <SPL06-007.h>
#include <Wire.h>
#include <U8x8lib.h>


// variables that store ascii formatted data 
// which gets written to the lcd
const char DEGREE_SYMBOL[] = { 0xB0, '\0' };
char lcd_dht_temperature [5];
char lcd_dht_humidity [5];

// gps stuff
//
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

#define DHTTYPE    DHT11
DHT dht(7, DHTTYPE);

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);

void setup() {  
  Serial.begin(9600); 

  setup_lcd();
  setup_dht();
  setup_pressure(); 
  setup_gps();
}

void loop() {  
  write_lcd();
  write_serial();

  delay(1000);
}

// code to setup the various devices
//
void setup_lcd() {
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_helvB10_tf); 
  u8g2.setColorIndex(1); 
}
void setup_dht() {
  dht.begin();
}
void setup_pressure() {
  Wire.begin();    // begin Wire(I2C)
  // Serial.begin(9600); // begin Serial
  // Serial.println("\nGoertek-SPL06-007 Demo\n");
  SPL_init(); // Setup initial SPL chip registers   
}
void setup_gps() {
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
void write_lcd() {
  u8g2.firstPage();
  do {   
    lcd_read_dht();
    lcd_read_gps();
    lcd_read_spl06();

    //u8g2.drawFrame(0,0,128,31);         
    //u8g2.drawFrame(0,33,128,31);           

    // temperature
    u8g2.drawStr( 15, 13, "Temperature");   
    u8g2.drawStr( 35, 28, lcd_dht_temperature);   
    u8g2.drawUTF8(70, 28, DEGREE_SYMBOL);
    u8g2.drawUTF8(76, 28, "C");

    // humidity
    u8g2.drawStr(30,46, "Humidity");         
    u8g2.drawStr(37, 61, lcd_dht_humidity); 
    u8g2.drawStr(75,61, "%");  

    // gps

    // spl06

  } while( u8g2.nextPage() );
}

void lcd_read_dht() {
  float t = dht.readTemperature();
  dtostrf(t, 3, 1, lcd_dht_temperature);

  float h = dht.readHumidity();
  dtostrf(h, 3, 1, lcd_dht_humidity);
}
void lcd_read_gps() {
  // todo read values from gps into lcd_gps_xxx
}
void lcd_read_spl06() {
  // todo read values from gps into lcd_spl06_xxx
}

// logic to write the device data in CSV format to the serial port
//
void write_serial() {
  // write_dht_serial();
    write_gps_serial();
  // write_spl06_serial();

  // Serial.println();
}

void write_dht_serial() {
  float t = dht.readTemperature();
  Serial.print(t); 
  Serial.print(","); 

  float h = dht.readHumidity();
  Serial.print(h); 
  Serial.print(","); 
}
void write_gps_serial() {
  // todo

  Serial.print(F("Location: ")); 
      if (gps.location.isValid())
      {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
      }
      else
      {
        Serial.print(F("INVALID"));
      }
      Serial.print(F("  Date/Time: "));
      if (gps.date.isValid())
      {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
      }
      else
      {
        Serial.print(F("INVALID"));
      }
      Serial.print(F(" "));
      if (gps.time.isValid())
      {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
      }
      else
      {
        Serial.print(F("INVALID"));
      }
      Serial.println();
}
void write_spl06_serial() {
  // todo

  // // ---- Register Values ----------------
  // Serial.print("ID: ");
  // Serial.println(get_spl_id());
  // Serial.print("PRS_CFG: ");
  // Serial.println(get_spl_prs_cfg(),BIN);
  // Serial.print("TMP_CFG: ");
  // Serial.println(get_spl_tmp_cfg(),BIN);
  // Serial.print("MEAS_CFG: ");
  // Serial.println(get_spl_meas_cfg(),BIN);
  // Serial.print("CFG_REG: ");
  // Serial.println(get_spl_cfg_reg(),BIN);
  // Serial.print("INT_STS: ");
  // Serial.println(get_spl_int_sts(),BIN);
  // Serial.print("FIFO_STS: ");
  // Serial.println(get_spl_fifo_sts(),BIN);

  // // ---- Coefficients ----------------
  // Serial.print("c0: ");
  // Serial.println(get_c0());
  // Serial.print("c1: ");
  // Serial.println(get_c1());
  // Serial.print("c00: ");
  // Serial.println(get_c00());
  // Serial.print("c10: ");
  // Serial.println(get_c10());
  // Serial.print("c01: ");
  // Serial.println(get_c01());
  // Serial.print("c11: ");
  // Serial.println(get_c11());
  
  // Serial.print("c20: ");
  // Serial.println(get_c20());
  
  // Serial.print("c21: ");
  // Serial.println(get_c21());
  // Serial.print("c30: ");
  // Serial.println(get_c30());

  // // ---- Temperature Values ----------------  
  // Serial.print("traw: ");
  // Serial.println(get_traw());
  // Serial.print("traw_sc: ");
  // Serial.println(get_traw_sc(),3);
  
  // Serial.print("Temperature: ");
  // Serial.print(get_temp_c());
  // Serial.println(" C");
  
  // Serial.print("Temperature: ");
  // Serial.print(get_temp_f());
  // Serial.println(" F");
  
  // // ---- Pressure Values ----------------
  // Serial.print("praw: ");
  // Serial.println(get_praw());
  
  // Serial.print("praw_sc: ");
  // Serial.println(get_praw_sc(),3);
  // Serial.print("pcomp: ");
  // Serial.println(get_pcomp(),2);
  // Serial.print("Measured Air Pressure: ");
  // Serial.print(get_pressure(),2);
  // Serial.println(" mb");

  // // ---- Altitude Values ----------------
  // double local_pressure = 1011.3; // Look up local sea level pressure on google // Local pressure from airport website 8/22
  // Serial.print("Local Airport Sea Level Pressure: ");
  // Serial.print(local_pressure,2);
  // Serial.println(" mb");
  
  // Serial.print("altitude: ");
  // Serial.print(get_altitude(get_pressure(),local_pressure),1);
  // Serial.println(" m");
  // Serial.print("altitude: ");
  // Serial.print(get_altitude_f(get_pressure(),local_pressure),1); // convert from meters to feet
  // Serial.println(" ft");


  // Serial.println("\n");
}