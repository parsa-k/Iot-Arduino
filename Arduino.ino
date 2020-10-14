we used the Arduino Yun board. 
And its code is as follows:




#include <OneWire.h>
#include <DallasTemperature.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// Variables
float Celcius=0; 
float Fahrenheit=0;
int dac = 0;
int DigitalPin[] = {2, 12, 13};
int DacPin = 3;
int pinLED = 13;
YunServer server;


// Define inputs and outputs
void setup() {
 Serial.begin(9600);
Bridge.begin();
sensors.begin();
pinMode(2,INPUT);
pinMode(4,INPUT);
pinMode(12,OUTPUT);
pinMode(13,OUTPUT);
digitalWrite(pinLED, LOW);
server.listenOnLocalhost(); // localhost
server.begin();
SerialUSB.begin(9600);// SerialUSB for showing information
while (!SerialUSB);// You must click to continue
SerialUSB.println("Starting bridge...\n");
  delay(2000);
  Process wifiCheck; 
  wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");
   while (wifiCheck.available() > 0) {
  char c = wifiCheck.read();
    SerialUSB.print(c);
     }

  SerialUSB.println();
}

// whole running code
void loop() {
YunClient client = server.accept();
if (client) {
process(client);
client.stop();
}
delay(50);
}

//Recognize the type of command
void process(YunClient client) {
String command = client.readStringUntil('/');
command.trim();
//Serial.print(command); // for being sure
//Serial.println(); // for being sure
if (command == "digital") {
digitalCommand(client);
}
if (command == "dac") {
dacCommand(client);
}
if (command == "status") {
statusCommand(client);
}
if (command=="adac"){
  dacc(client);
}
if (command == "led") {
   led(client); 
}

if (command == "temperature"){
  temperature(client);
}
if (command == "f") {
  fahrenheit(client);
}
if (command== "e"){
  report(client);
}
}



void digitalCommand(YunClient client) {
int pin, value;
pin = client.parseInt();
if (client.read() == '/') {
value = client.parseInt();
digitalWrite(pin, value);
}
else {
value = digitalRead(pin);
}
client.print(F("digital,"));
client.print(pin);
client.print(F(","));
client.println(value);
}



void dacCommand(YunClient client) {
int pin, value;
pin = client.parseInt();
if (client.read() == '/') {
value = client.parseInt();
dac = value;
analogWrite(pin, value);
}
else {
value = dac;
}
client.print(F("dac,"));
client.print(pin);
client.print(F(","));
client.println(value);
}



void statusCommand(YunClient client) {
int pin, value;
client.print(F("status"));
for (int thisPin = 0; thisPin < 3; thisPin++) {
pin = DigitalPin[thisPin];
value = digitalRead(pin);
client.print(F("#"));
client.print(pin);
client.print(F("="));
client.print(value);
}
{
pin = DacPin;
value = dac;
client.print(F("#"));
client.print(pin);
client.print(F("="));
client.print(value);
}
{
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
//  Serial.print(Celcius);
 // Serial.println("℃ ");
 // Serial.print(Fahrenheit);
 // Serial.println("℉");
      float temperature = Celcius;
client.print(F("#A0"));
client.print(F("="));
client.print(temperature);
}
client.println("");
}
void dacc(YunClient client){
  int value = client.parseInt();
//Serial.print(value);
analogWrite(DacPin, value);
}

void led(YunClient client){
  int value = client.parseInt();
   if (value == 131) {
digitalWrite(pinLED, HIGH);
}
   if (value == 130) {
digitalWrite(pinLED, LOW);
}
   if (value == 121) {
digitalWrite(12, HIGH);
}
    if (value == 120) {
digitalWrite(12, LOW);
} 
}



void temperature(YunClient client){
    sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
      client.print(Celcius);
      //Serial.print(Celcius); 
}
void fahrenheit(YunClient client){
       sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
      client.print(Fahrenheit);
     // Serial.print(Fahrenheit); 
}
void report(YunClient client){
  int pin=2,value;
  String report;
 value = digitalRead(pin); 
 if (value==1){
  report="Enable";
  client.print(report);
 }
 else{
  report="Disable";
  client.print(report);
 }
}
