#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

#define ADDR 0x06
#define DT 100

const int in = 2;

int lux = 0;
int move = 0;

void setup(){
  Serial.begin(115200);
  delay(2000);
  Serial.println();

  Serial.println("Starting Room Node");
  
  Wire.begin(ADDR);
  Wire.onReceive(recv);
  Wire.onRequest(send);

  pinMode(in, INPUT);

  if(!tsl.begin()){
    Serial.println("No lux sensor found");
    while(1);
  }
  Serial.println("lux sensor found");

  configureSensor();

  Serial.print("I2c opening on addr: ");
  Serial.println(ADDR);
  Serial.println("Ready ...");
}

void configureSensor(void){
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
}

void loop(){
  sensors_event_t event;
  tsl.getEvent(&event);

  lux = (int) event.light;
  Serial.print("Lux: ");
  Serial.println(lux);

  move = (digitalRead(in)) ? 1 : 0;
  Serial.print("PIR: ");
  Serial.println(move);
  delay(DT);
}

void recv(int i){
  byte r = Wire.read();
  Serial.println(r);
}

void send(){
  Wire.write(lux);
}

