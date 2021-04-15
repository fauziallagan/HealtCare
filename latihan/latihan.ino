#include <ESP8266WiFi.h>

String apiKey = "SU3XVAMCN2T6M1IP"; // Write API thingspeak 
const char* ssid = "UGMURO-1";
const char* password = "Piscok2000";
const char* server = "api.thingspeak.com";
WiFiClient client;

int adc;
boolean counter;
int count;
int nilai_bpm;
unsigned long millisBefore;
unsigned long beatTime = 20000;
const int threshold = 500;


void setup() {
  Serial.begin(115200);
  counter = true;
  millisBefore = millis();
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

}

void loop() {
  nilai_bpm = (count*(60000/beatTime));
  adc = analogRead(A0);

    //Serial.println(adc);
  delay(10);
  if ((millis() - millisBefore) < beatTime) {
    if (counter == true) {
      if (adc >= threshold) {
        count++;
        counter = false;
        Serial.print("Beat : ");
        Serial.println(count);
      }
    }
    if (adc < threshold) {
      counter = true;
    }
  }
  else {
    Serial.print(nilai_bpm);
    Serial.println(" BPM");
    kirim();
    count = 0;
    millisBefore = millis();
    delay(4000);
  }
}

void kirim()
{
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += float(nilai_bpm);
    //  postStr +="&field2=";
    //  postStr += int(dataJarak);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n\n");
    client.print(postStr);
    Serial.println(" Sending data to Thingspeak");
  }
  client.stop();
  Serial.println("Waiting 20 secs");
   millisBefore = millis();
}
