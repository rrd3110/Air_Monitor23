//ARDUINO CODE for the system
//Install these libaries
#include <MQ2.h>
#include <DHT.h>

//Declare Sensor Pins
#define RCWLPIN 7
#define DHTPIN A1    
#define MQ2PIN A0  

DHT dht(DHTPIN, DHT11);
MQ2 mq2(MQ2PIN);

//Global Variable Declaration
int peopleCount = 0;

//Declare Elapsed Time Intervals
unsigned long t1 = 0;  
unsigned long t2 = 0;  
unsigned long t3 = 0;
const long i1 = 1000;  // 1 second
const long i2 = 1000;  // 1 second
const long i3 = 500;   // 0.5 seconds

void setup() {
  Serial.begin(9600); //baud rate can be set accordingly
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
}

void loop() {
  unsigned long curr = millis();
  if(curr - t1 >= i1) {t1 = curr; temp();}
  if(curr - t2 >= i2) {t2 = curr; gasSensor();}
  if(curr - t3 >= i3) {t3 = curr; motion();}
  
  // Print people count every time loop runs
  Serial.print("People count: ");
  Serial.println(peopleCount);
}
//People Count
void motion() {
  static bool motionDetected = false;  // Use a static variable to remember motion state
  int currentMotionState = digitalRead(RCWLPIN);

  if (currentMotionState == HIGH && !motionDetected) {
    // Increment people count only when motion is initially detected
    peopleCount++;
    motionDetected = true;
  } else if (currentMotionState == LOW) {
    // Reset motion detection when motion is no longer detected
    motionDetected = false;
  }
}

//Only LPG, Smoke, CO gas are being monitored
void gasSensor() {
  delay(1000); // 1-second delay
  double alcohol = mq2.readLPG()/1000;
  double smoke = mq2.readSmoke()/1000;
  double co = mq2.readCO()/1000;
  double AQI=(alcohol+smoke+co)*3;

  Serial.print("LPG: ");  
  Serial.print(abs(alcohol));  
  Serial.print(" ppm\tSmoke: ");
  Serial.print(abs(smoke));    
  Serial.print(" ppm\tCO: ");
  Serial.print(abs(co));       
  Serial.print(" ppm\tAQI:");
  Serial.print(abs(AQI));       
  Serial.println(" ppm");
}

//Monitoring of Temperature and Humidity
void temp() {
  float humidity = dht.readHumidity();      
  float temperature = dht.readTemperature();

  Serial.print(" Humidity:");    
  Serial.print(humidity, 2);    
  Serial.print(" %"); 
  Serial.print("\t\t");
  Serial.print(" Temperature:");
  Serial.print(temperature, 2);
  Serial.print(" C");  
  Serial.println();
}
