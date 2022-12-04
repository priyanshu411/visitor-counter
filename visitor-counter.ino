
#include <WiFi.h>
#include <ThingSpeak.h> // always include thingspeak header file after other header files and custom macros

#define SECRET_SSID "Wokwi-GUEST"    // replace with your WiFi network name
#define SECRET_PASS "" // replace with your WiFi password

#define SECRET_CH_ID 1917824 // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "M1ST8LURDIJYYWKN" // replace XYZ with your channel write API Key

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

const int TRIG_PIN_IN = 12;
const int ECHO_PIN_IN = 14;

const int TRIG_PIN_OUT = 5;
const int ECHO_PIN_OUT = 4;
long totalVisitors, countIn, countOut;


void setup()
{
  setSensorPin();
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop()
{
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this  line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  if (getSensorDataIn()) {
    countIn++;
  }
  Serial.println("In :" + String(countIn));
  delay(2000);

  if (getSensorDataOut()) {
    if (countIn > 0) {
      totalVisitors++;
      countIn--;
    }
    Serial.println("total visitors" + String(totalVisitors));
    Serial.println("In :" + String(countIn));
    digitalWrite(13, HIGH);
    tone(13, 200, 1000);
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeField(myChannelNumber, 1, totalVisitors, myWriteAPIKey);
    if (x == 200)
    {
      Serial.println("Channel update successful.");
      totalVisitors = 0;
    }
    else
    {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    // Wait 7 seconds to update the channel again
  }
  delay(2000);
  digitalWrite(13, LOW);
  delay(1000);
}

// setup sensor pin
void setSensorPin() {
  pinMode(13, OUTPUT); //buzzer and led
  pinMode(TRIG_PIN_IN, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(ECHO_PIN_IN, INPUT);

  pinMode(TRIG_PIN_OUT, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(ECHO_PIN_OUT, INPUT);
}
// get sensor data for in
boolean getSensorDataIn() {
  // Clears the trigPin condition
  digitalWrite(TRIG_PIN_IN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN_IN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_IN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(ECHO_PIN_IN, HIGH);
  // Calculating the distance
  int distanceCm = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  // Serial.print("Distance in: ");
  // Serial.print(distanceCm);
  // Serial.println(" cm");
  if (distanceCm > 0 && distanceCm < 50 ) {
    return true;
  }
  return false;
}

// get sensor data for out
boolean getSensorDataOut() {
  // Clears the trigPin condition
  digitalWrite(TRIG_PIN_OUT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN_OUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_OUT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(ECHO_PIN_OUT, HIGH);
  // Calculating the distance
  int distanceCm = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  // Serial.print("Distance out: ");
  // Serial.print(distanceCm);
  // Serial.println(" cm");
  if (distanceCm > 0 && distanceCm < 50 ) {
    return true;
  }
  return false;
}
