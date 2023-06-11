
#include <DHT.h>
#include <U8g2lib.h>
#include <SPL06-007.h>
#include <Wire.h>

#include <U8x8lib.h>

#define DHTTYPE    DHT11

char temperature [5];
char humidity [5];
const char DEGREE_SYMBOL[] = { 0xB0, '\0' };

DHT dht(7, DHTTYPE);

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);

void setup() {  
  Serial.begin(9600); 

  setup_lcd();
  setup_dht();
  setup_pressure(); 
}

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


void loop() {  
  // u8g2.firstPage();
  // do {   
  //   draw();
  // } while( u8g2.nextPage() );

  loop_pressure();
}

void loop_pressure() {
  // ---- Register Values ----------------
  Serial.print("ID: ");
  Serial.println(get_spl_id());
  Serial.print("PRS_CFG: ");
  Serial.println(get_spl_prs_cfg(),BIN);
  Serial.print("TMP_CFG: ");
  Serial.println(get_spl_tmp_cfg(),BIN);
  Serial.print("MEAS_CFG: ");
  Serial.println(get_spl_meas_cfg(),BIN);
  Serial.print("CFG_REG: ");
  Serial.println(get_spl_cfg_reg(),BIN);
  Serial.print("INT_STS: ");
  Serial.println(get_spl_int_sts(),BIN);
  Serial.print("FIFO_STS: ");
  Serial.println(get_spl_fifo_sts(),BIN);

  // ---- Coefficients ----------------
  Serial.print("c0: ");
  Serial.println(get_c0());
  Serial.print("c1: ");
  Serial.println(get_c1());
  Serial.print("c00: ");
  Serial.println(get_c00());
  Serial.print("c10: ");
  Serial.println(get_c10());
  Serial.print("c01: ");
  Serial.println(get_c01());
  Serial.print("c11: ");
  Serial.println(get_c11());
  
  Serial.print("c20: ");
  Serial.println(get_c20());
  
  Serial.print("c21: ");
  Serial.println(get_c21());
  Serial.print("c30: ");
  Serial.println(get_c30());

  // ---- Temperature Values ----------------  
  Serial.print("traw: ");
  Serial.println(get_traw());
  Serial.print("traw_sc: ");
  Serial.println(get_traw_sc(),3);
  
  Serial.print("Temperature: ");
  Serial.print(get_temp_c());
  Serial.println(" C");
  
  Serial.print("Temperature: ");
  Serial.print(get_temp_f());
  Serial.println(" F");
  
  // ---- Pressure Values ----------------
  Serial.print("praw: ");
  Serial.println(get_praw());
  
  Serial.print("praw_sc: ");
  Serial.println(get_praw_sc(),3);
  Serial.print("pcomp: ");
  Serial.println(get_pcomp(),2);
  Serial.print("Measured Air Pressure: ");
  Serial.print(get_pressure(),2);
  Serial.println(" mb");

  // ---- Altitude Values ----------------
  double local_pressure = 1011.3; // Look up local sea level pressure on google // Local pressure from airport website 8/22
  Serial.print("Local Airport Sea Level Pressure: ");
  Serial.print(local_pressure,2);
  Serial.println(" mb");
  
  Serial.print("altitude: ");
  Serial.print(get_altitude(get_pressure(),local_pressure),1);
  Serial.println(" m");
  Serial.print("altitude: ");
  Serial.print(get_altitude_f(get_pressure(),local_pressure),1); // convert from meters to feet
  Serial.println(" ft");


  Serial.println("\n");
  delay(2000);
}

  
void draw(){

  readTemperature();
  readHumidity();
  readData();
  
  u8g2.drawFrame(0,0,128,31);         
  u8g2.drawFrame(0,33,128,31);           
  
  u8g2.drawStr( 15, 13, "Temperature");   
  u8g2.drawStr( 35, 28, temperature);   
  u8g2.drawUTF8(70, 28, DEGREE_SYMBOL);
  u8g2.drawUTF8(76, 28, "C");

  u8g2.drawStr(30,46, "Humidity");         
  u8g2.drawStr( 37, 61, humidity); 
  u8g2.drawStr(75,61, "%");  
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

void readData()
{
  
  float t = dht.readTemperature();
  Serial.print(t); 
  Serial.print(","); 
  float h = dht.readHumidity();
  Serial.print(h); 
  Serial.print(","); 
  Serial.println();
  
}