/*

  This program fetches the available data from the server and displays it on an LCD display.
  This is primarily used as a Proof of Concept, for the larger application with a larger display.

*/

#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// ********** WiFi Access Details ********* //
const char *ssid = "Aaron'sWife,Hi";  
const char *password = "angels12";

// ************ Chosen Carpark *********** //
const char *parkingCode = "LSF";

// Creates an lcd object of an lcd module that is connected to the below pins
LiquidCrystal lcd(15, 13, 0, 4, 5, 16);

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  lcd.begin(16, 2);
  // Prevents reconnection issue (taking too long to connect)
  WiFi.mode(WIFI_OFF);

  delay(1000);

  // This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);        
  
  // Connects to the WiFi with the supplied access details
  WiFi.begin(ssid, password);     

  lcd.clear();
  lcd.print("Connecting");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    // Delays before it attempts to connect again
    delay(500);
  }

  // CLears the screen and then prints the local IP address of the unit
  lcd.clear();
  lcd.print("Done");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {

  // Declare object of class HTTPClient
  HTTPClient http;    

  // The parking lot which wlll be used is P6 (North Stop)
  String code = "LSF";

  // GET Data which is what the server requires
  String getData = "?code=" + code;

  // The full URL with the data needed to submit the HTTP GET request
  String Link = "http://uctcarpark.herokuapp.com/api/Parkings/" + getData;
  
  http.begin(Link);     
  
  // Gets the HTTP Code from the connection
  int httpCode = http.GET();

  // Retrieves the data as a response payload
  String payload = http.getString();    

  // Puts the payload into a JavaScript Object Notation (JSON) format
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  // Gets the values out of the payload
  String availString = doc[0]["available"];
  String parkingName = doc[0]["parkingName"];
  int avail = availString.toInt();

  // Prints the availability to the lcd screen
  lcd.clear();
  lcd.print("Name: ");
  lcd.print(parkingName); 
  lcd.setCursor(0, 1);
  lcd.print("Available: ");
  lcd.print(avail); 

  // Closes the connection
  http.end();  //Close connection
  
  // Wait for 5 seconds before getting the data from the server
  delay(5000);
}
//=======================================================================
