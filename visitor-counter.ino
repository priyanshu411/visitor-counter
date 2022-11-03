#include <SoftwareSerial.h>

#define echoPin 2
#define trigPin 3
#define RX 4
#define TX 5
// variable for sensor
long duration;
int distance;
int count = 0;
//variable for wifi
String AP = "hello";              // AP NAME
String PASS = "hello@hello";      // AP PASSWORD
String API = "6NVT59OJR1YMLAN4";  // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
long valSensor = 1;
SoftwareSerial esp8266(RX, TX);


void setup() {
  Serial.begin(9600);
  //confi for sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //confi for wifi
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void loop() {
  //valSensor = getSensorData();
  valSensor =random(1, 11);
  String getData = "GET /update?api_key=" + API + "&" + field + "=" + String(valSensor);
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
}

int getSensorData() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  if (distance > 0 && distance < 10) {
    count++;
    Serial.println("total visitors........."); 
    Serial.println(count); 
  }
  return(count);
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1)) {
    esp8266.println(command);      //at+cipsend
    if (esp8266.find(readReplay))  //ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true) {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false) {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}